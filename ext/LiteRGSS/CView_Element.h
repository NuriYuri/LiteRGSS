#ifndef CVIEW_ELEMENT_H
#define CVIEW_ELEMENT_H
#include <vector>
#include "CDrawable_Element.h"
#include "CGraphicsStack_Element.h"
#include "CRubyGraphicsStack.h"

class CView_Element : 
    public CDrawable_Element {
public:
    CView_Element() = default;
    ~CView_Element() override = default;
    
    void syncStackCppFromRuby();
    void bind(CDrawable_Element& drawable);
    void drawFast(sf::RenderTarget& target) const override;
    void updateContentsOpacity(long opacity);
    void onSelfSetted() override {
        stack = std::make_unique<CGraphicsStack_Element>();
        rubyStack = std::make_unique<CRubyGraphicsStack>(self);
    }

private:
    std::unique_ptr<CGraphicsStack_Element> stack;
    std::unique_ptr<CRubyGraphicsStack> rubyStack;
};

#endif