#include <iostream>
#include "CGraphicsStackItem.h"
#include "CGraphicsStack.h"

void CGraphicsStackItem::resetOriginStack() {
    CGraphicsCppStackItem::resetOriginStack();
    CGraphicsRubyStackItem::resetOriginStack();
}

void CGraphicsStackItem::setOriginStack(CGraphicsCppStack& cppStack, CGraphicsRubyStack& rubyStack) {
    CGraphicsCppStackItem::setOriginStack(cppStack);
    CGraphicsRubyStackItem::setOriginStack(rubyStack);
}

void CGraphicsStackItem::overrideOriginCppStack(CGraphicsCppStack* oCpp) {
    CGraphicsCppStackItem::overrideOriginStack(oCpp);
}

void CGraphicsStackItem::overrideOriginRubyStack(CGraphicsRubyStack* oRuby) {
    CGraphicsRubyStackItem::overrideOriginStack(oRuby);
}

CGraphicsStackItem::~CGraphicsStackItem() {  
    CGraphicsStackItem::resetOriginStack();
}

unsigned long CGraphicsStackItem::getDrawPriority() const {
    return CGraphicsCppStackItem::getDrawPriority();
}
