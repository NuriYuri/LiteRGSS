#ifndef CWindow_Element_H
#define CWindow_Element_H
#include "ruby.h"
#include "CView_Element.h"

class CWindow_Element : public CView_Element {
	private:
		std::vector<sf::VertexArray> vertices;
		sf::Texture* texture = nullptr;
		sf::View view;
		bool visible = true;
		unsigned long num_vertices_line = 0;
		long counter = 0;
		sf::Sprite pause_sprite;
		sf::Sprite cursor_sprite;
		bool locked = false;
		void updateVerticesBlt(long wt, long ht);
		void updateVerticesStretch(long wt, long ht);
		void allocateVerticesBlt(long delta_w, long nb2, long delta_h, long nb4);
		void calculateVertices(long x, long y, long line, long cell, sf::Vector2i &a, sf::IntRect &rect);
		void rectSet(sf::IntRect &rect, long x, long y, long width, long height);
		void allocateVerticesStretch();
		void calculateVerticesStretch(long x, long y, long line, long cell, sf::Vector2i &s, sf::Vector2i &a, sf::IntRect &rect);
	public:
		CWindow_Element();
		virtual ~CWindow_Element() = default;
		void draw(sf::RenderTarget& target) const override;
		void drawFast(sf::RenderTarget& target) const override;
		void drawCalculateView(sf::RenderTarget& target, sf::View& targetView) const;
		bool isViewport() const override { return false; };
		bool isPureSprite() const override { return false; };
		bool isShape() const override { return false; };
		void setVisible(bool value) { visible = value;};
		bool getVisible() { return visible; };
		sf::VertexArray* getVertices();
		void setVertices(sf::VertexArray* _vertices);
		sf::Texture* getTexture();
		void setTexture(sf::Texture* text);
		void update();
		void updatePauseSprite();
		void updateCursorSprite();
		void updateVertices();
		void updateContents();
		void updateBackOpacity();
		void updateContentsOpacity();
		void updateView();
		void lock();
		void unlock();
		bool is_locked();
		sf::Sprite& getPauseSprite();
		sf::Sprite& getCursorSprite();
		void resetPausePosition();
		void resetCursorPosition(sf::IntRect* rect);

		/* Instance variable for Ruby */
		VALUE rBitmap = Qnil;
		VALUE rX = Qnil;
		VALUE rY = Qnil;
		VALUE rZ = Qnil;
		VALUE rOX = Qnil;
		VALUE rOY = Qnil;
		VALUE rRect = Qnil;
		VALUE rWidth = Qnil;
		VALUE rHeight = Qnil;
		VALUE rCursorRect = Qnil;
		VALUE rBackOpacity = Qnil;
		VALUE rContentOpacity = Qnil;
		VALUE rOpacity = Qnil;
		VALUE rWindowBuilder = Qnil;
		VALUE rPauseSkin = Qnil;
		VALUE rCursorSkin = Qnil;
		VALUE rPause = Qnil;
		VALUE rPauseX = Qnil;
		VALUE rPauseY = Qnil;
		VALUE rActive = Qnil;
		VALUE rStretch = Qnil;
};

void Dispose_AllSprite(VALUE table);

namespace meta {
	template<>
	struct Log<CWindow_Element> {
		static constexpr auto classname = "Window";
	};
}
#endif