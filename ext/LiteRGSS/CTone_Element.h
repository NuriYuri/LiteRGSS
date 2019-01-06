#ifndef CTONE_ELEMENT_H
#define CTONE_ELEMENT_H
#include <SFML/Graphics.hpp>
#include "CViewport_Element.h"

class CTone_Element {
    private:
        sf::Glsl::Vec4 tonevalues;
        CViewport_Element* target;
    public:
        sf::Glsl::Vec4* getTone() { return &tonevalues; };
        void setElement(CViewport_Element* element) {target = element;};
        CViewport_Element* getElement() { return target;};
};


#endif