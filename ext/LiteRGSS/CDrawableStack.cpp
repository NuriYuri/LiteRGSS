#include "CDrawableStack.h"
#include "CText_Element.h"
#include "CSprite_Element.h"

void CDrawableStack::draw(sf::View& defview, sf::RenderTarget& target) const  {
	bool was_viewport = false;
	for(auto& element: *this) {
		if(was_viewport && !element->isViewport()) {
			target.setView(defview);
		}
		was_viewport = element->isViewport();
		element->draw(target);
	}
}

void CDrawableStack::drawFast(sf::RenderTarget& target) const {
	for(auto& element: *this) {
		element->drawFast(target);
	}
}

void CDrawableStack::updateContentsOpacity(long opacity) {
	for (auto& sp : *this) {
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
