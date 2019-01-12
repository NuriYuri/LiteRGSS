#ifndef CViewport_Element_H
#define CViewport_Element_H
#include <memory>
#include "CView_Element.h"
#include "CSprite_Element.h"
#include "utils/rbAdapter.h"

class CViewport_Element : public CView_Element {
    private:
        long ox = 0;
        long oy = 0;
        sf::View view;
        sf::Glsl::Vec4 tone;
        CTone_Element* linkedTone = nullptr;
	std::unique_ptr<sf::Color> color_copy;
        bool visible = true;
        sf::RenderStates* render_states = nullptr;
        mutable sf::Shader* render_states_shader = nullptr;
	std::unique_ptr<sf::RenderStates> default_render_states;
        mutable std::unique_ptr<sf::Shader> default_render_states_shader;
    public:
	CViewport_Element() = default;
        virtual ~CViewport_Element();
        void draw(sf::RenderTarget& target) const override;
	bool isViewport() const override;
	bool isPureSprite() const override;
	bool isShape() const override;
        sf::View& getView() {return view;};
	void setRenderStates(sf::RenderStates* states);
        long getOx() {return ox;};
        long getOy() {return oy;};
        void setOx(long nox) {ox = nox;};
        void setOy(long noy) {oy = noy;};
        sf::Glsl::Vec4* getTone();
	void updatetone();
        void bindTone(CTone_Element* tone);
	CTone_Element* getBoundTone();
        // void reset_render();
        void create_render();
	void setVisible(bool value);
        bool getVisible() { return visible; };
        /* Ruby Ivar */
        VALUE rRect = Qnil;
        VALUE rTone = Qnil;
        VALUE rColor = Qnil;
        VALUE rZ = Qnil;
	VALUE rAngle = Qnil;
	VALUE rZoom = Qnil;
	VALUE rRenderState = Qnil;
        /* Shader related stuff */
	static std::unique_ptr<sf::RenderTexture> render;
	static std::unique_ptr<sf::Sprite> render_sprite;
private:
	sf::Color* check_up_color() const;
        sf::RenderStates* getRenderState();
        sf::Shader* getRenderStateShader() const;
};
namespace meta {
    template<>
    struct Log<CViewport_Element> {
        static constexpr auto classname = "Viewport";
    };
}
#endif
