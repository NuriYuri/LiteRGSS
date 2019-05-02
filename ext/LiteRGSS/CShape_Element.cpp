#include "CViewport_Element.h"
#include "CShape_Element.h"

void CShape_Element::draw(sf::RenderTarget& target) const
{
	if (render_states != nullptr)
	{
		if (visible && shape != nullptr)
			target.draw(*shape, *render_states);
	}
	else
		CShape_Element::drawFast(target);
}

void CShape_Element::drawFast(sf::RenderTarget& target) const
{
	if(visible && shape != nullptr)
	{
		target.draw(*shape);
	}
}

void CShape_Element::setVisible(bool value)
{
	visible = value;
}

bool CShape_Element::getVisible()
{
	return visible;
}

void CShape_Element::setRenderState(sf::RenderStates * states)
{
	render_states = states;
}
