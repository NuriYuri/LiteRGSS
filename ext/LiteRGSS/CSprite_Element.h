#ifndef CSprite_Element_H
#define CSprite_Element_H
#include "ruby.h"
#include "CDrawable_Element.h"

class CSprite_Element : public CDrawable_Element {
    protected:
        sf::Sprite sprite;
        bool drawable;
        bool visible;
    public:
        CSprite_Element() : CDrawable_Element(), drawable(false), visible(true) {};
        virtual ~CSprite_Element() = default;
        
        void draw(sf::RenderTarget& target) const override;
        void drawFast(sf::RenderTarget& target) const override;
        bool isViewport() const override { return false; };
        bool isPureSprite() const override { return true; };
		bool isShape() const override { return false; };
        sf::Sprite* getSprite() { return &sprite;};
        void setDrawable(bool value) { drawable = value;};
        bool getDrawable() { return drawable; };
        void setVisible(bool value) { visible = value;};
        bool getVisible() { return visible; };
        /* Instance variable for Ruby */
        VALUE rViewport;
        VALUE rBitmap;
        VALUE rX;
        VALUE rY;
        VALUE rZ;
        VALUE rOX;
        VALUE rOY;
        VALUE rAngle;
        VALUE rZoomX;
        VALUE rZoomY;
        VALUE rRect;
		VALUE rMirror;
};
#endif