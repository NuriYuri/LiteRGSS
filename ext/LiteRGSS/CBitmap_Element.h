#ifndef CBITMAP_ELEMENT_H
#define CBITMAP_ELEMENT_H
#include <SFML/Graphics.hpp>

class CBitmap_Element {
    sf::Texture* text;
    sf::RenderTexture* render;
    public:
        CBitmap_Element();
        ~CBitmap_Element();
        sf::Texture* getTexture();
        sf::RenderTexture* getRender();
        void copy(CBitmap_Element* original);
    private:
        void create_render();
};

#endif