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
    private:
        void resetOriginStack();
        void setOriginStack(std::vector<CDrawable_Element*> *o);
    protected:
        std::vector<CDrawable_Element*> *origin_stack = nullptr;
        CRect_Element* linkedRect = nullptr;
        unsigned long index = 0;
        bool disposeFromViewport_ = false;
    public:
        CDrawable_Element() {};
        virtual ~CDrawable_Element();
        void setOriginStack(std::vector<CDrawable_Element*>& o);
        void overrideOrigineStack() {origin_stack = nullptr;};
        unsigned long getIndex();
        virtual void draw(sf::RenderTarget& target) const = 0;
        virtual void drawFast(sf::RenderTarget& target) const = 0;
        virtual bool isViewport() const = 0;
        virtual bool isPureSprite() const = 0;
		virtual bool isShape() const = 0;
        void disposeFromViewport() { RDATA(self)->data = nullptr; disposeFromViewport_ = true;}
        void bindRect(CRect_Element* rect);
        CRect_Element* getRect() const { return linkedRect; };

        /* Instance variable for Ruby */
        VALUE rViewport = Qnil;
        VALUE self = Qnil;
};
#endif