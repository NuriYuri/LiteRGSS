#include <cassert>
#include "utils/ruby_common.h"
#include "CGraphicsStack_Element.h"
#include "CDrawable_Element.h"

#include "CText_Element.h"
#include "CSprite_Element.h"

CGraphicsStack_Element::~CGraphicsStack_Element() {
	detachSprites();
}

void CGraphicsStack_Element::bind(CRubyGraphicsStack& rubyStack, CDrawable_Element& el) {
	el.setOriginStack(rubyStack, stack);
}

void CGraphicsStack_Element::detachSprites() {
	for(auto& it : stack) {
		it->overrideOrigineStack();
	}
	stack.clear();
}

void CGraphicsStack_Element::drawFast(sf::RenderTarget& target) const {
	for(auto& element: stack) {
		element->drawFast(target);
	}
}

void CGraphicsStack_Element::draw(sf::View& defview, sf::RenderTarget& target) const  {
    bool was_viewport = false;
	for(auto& element: stack) {
        if(was_viewport && !element->isViewport()) {
			target.setView(defview);
        }
        was_viewport = element->isViewport();
        element->draw(target);
    }
}

void CGraphicsStack_Element::updateContentsOpacity(long opacity) {
	for (auto& sp : stack) {
		CText_Element* text = dynamic_cast<CText_Element*>(sp);
		if (text != nullptr) {
			auto& text2 = text->getText();
			sf::Color col = sf::Color(text2.getFillColor());
			col.a = opacity;
			text2.setFillColor(col);
			col = sf::Color(text2.getOutlineColor());
			col.a = opacity;
			text2.setOutlineColor(col);
		} else {
			CSprite_Element* sprite = dynamic_cast<CSprite_Element*>(sp);
			if (sprite != nullptr) {
				sf::Color col = sf::Color(sprite->getSprite().getColor());
				col.a = opacity;
				sprite->getSprite().setColor(col);
			}
		}
	}
}