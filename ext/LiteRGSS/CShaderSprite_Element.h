#ifndef CShaderSprite_Element_H
#define CShaderSprite_Element_H
#include "CSprite_Element.h"

class CShaderSprite_Element : public CSprite_Element {
protected:
	sf::RenderStates* render_states = nullptr;
public:
	CShaderSprite_Element();
	virtual ~CShaderSprite_Element() = default;

	void draw(sf::RenderTarget& target) const override;
	void drawFast(sf::RenderTarget& target) const override;
	bool isViewport() const override { return false; };
	bool isPureSprite() const override { return true; };
	bool isShape() const override;
	void setRenderState(sf::RenderStates* states);

	VALUE rRenderStates = Qnil;
};

namespace meta {
    template<>
    struct Log<CShaderSprite_Element> {
        static constexpr auto classname = "ShaderedSprite";
    };
}
#endif
