#include <iostream>
#include "CDrawable_Element.h"
#include "CRect_Element.h"

extern ID rb_iElementTable;
extern VALUE rb_mGraphics;
extern VALUE rb_eRGSSError;

void CDrawable_Element::resetOriginStack() {
    setOriginStack(nullptr);
}

void CDrawable_Element::setOriginStack(std::vector<CDrawable_Element*>& o) {
    setOriginStack(&o);
}

void CDrawable_Element::bindRect(CRect_Element* rect) {
    if(rect != linkedRect) {
        auto lastLinkedRect = linkedRect;
        linkedRect = nullptr;
        if(lastLinkedRect != nullptr) {
            lastLinkedRect->bindElement(nullptr);
        }
        linkedRect = rect;
        if(linkedRect != nullptr) {
            linkedRect->bindElement(this);
        }
    }
}

template <typename S, typename Checker>
inline bool erase_if(S & container, Checker&& checker) {
    auto it = std::begin(container);
    auto last = std::end(container);
    while(it != last) {
		if(Checker(checker)(*it)) {
			it = container.erase(it);
            return true;
		} else {
			it++;
        }
	}
    return false;
}

void CDrawable_Element::setOriginStack(std::vector<CDrawable_Element*> *o)  {
    /* Removing from the old stack */
    if(origin_stack != nullptr)
    {
        if(!erase_if(*origin_stack, [&](const CDrawable_Element* drawable) { return drawable == this; })) {
            rb_raise(rb_eRGSSError, "Desynchronized graphics stack");
        }
    }
    origin_stack = o;
    if(o != nullptr)
    {
        origin_stack->push_back(this);
        if(index == 0) {
            index = origin_stack->size();
        }
    }
}

CDrawable_Element::~CDrawable_Element() {  
    bindRect(nullptr);

    resetOriginStack();
    if(!disposeFromViewport_) {
        VALUE table = rb_ivar_get(NIL_P(rViewport) ? rb_mGraphics : rViewport, rb_iElementTable);
        rb_check_type(table, T_ARRAY);
        /* Suppression du drawable de ses stacks */
        rb_ary_delete(table, self);
    }

    RDATA(self)->data = nullptr;
}

unsigned long CDrawable_Element::getIndex() {
    return index;
}
