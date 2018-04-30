#ifndef CViewport_Element_H
#define CViewport_Element_H
#include "CDrawable_Element.h"
#include "CSprite_Element.h"

class CViewport_Element : public CDrawable_Element {
    private:
        long ox, oy;
        sf::View view;
        std::vector<CDrawable_Element*> stack;
        sf::Glsl::Vec4 tone;
        CTone_Element* linkedTone;
        sf::RenderTexture* render;
		sf::Shader* shader;
		sf::Color* color_copy;
        bool visible;
    public:
		CViewport_Element();// : CDrawable_Element() { render = nullptr; linkedTone = nullptr; };
        ~CViewport_Element();
        void draw(sf::RenderTarget& target) const override;
        void drawFast(sf::RenderTarget& target) const override;
        bool isViewport() const override { return true; };
        bool isPureSprite() const override { return false; };
        sf::View* getView() {return &view;};
        long getOx() {return ox;};
        long getOy() {return oy;};
        void setOx(long nox) {ox = nox;};
        void setOy(long noy) {oy = noy;};
        void bind(CDrawable_Element* sprite);
        void clearStack();
        sf::Glsl::Vec4* getTone();
		void updatetone();
        void setLinkedTone(CTone_Element* _tone) {linkedTone = _tone;};
        CTone_Element* getLinkedTone() { return linkedTone;};
        void reset_render();
        void create_render();
        void setVisible(bool value) { visible = value;};
        bool getVisible() { return visible; };
        const std::vector<CDrawable_Element*>* getStack() { return &stack; };
        /* Ruby Ivar */
        VALUE rRect;
        VALUE rTone;
        VALUE rColor;
        VALUE rZ;
        /* Shader related stuff */
        static unsigned long render_count;
	private:
		sf::Color* check_up_color() const;
};
#endif