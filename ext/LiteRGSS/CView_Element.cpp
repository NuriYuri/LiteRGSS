#include "CView_Element.h"

#include "SpriteDisposer.h"

extern ID rb_iElementTable;

CView_Element::~CView_Element() {
    clearStack();
}

void CView_Element::detachSprites() {
    for(auto& it : stack) {
		it->overrideOrigineStack();
	}
	stack.clear();
}

void CView_Element::clearStack()  {
	DisposeAllSprites(rb_ivar_get(self, rb_iElementTable));
}

void CView_Element::bind(CDrawable_Element& drawable) {
	drawable.setOriginStack(stack);
}

void CView_Element::drawFast(sf::RenderTarget& target) const  {
    for(auto& sp: stack) {
        sp->drawFast(target);
    }
}
