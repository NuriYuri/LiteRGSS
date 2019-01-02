#ifndef CRECT_ELEMENT_H
#define CRECT_ELEMENT_H
#include <SFML/Graphics.hpp>
#include "CDrawable_Element.h"
class CRect_Element {
    private:
    sf::IntRect rect;
    CDrawable_Element* target;
    public:
    sf::IntRect& getRect() { return rect; };
    void setElement(CDrawable_Element* _target) {target = _target;};
    CDrawable_Element* getElement() { return target; };
};

#endif