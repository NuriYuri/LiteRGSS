#include "CDrawable_Element.h"
#include "CRect_Element.h"
#include <iostream>

extern ID rb_iElementTable;
extern VALUE rb_mGraphics;
extern std::vector<CDrawable_Element*> Graphics_stack;
extern bool InsideGraphicsUpdate;
extern VALUE rb_eRGSSError;

void CDrawable_Element::resetOriginStack() {
    setOriginStack(nullptr);
}

void CDrawable_Element::setOriginStack(std::vector<CDrawable_Element*>& o) {
    setOriginStack(&o);
}

void CDrawable_Element::setOriginStack(std::vector<CDrawable_Element*> *o) 
{
    
    /* Removing from the old stack */
    if(origin_stack != nullptr)
    {
        auto it = std::find(origin_stack->begin(), origin_stack->end(), this);
        if(it != origin_stack->end()) {
            origin_stack->erase(it);
        } else {
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
    RDATA(self)->data = nullptr;
    if(disposeFromViewport_) {
        return;
    }
    //std::cout << "Entering Drawable destructor for " << self << " with graphics " << (InsideGraphicsUpdate ? "INSIDE" : "outside") << std::endl;
   
    const auto cppStackSize = origin_stack == nullptr ? 0 : origin_stack->size();
    VALUE table = rb_ivar_get(NIL_P(rViewport) ? rb_mGraphics : rViewport, rb_iElementTable);
    const auto sz = RARRAY_LEN(table);
    if(cppStackSize != sz) {
        rb_raise(rb_eRGSSError, "Desynchronized graphics stack");
    }

    resetOriginStack();
    CRect_Element* rect = getLinkedRect();
    if(rect != nullptr) {
        rect->setElement(nullptr);
    }

    /* Suppression du drawable de ses stacks */
    if(rb_ary_delete(table, self) == Qnil && !(cppStackSize == 0 && sz == 0)) {
        rb_raise(rb_eRGSSError, "Desynchronized graphics stack");
    }
}

unsigned long CDrawable_Element::getIndex()
{
    return index;
}