#include <iostream>
#include "CDrawable_Element.h"
#include "CRubyGraphicsStack.h"
#include "CRect_Element.h"

extern VALUE rb_mGraphics;
extern VALUE rb_eRGSSError;

void CDrawable_Element::resetOriginStack() {
    setOriginStack(nullptr, nullptr);
}

void CDrawable_Element::setOriginStack(CRubyGraphicsStack& oRuby, vector_tracker<CDrawable_Element*>& o) {
    setOriginStack(&oRuby, &o);
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

void CDrawable_Element::setOriginStack(CRubyGraphicsStack* oRuby, vector_tracker<CDrawable_Element*> *o)  {
    /* if the stack is already setted, nothing to be done */
    if(origin_stack != o) {
        //Detach & Attach C++
        if(origin_stack != nullptr)
        {
            if(!origin_stack->remove(this)) {
                rb_raise(rb_eRGSSError, "Desynchronized graphics stack");
            }
        }
        origin_stack = o;
        if(origin_stack != nullptr)
        {
            origin_stack->push_back(this);
            if(drawPriority == 0) {
                drawPriority = origin_stack->size();
            }
        }
    }
    
    /* if the stack is already setted, nothing to be done */
    if(origin_ruby_stack != oRuby) {
        //Detach & Attach ruby
        if(origin_ruby_stack != nullptr) {
            origin_ruby_stack->remove(self);
        }

        origin_ruby_stack = oRuby;

        if(origin_ruby_stack != nullptr) {
            origin_ruby_stack->add(self);
        }
    }
}

void CDrawable_Element::overrideOriginCppStack() {
    origin_stack = nullptr;
}

void CDrawable_Element::overrideOriginRubyStack() {
    origin_ruby_stack = nullptr;
}

CDrawable_Element::~CDrawable_Element() {  
    bindRect(nullptr);
    resetOriginStack();

    RDATA(self)->data = nullptr;
}

unsigned long CDrawable_Element::getIndex() {
    return drawPriority;
}
