#include "CBitmap_Element.h"

sf::Texture* CBitmap_Element::getTexture()
{
    return &text;
}

sf::Image* CBitmap_Element::getImage()
{
    return &img;
}

void CBitmap_Element::copy(CBitmap_Element* original) {
    img = original->getTexture()->copyToImage();
    text.loadFromImage(img);
}
