#ifndef CShape_Element_H
#define CShape_Element_H
#include <memory>
#include "ruby.h"
#include "CDrawable_Element.h"

class CShape_Element : public CDrawable_Element {
    protected:
        std::unique_ptr<sf::Shape> shape;
        bool visible;
		sf::RenderStates* render_states;
    public:
		CShape_Element();
		~CShape_Element();
        void draw(sf::RenderTarget& target) const override;
        void drawFast(sf::RenderTarget& target) const override;
        bool isViewport() const override { return false; };
        bool isPureSprite() const override { return false; };
        bool isShape() const override { return true; };
        sf::Shape* getShape() { return shape.get(); }
        template <class ShapeC, class ... Args>
		void setShape(Args&& ... args) {
            shape = std::make_unique<ShapeC>(std::forward<Args>(args)...);
        }
        void setVisible(bool value);
        bool getVisible();
		void setRenderState(sf::RenderStates* states);
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
		VALUE rShapeType;
		VALUE rRenderStates;
		VALUE rColor;
		VALUE rOutlineColor;
		VALUE rOutlineThickness;
};
#endif