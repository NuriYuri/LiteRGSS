#include "CText_Element.h"

void CText_Element::draw(sf::RenderTarget& target) const
{
    drawFast(target);
}

void CText_Element::drawFast(sf::RenderTarget& target) const
{
    if(visible)
        target.draw(text);
}
