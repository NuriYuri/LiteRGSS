#include <iostream>
#include "CDrawable_Element.h"
#include "CRect_Element.h"

extern ID rb_iElementTable;
extern VALUE rb_mGraphics;
extern VALUE rb_eRGSSError;

void CDrawable_Element::resetOriginStack() {
    setOriginStack(nullptr);
}

void CDrawable_Element::setOriginStack(vector_tracker<CDrawable_Element*>& o) {
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

void CDrawable_Element::setOriginStack(vector_tracker<CDrawable_Element*> *o)  {
    /* if the stack is already setted, nothing to be done */
    if(origin_stack == o) {
        return;
    }
    
    /* Removing from the old stack */
    if(origin_stack != nullptr)
    {
        if(!origin_stack->remove(this)) {
            rb_raise(rb_eRGSSError, "Desynchronized graphics stack");
        }
    }
    origin_stack = o;
    if(o != nullptr)
    {
        origin_stack->push_back(this);
        if(drawPriority == 0) {
            drawPriority = origin_stack->size();
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
    return drawPriority;
}
