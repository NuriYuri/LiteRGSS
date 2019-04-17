#ifndef CSprite_Element_H
#define CSprite_Element_H
#include "ruby.h"
#include "CDrawable_Element.h"
#include "rbAdapter.h"

class CSprite_Element : public CDrawable_Element {
protected:
    sf::Sprite sprite;
    bool drawable = false;
    bool visible = true;
public:
    CSprite_Element() = default;
    CSprite_Element(CSprite_Element&&) = default;
    CSprite_Element(const CSprite_Element&) = delete;
    CSprite_Element& operator=(const CSprite_Element&) = delete;
    CSprite_Element& operator=(CSprite_Element&&) = default;
    virtual ~CSprite_Element();
    
    void draw(sf::RenderTarget& target) const override;
    void drawFast(sf::RenderTarget& target) const override;
    bool isViewport() const override { return false; };
    bool isPureSprite() const override { return true; };
    bool isShape() const override { return false; };
    sf::Sprite& getSprite() { return sprite;};
    void setDrawable(bool value) { drawable = value;};
    bool getDrawable() { return drawable; };
    void setVisible(bool value) { visible = value;};
    bool getVisible() { return visible; };
    
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
    VALUE rMirror = Qnil;
};
namespace meta {
    template<>
    struct Log<CSprite_Element> {
        static constexpr auto classname = "Sprite";
    };
}
#endif
