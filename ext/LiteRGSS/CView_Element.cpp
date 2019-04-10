#include "CView_Element.h"

void CView_Element::syncStackCppFromRuby() {
    stack->syncStackCppFromRuby();
}

void CView_Element::bind(CDrawable_Element& drawable) {
	stack->bind(drawable);
}

void CView_Element::drawFast(sf::RenderTarget& target) const  {
    stack->drawFast(target);
}

void CView_Element::updateContentsOpacity(long opacity) {
    stack->updateContentsOpacity(opacity);
}
