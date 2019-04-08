#include "CView_Element.h"

void CView_Element::syncStacks() {
    rubyStack->syncStack(*stack);
}

void CView_Element::bind(VALUE rubyDrawable, CDrawable_Element& drawable) {
	stack->bind(*rubyStack, drawable);
}

void CView_Element::drawFast(sf::RenderTarget& target) const  {
    stack->drawFast(target);
}

void CView_Element::updateContentsOpacity(long opacity) {
    stack->updateContentsOpacity(opacity);
}
