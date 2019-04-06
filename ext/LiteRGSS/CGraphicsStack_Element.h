#ifndef CGraphicsStack_Element_H
#define CGraphicsStack_Element_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "ruby.h"

class CDrawable_Element;
class CGraphicsStack_Element {
public:
    CGraphicsStack_Element(VALUE& self);
    virtual ~CGraphicsStack_Element();

    void detachSprites();
    void bind(CDrawable_Element& drawable);
    void draw(sf::View& defview, sf::RenderTarget& target) const;
    void drawFast(sf::RenderTarget& target) const;
    void updateContentsOpacity(long opacity);
private:
    void clearStack();
    bool isSelfValid() const {
        return self_ != Qnil;
    }

    VALUE self_ = Qnil;
    std::vector<CDrawable_Element*> stack;
};
#endif