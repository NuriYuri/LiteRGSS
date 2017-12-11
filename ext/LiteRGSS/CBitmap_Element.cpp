#include "CBitmap_Element.h"


CBitmap_Element::CBitmap_Element()
{
    text = new sf::Texture();
    render = nullptr;
}

CBitmap_Element::~CBitmap_Element()
{
    if(render != nullptr) /* Texture owned by render */
    {
        delete render;
        render = nullptr;
        text = nullptr;
    }
    else /* No render => generated texture */
    {
        if(text != nullptr)
        {
            delete text;
            text = nullptr;
        }
    }
}

sf::Texture* CBitmap_Element::getTexture()
{
    return text;
}

sf::RenderTexture* CBitmap_Element::getRender()
{
    if(render == nullptr)
    {
        create_render();
    }
    return render;
}

void CBitmap_Element::copy(CBitmap_Element* original) {
    sf::Image img = original->getTexture()->copyToImage();
    text->loadFromImage(img);
}

void CBitmap_Element::create_render()
{
    if(text != nullptr)
    {
        render = new sf::RenderTexture();
        sf::Vector2u sz = text->getSize();
        sf::Sprite sp(*text);
        render->create(sz.x, sz.y);
        render->clear(sf::Color(0, 0, 0, 0));
        render->draw(sp);
        render->display();
        delete text;
        const sf::Texture* subtext = &render->getTexture();
        text = const_cast<sf::Texture*>(subtext); /* The only operation on the texture should be copyToImage which is const */
    }
}