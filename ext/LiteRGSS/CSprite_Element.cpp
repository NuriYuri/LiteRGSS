#include "CViewport_Element.h"
#include <iostream>

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