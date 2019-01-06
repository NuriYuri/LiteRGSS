#include "CBitmap_Element.h"


CBitmap_Element::CBitmap_Element()
{
    text = new sf::Texture();
    //img = nullptr;
}

CBitmap_Element::~CBitmap_Element()
{
    if(text != nullptr)
    {
        delete text;
        text = nullptr;
    }
    /*if(img != nullptr)
    {
        delete img;
        img = nullptr;
    }*/
}

sf::Texture* CBitmap_Element::getTexture()
{
    return text;
}

void CBitmap_Element::copy(CBitmap_Element* original) {
    sf::Image img = original->getTexture()->copyToImage();
    text->loadFromImage(img);
}

sf::Image* CBitmap_Element::getImage()
{
    /*if(img == nullptr)
    {
        if(text->getSize().x > 0)
        {

        }
        else
            img = new sf::Image();
    }*/
    if(!has_image())
    {
        img = text->copyToImage();
    }
    return &img;
}