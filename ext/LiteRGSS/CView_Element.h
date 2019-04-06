#ifndef CVIEW_ELEMENT_H
#define CVIEW_ELEMENT_H
#include <vector>
#include "CDrawable_Element.h"
#include "CGraphicsStack_Element.h"

class CView_Element : 
    public CDrawable_Element {
public:
    CView_Element() = default;
    ~CView_Element() override = default;
    
    void detachSprites();
    void bind(CDrawable_Element& drawable);
    void drawFast(sf::RenderTarget& target) const override;
    void updateContentsOpacity(long opacity);
    void onSelfSetted() override {
        stack = std::make_unique<CGraphicsStack_Element>(self);
    }

private:
    std::unique_ptr<CGraphicsStack_Element> stack;
};

#endif