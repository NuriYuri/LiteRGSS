#ifndef CViewport_Element_H
#define CViewport_Element_H
#include <memory>
#include "CDrawable_Element.h"
#include "CSprite_Element.h"

class CViewport_Element : public CDrawable_Element {
    protected:
        long ox, oy;
        sf::View view;
        std::vector<CDrawable_Element*> stack;
        sf::Glsl::Vec4 tone;
        CTone_Element* linkedTone;
        /*sf::RenderTexture* render;
		sf::Shader* shader;*/
		std::unique_ptr<sf::Color> color_copy;
        bool visible;
		std::unique_ptr<sf::RenderStates> render_states;
        std::unique_ptr<sf::Shader> render_states_shader;
    public:
		CViewport_Element();
        virtual ~CViewport_Element();
        void draw(sf::RenderTarget& target) const override;
        void drawFast(sf::RenderTarget& target) const override;
		bool isViewport() const override;
		bool isPureSprite() const override;
		bool isShape() const override;
        sf::View* getView() {return &view;};
		void setRenderStates(std::unique_ptr<sf::RenderStates> states);
        long getOx() {return ox;};
        long getOy() {return oy;};
        void setOx(long nox) {ox = nox;};
        void setOy(long noy) {oy = noy;};
        void bind(CDrawable_Element& sprite);
        void clearStack();
        sf::Glsl::Vec4* getTone();
		void updatetone();
		void setLinkedTone(CTone_Element* _tone);
		CTone_Element* getLinkedTone();
        // void reset_render();
        void create_render();
		void setVisible(bool value);
        bool getVisible() { return visible; };
		const std::vector<CDrawable_Element*>& getStack() const;
        /* Ruby Ivar */
        VALUE rRect;
        VALUE rTone;
        VALUE rColor;
        VALUE rZ;
		VALUE rAngle;
		VALUE rZoom;
		VALUE rRenderState;
        /* Shader related stuff */
		static std::unique_ptr<sf::RenderTexture> render;
		static std::unique_ptr<sf::Sprite> render_sprite;
	private:
		sf::Color* check_up_color() const;
};
#endif