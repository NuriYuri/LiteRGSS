#include "CViewport_Element.h"

void CViewport_Element::draw(sf::RenderTarget& target) const
{
    target.setView(view);
    for(auto sp = stack.begin();sp != stack.end();sp++)
    {
        (*sp)->draw(target);
    }
}