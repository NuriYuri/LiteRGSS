#include "CSprite_Element.h"
#include <iostream>

void CSprite_Element::draw(sf::RenderTarget& target) const 
{
    if(drawable && visible)
    {
        target.draw(sprite);
    }
}