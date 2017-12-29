#ifndef CDrawable_Element_H
#define CDrawable_Element_H
#include "ruby.h" // Windows Build Patch
#include <SFML/Graphics.hpp>
#include "sf_Text2.hpp"
#include <vector>
#include <algorithm>

class CRect_Element;
class CTone_Element;

class CDrawable_Element {
    protected:
        std::vector<CDrawable_Element*> *origin_stack;
        CRect_Element* linkedRect;
    public:
        CDrawable_Element() {origin_stack = nullptr; linkedRect = nullptr;}
        void setOriginStack(std::vector<CDrawable_Element*> *o);
        void overrideOrigineStack(std::vector<CDrawable_Element*> *o) {origin_stack = o;};
        virtual void draw(sf::RenderTarget& target) const = 0;
        virtual void drawFast(sf::RenderTarget& target) const = 0;
        virtual bool isViewport() const = 0;
        virtual bool isPureSprite() const = 0;
        void setLinkedRect(CRect_Element* _rect) { linkedRect = _rect; };
        CRect_Element* getLinkedRect() { return linkedRect; };
};
#endif