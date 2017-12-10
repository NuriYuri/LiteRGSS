#include "CViewport_Element.h"

void CViewport_Element::draw(sf::RenderTarget& target) const
{
    target.setView(view);
    for(auto sp = stack.begin();sp != stack.end();sp++)
    {
        (*sp)->draw(target);
    }
}

void CViewport_Element::bindSprite(CSprite_Element* sprite)
{
    stack.push_back(sprite);
}

void CViewport_Element::clearStack() 
{
    stack.clear();
}