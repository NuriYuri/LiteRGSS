#include "CShaderSprite_Element.h"

CShaderSprite_Element::CShaderSprite_Element() {
	getSprite().setColor(sf::Color(255, 255, 255, 255));
    //setLinkedRect(nullptr);
	rRenderStates = Qnil;
}

void CShaderSprite_Element::draw(sf::RenderTarget& target) const
{
    if(drawable && visible)
    {
		if (render_states == nullptr)
			target.draw(sprite);
		else
			target.draw(sprite, *render_states);//, render_states->shader);
    }
}

void CShaderSprite_Element::drawFast(sf::RenderTarget& target) const
{
	draw(target);
}

bool CShaderSprite_Element::isShape() const
{
	return false;
}

void CShaderSprite_Element::setRenderState(sf::RenderStates * states)
{
	render_states = states;
}

CShaderSprite_Element::~CShaderSprite_Element() {
	std::cout << "Entering ShaderSprite destructor" << std::endl;
}