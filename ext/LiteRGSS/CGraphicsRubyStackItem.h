#ifndef CGraphicsRubyStackItem_H
#define CGraphicsRubyStackItem_H

#include "ruby.h"

class CGraphicsRubyStack;
class CGraphicsRubyStackItem {
    private:
        friend class CGraphicsRubyStack;
        void setOriginStack(CGraphicsRubyStack *o);
        

        CGraphicsRubyStack *origin_stack = nullptr;
        VALUE m_self = Qnil;
    public:
        ~CGraphicsRubyStackItem() = default;
        
        void setSelf(VALUE self) { m_self = self; onSelfSetted(m_self);}
    protected:
        void overrideOriginStack(CGraphicsRubyStack* stack = nullptr);
        VALUE self() const { return m_self; }
        virtual void onSelfSetted(VALUE self) {}

        CGraphicsRubyStackItem() = default;
        
        void setOriginStack(CGraphicsRubyStack& o);
        void resetOriginStack();
};

#endif