#include "CDrawable_Element.h"
#include "CRect_Element.h"

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


CDrawable_Element::~CDrawable_Element() {  
    bindRect(nullptr);
}

