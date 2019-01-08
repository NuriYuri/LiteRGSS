#include <iostream>
#include "CViewport_Element.h"

void CSprite_Element::draw(sf::RenderTarget& target) const 
{
	CSprite_Element::drawFast(target);
}

void CSprite_Element::drawFast(sf::RenderTarget& target) const 
{
    if(drawable && visible)
    {
        target.draw(sprite);
    }
}

CSprite_Element::~CSprite_Element() {
    //std::cout << "Entering Sprite destructor" << std::endl;
    sprite.setColor(sf::Color(255, 255, 255, 255));
    setLinkedRect(nullptr);
    drawable = false;
    visible = false;
}