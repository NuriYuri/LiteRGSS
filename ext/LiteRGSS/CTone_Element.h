#ifndef CTONE_ELEMENT_H
#define CTONE_ELEMENT_H
#include <SFML/Graphics.hpp>
#include "CViewport_Element.h"

class CTone_Element {
    private:
        sf::Glsl::Vec4 tonevalues;
        CViewport_Element* target = nullptr;
    public:
        CTone_Element() = default;
        ~CTone_Element() { 
            if(target != nullptr) {
                target->setLinkedRect(nullptr);
            }
        }
        sf::Glsl::Vec4* getTone() { return &tonevalues; };
        void setElement(CViewport_Element* element) {target = element;};
        CViewport_Element* getElement() { return target;};
};

namespace meta {
    template<>
    struct Log<CTone_Element> {
        static constexpr auto classname = "Tone";
    };
}
#endif