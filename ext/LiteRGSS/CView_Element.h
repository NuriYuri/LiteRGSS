#ifndef CVIEW_ELEMENT_H
#define CVIEW_ELEMENT_H
#include <vector>
#include "CDrawable_Element.h"
#include "CDrawableStack.h"

class CView_Element : 
    public CDrawable_Element {
public:
    CView_Element() = default;
    ~CView_Element() override = default;
    
    //Forward from stack
    void syncStackCppFromRuby();
    void add(CDrawable_Element& drawable);
    void drawFast(sf::RenderTarget& target) const override;
    void updateContentsOpacity(long opacity);

    void onSelfSetted(VALUE self) override {
        stack = std::make_unique<CDrawableStack>(self);
    }

private:
    std::unique_ptr<CDrawableStack> stack;
};

#endif