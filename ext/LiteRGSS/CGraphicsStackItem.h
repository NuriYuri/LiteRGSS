#ifndef CGraphicsStackItem_H
#define CGraphicsStackItem_H
#include "ruby.h" //Fix Windows compilation
#include "CGraphicsCppStackItem.h"
#include "CGraphicsRubyStackItem.h"

class CGraphicsStack;
class CGraphicsRubyStack;
class CGraphicsStackItem : 
    public CGraphicsRubyStackItem,
    public CGraphicsCppStackItem<CGraphicsStackItem> {
    friend class CGraphicsStack;
    private:
        void resetOriginStack();
        void overrideOriginCppStack(CGraphicsCppStack* stack = nullptr);
        void overrideOriginRubyStack(CGraphicsRubyStack* stack = nullptr);

    public:
        CGraphicsStackItem() = default;
        ~CGraphicsStackItem() override = default;

        void setOriginStack(CGraphicsCppStack& cppStack, CGraphicsRubyStack& rubyStack);
        unsigned long getDrawPriority() const;
};
#endif