#ifndef CBITMAP_ELEMENT_H
#define CBITMAP_ELEMENT_H
#include <SFML/Graphics.hpp>

class CBitmap_Element {
    sf::Texture* text;
    public:
        CBitmap_Element();
        ~CBitmap_Element();
        sf::Texture* getTexture();
        void copy(CBitmap_Element* original);
};

#endif