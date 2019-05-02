#ifndef CDrawableStack_H
#define CDrawableStack_H
#include "CGraphicsStack.h"

class CDrawableStack : public CGraphicsStack {
public:
	using CGraphicsStack::CGraphicsStack;
	void draw(sf::View& defview, sf::RenderTarget& target) const;
	void drawFast(sf::RenderTarget& target) const;
	void updateContentsOpacity(long opacity);
};
#endif