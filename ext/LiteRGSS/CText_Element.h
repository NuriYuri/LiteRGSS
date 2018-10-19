#ifndef CText_Element_H
#define CText_Element_H
#include "ruby.h"
#include "CDrawable_Element.h"

class CText_Element : public CDrawable_Element {
    private:
        sf::Text2 text;
        bool visible;
    public:
        CText_Element() : CDrawable_Element(), visible(true) {};
        void draw(sf::RenderTarget& target) const override;
        void drawFast(sf::RenderTarget& target) const override;
        bool isViewport() const override { return false; };
        bool isPureSprite() const override { return false; };
		bool isShape() const override { return false; };
        sf::Text2* getText() { return &text;};
        void setVisible(bool value) { visible = value;};
        bool getVisible() { return visible; };
        /* Instance variable for Ruby */
        VALUE rViewport;
        VALUE rX;
        VALUE rY;
        VALUE rZ;
        VALUE rwidth;
        VALUE rheight;
        VALUE rtext;
        VALUE rAlign;
};
#endif