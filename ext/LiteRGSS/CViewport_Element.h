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
    public:
        CViewport_Element() : CDrawable_Element() { render = nullptr;};
        ~CViewport_Element() { if(render != nullptr) delete render;};
        void draw(sf::RenderTarget& target) const override;
        void drawFast(sf::RenderTarget& target) const override;
        bool isViewport() const override { return true; };
        bool isPureSprite() const override { return false; };
        sf::View* getView() {return &view;};
        long getOx() {return ox;};
        long getOy() {return oy;};
        void setOx(long nox) {ox = nox;};
        void setOy(long noy) {oy = noy;};
        void bindSprite(CSprite_Element* sprite);
        void clearStack();
        sf::Glsl::Vec4* getTone() {return &tone;};
        void setLinkedTone(CTone_Element* _tone) {linkedTone = _tone;};
        CTone_Element* getLinkedTone() { return linkedTone;};
        void reset_render();
        void create_render();
        /* Ruby Ivar */
        VALUE rRect;
        VALUE rTone;
        /* Shader related stuff */
        static sf::Shader* globalshader;
        static void load_globalshader();
        static void reset_globalshader();
};
#endif