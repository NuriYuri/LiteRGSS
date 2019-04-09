#ifndef CGraphicsStack_Element_H
#define CGraphicsStack_Element_H

#include <SFML/Graphics.hpp>
#include "utils/container/vector_tracker.h"
#include "ruby.h"

class CDrawable_Element;
class CRubyGraphicsStack;
class CGraphicsStack_Element {
    friend class CRubyGraphicsStack;
public:
    CGraphicsStack_Element() = default;
    ~CGraphicsStack_Element();

    void draw(sf::View& defview, sf::RenderTarget& target) const;
    void drawFast(sf::RenderTarget& target) const;
    void updateContentsOpacity(long opacity);

    void bind(CRubyGraphicsStack& rubyStack, CDrawable_Element& el);
    void clear();

private:
    vector_tracker<CDrawable_Element*> stack;
};
#endif