#ifndef CGraphicsStack_Element_H
#define CGraphicsStack_Element_H
#include <memory>
#include <SFML/Graphics.hpp>
#include "utils/container/vector_tracker.h"
#include "ruby.h"

class CDrawable_Element;
class CRubyGraphicsStack;
class CGraphicsStack_Element {
    friend class CRubyGraphicsStack;
public:
    CGraphicsStack_Element(std::unique_ptr<CRubyGraphicsStack> rubyStack);
    ~CGraphicsStack_Element();

    void draw(sf::View& defview, sf::RenderTarget& target) const;
    void drawFast(sf::RenderTarget& target) const;
    void updateContentsOpacity(long opacity);

    void bind(CDrawable_Element& el);
    void clear();
    
    void syncStackCppFromRuby();
private:
    void detach();
    void clearRuby();

    vector_tracker<CDrawable_Element*> stack;
    std::unique_ptr<CRubyGraphicsStack> rubyStack;
};
#endif