#ifndef CDrawable_Element_H
#define CDrawable_Element_H
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
class CDrawable_Element {
    protected:
    std::vector<CDrawable_Element*> *origin_stack;
    public:
    CDrawable_Element() {origin_stack = nullptr;}
    void setOriginStack(std::vector<CDrawable_Element*> *o);
    virtual void draw(sf::RenderTarget& target) const = 0;
    virtual bool isViewport() const = 0;
    virtual bool isPureSprite() const = 0;
};
#endif