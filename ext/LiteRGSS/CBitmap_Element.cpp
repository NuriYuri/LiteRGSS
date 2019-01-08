#include <iostream>
#include "CBitmap_Element.h"

sf::Texture& CBitmap_Element::getTexture() {
    return text;
}

sf::Image& CBitmap_Element::getImage() {
    if(!has_image()) {
        img = text.copyToImage();
    }
    return img;
}

CBitmap_Element::~CBitmap_Element() {
    //std::cout << "Entering Bitmap destructor" << std::endl;
}