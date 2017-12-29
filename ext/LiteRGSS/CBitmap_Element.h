#ifndef CBITMAP_ELEMENT_H
#define CBITMAP_ELEMENT_H
#include <SFML/Graphics.hpp>

class CBitmap_Element {
    sf::Texture* text;
    sf::Image img;
    public:
        CBitmap_Element();
        ~CBitmap_Element();
        sf::Texture* getTexture();
        sf::Image* getImage();
        void copy(CBitmap_Element* original);
        bool has_image() { return img.getSize().x != 0; }; //return img != nullptr; };
};

#endif