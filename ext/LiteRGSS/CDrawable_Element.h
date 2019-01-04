#ifndef CDrawable_Element_H
#define CDrawable_Element_H
#include "ruby.h" // Windows Build Patch
#include <SFML/Graphics.hpp>
#include "sf_Text2.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

class CRect_Element;
class CTone_Element;

class CDrawable_Element {
    protected:
        std::vector<CDrawable_Element*> *origin_stack;
        CRect_Element* linkedRect = nullptr;
        unsigned long index;
    public:
        CDrawable_Element();
        virtual ~CDrawable_Element() = default;
        void setOriginStack(std::vector<CDrawable_Element*> *o);
        void overrideOrigineStack(std::vector<CDrawable_Element*> *o) {origin_stack = o;};
        void setIndex(unsigned long nindex);
        unsigned long getIndex();
        virtual void draw(sf::RenderTarget& target) const = 0;
        virtual void drawFast(sf::RenderTarget& target) const = 0;
        virtual bool isViewport() const = 0;
        virtual bool isPureSprite() const = 0;
		virtual bool isShape() const = 0;
        void setLinkedRect(CRect_Element* _rect) { linkedRect = _rect; };
        CRect_Element* getLinkedRect() const { return linkedRect; };
};
#endif