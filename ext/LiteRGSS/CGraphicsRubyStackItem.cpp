#include "CGraphicsRubyStackItem.h"
#include "CGraphicsRubyStack.h"

void CGraphicsRubyStackItem::resetOriginStack() {
    setOriginStack(nullptr);
}

void CGraphicsRubyStackItem::setOriginStack(CGraphicsRubyStack *o) {
    /* if the stack is already setted, nothing to be done */
    if(origin_stack != o) {
        //Detach & Attach ruby
        if(origin_stack != nullptr) {
            origin_stack->remove(m_self);
        }

        origin_stack = o;

        if(origin_stack != nullptr) {
            origin_stack->add(m_self);
        }
    }
}

void CGraphicsRubyStackItem::overrideOriginStack(CGraphicsRubyStack* stack) {
    origin_stack = stack;
}

void CGraphicsRubyStackItem::setOriginStack(CGraphicsRubyStack& o) {
    setOriginStack(&o);
}
