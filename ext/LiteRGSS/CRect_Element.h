#ifndef CRECT_ELEMENT_H
#define CRECT_ELEMENT_H
#include <iostream>
#include <SFML/Graphics.hpp>
#include "CDrawable_Element.h"
#include "utils/metadata.h"

class CRect_Element {
private:
    sf::IntRect rect;
    CDrawable_Element* target = nullptr;
public:
    CRect_Element() = default;
    ~CRect_Element() {
        if(target != nullptr) {
            target->setLinkedRect(nullptr);
        }
    }
    
    sf::IntRect& getRect() { return rect; };
    void setElement(CDrawable_Element* _target) {target = _target;};
    CDrawable_Element* getElement() { return target; };
};

namespace meta {
    template<>
    struct Log<CRect_Element> {
        static constexpr auto classname = "Rect";
    };
}


#endif