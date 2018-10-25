#ifndef CWindow_Element_H
#define CWindow_Element_H
#include "ruby.h"
#include "CDrawable_Element.h"

class CWindow_Element : public CDrawable_Element {
    protected:
		sf::VertexArray* vertices;
		sf::Texture* texture;
        bool visible;
		unsigned long num_vertices_line;
		sf::Sprite pause_sprite;
		sf::Sprite cursor_sprite;
		void updateVerticesBlt(long wt, long ht);
		void allocateVerticesBlt(long delta_w, long nb2, long delta_h, long nb4);
		void calculateVertices(long x, long y, long line, long cell, sf::Vector2i &a, sf::IntRect &rect);
		void rectSet(sf::IntRect &rect, long x, long y, long width, long height);
    public:
		CWindow_Element();
		~CWindow_Element();
        void draw(sf::RenderTarget& target) const override;
        void drawFast(sf::RenderTarget& target) const override;
        bool isViewport() const override { return false; };
        bool isPureSprite() const override { return false; };
		bool isShape() const override { return false; };
        void setVisible(bool value) { visible = value;};
        bool getVisible() { return visible; };
		sf::VertexArray* getVertices();
		void setVertices(sf::VertexArray* _vertices);
		sf::Texture* getTexture();
		void setTexture(sf::Texture* text);
		void updateVertices();
		void updateContents();
        /* Instance variable for Ruby */
        VALUE rViewport;
        VALUE rBitmap;
        VALUE rX;
        VALUE rY;
        VALUE rZ;
        VALUE rOX;
        VALUE rOY;
		VALUE rWidth;
		VALUE rHeight;
		VALUE rCursorRect;
		VALUE rBackOpacity;
		VALUE rContentOpacity;
		VALUE rOpacity;
		VALUE rWindowBuilder;
		VALUE rPauseSkin;
		VALUE rCursorSkin;
		VALUE rPause;
		VALUE rPauseX;
		VALUE rPauseY;
};
#endif