#ifndef CSprite_Element_H
#define CSprite_Element_H
#include "CDrawable_Element.h"
class CSprite_Element : public CDrawable_Element {
    private:
    sf::Sprite sprite;
    bool drawable;
    bool visible;
    public:
    CSprite_Element() : CDrawable_Element(), drawable(false), visible(true) {};
    void draw(sf::RenderTarget& target) const override;
    bool isViewport() const override { return false; };
    sf::Sprite* getSprite() { return &sprite;};
    void setDrawable(bool value) { drawable = value;};
    void setVisible(bool value) { visible = value;};
    bool getVisible() { return visible; };
};
#endif