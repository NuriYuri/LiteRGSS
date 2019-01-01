#ifndef CBITMAP_ELEMENT_H
#define CBITMAP_ELEMENT_H
#include <SFML/Graphics.hpp>
#include "utils/metadata.h"

class CBitmap_Element {
    sf::Texture text;
    sf::Image img;
    public:
        CBitmap_Element() = default;
        ~CBitmap_Element() = default;
        sf::Texture& getTexture();
        sf::Image& getImage();
        bool has_image() { return img.getSize().x != 0; };
};

namespace meta {
    template<>
    struct Log<CBitmap_Element> {
        static constexpr auto classname = "Bitmap";
    };
}

#endif