#ifndef CBITMAP_ELEMENT_H
#define CBITMAP_ELEMENT_H
#include <SFML/Graphics.hpp>

class CBitmap_Element {
    sf::Texture text;
    sf::Image img;
    public:
    sf::Texture* getTexture();
    sf::Image* getImage();
    void copy(CBitmap_Element* original);
};

#endif