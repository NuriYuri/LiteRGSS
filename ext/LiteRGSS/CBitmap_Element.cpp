#include "CBitmap_Element.h"


CBitmap_Element::CBitmap_Element()
{
    text = new sf::Texture();
}

CBitmap_Element::~CBitmap_Element()
{
    if(text != nullptr)
    {
        delete text;
        text = nullptr;
    }
}

sf::Texture* CBitmap_Element::getTexture()
{
    return text;
}

void CBitmap_Element::copy(CBitmap_Element* original) {
    sf::Image img = original->getTexture()->copyToImage();
    text->loadFromImage(img);
}