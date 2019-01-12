#ifndef CVIEW_ELEMENT_H
#define CVIEW_ELEMENT_H
#include <vector>
#include "CDrawable_Element.h"

class CView_Element : 
    public CDrawable_Element {
public:
    CView_Element() = default;
    virtual ~CView_Element();

    void detachSprites();
    void bind(CDrawable_Element& drawable);
    void drawFast(sf::RenderTarget& target) const override;
    void clearStack();
    
private:
    void disposeSprites();
    std::vector<CDrawable_Element*> stack;

protected:
    const std::vector<CDrawable_Element*>& getStack() const {
        return stack;
    }
};

#endif