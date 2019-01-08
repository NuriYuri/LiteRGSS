#ifndef CShape_Element_H
#define CShape_Element_H
#include <memory>
#include "ruby.h"
#include "CDrawable_Element.h"

class CShape_Element : public CDrawable_Element {
protected:
    std::unique_ptr<sf::Shape> shape;
    bool visible = true;
	sf::RenderStates* render_states = nullptr;
public:
	CShape_Element() = default;
	virtual ~CShape_Element();
        
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
    VALUE rBitmap = Qnil;
    VALUE rX = Qnil;
    VALUE rY = Qnil;
    VALUE rZ = Qnil;
    VALUE rOX = Qnil;
    VALUE rOY = Qnil;
    VALUE rAngle = Qnil;
    VALUE rZoomX = Qnil;
    VALUE rZoomY = Qnil;
    VALUE rRect = Qnil;
    VALUE rShapeType = Qnil;
    VALUE rRenderStates = Qnil;
    VALUE rColor = Qnil;
    VALUE rOutlineColor = Qnil;
    VALUE rOutlineThickness = Qnil;
};

namespace meta {
    template<>
    struct Log<CShape_Element> {
        static constexpr auto classname = "Shape";
    };
}
#endif
