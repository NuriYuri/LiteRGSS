#ifndef CText_Element_H
#define CText_Element_H
#include "ruby.h"
#include "CDrawable_Element.h"
#include "sf_Text2.hpp"
#include "metadata.h"

class CText_Element : public CDrawable_Element {
	private:
		sf::Text2 text;
		bool visible = true;
	public:
		CText_Element() : 
			rAlign(LONG2FIX(0)), 
			rZ(LONG2FIX(0)){
		}
		virtual ~CText_Element() = default;
		void draw(sf::RenderTarget& target) const override;
		void drawFast(sf::RenderTarget& target) const override;
		bool isViewport() const override { return false; };
		bool isPureSprite() const override { return false; };
		bool isShape() const override { return false; };
		sf::Text2& getText() { return text;};
		void setVisible(bool value) { visible = value;};
		bool getVisible() { return visible; };
		
		/* Instance variable for Ruby */
		VALUE rX = Qnil;
		VALUE rY = Qnil;
		VALUE rZ = Qnil;
		VALUE rwidth = Qnil;
		VALUE rheight = Qnil;
		VALUE rtext = Qnil;
		VALUE rAlign = Qnil;
};
namespace meta {
	template<>
	struct Log<CText_Element> {
		static constexpr auto classname = "Text";
	};
}
#endif
