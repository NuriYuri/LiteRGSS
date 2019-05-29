#ifndef CSpriteMap_Element_H
#define CSpriteMap_Element_H
#include "ruby.h"
#include "CDrawable_Element.h"
#include "rbAdapter.h"

#define SPRITE_MAP_USE_VERTEX

class CSpriteMap_Element : public CDrawable_Element {
protected:
#ifndef SPRITE_MAP_USE_VERTEX
	sf::Sprite *sprites;
#else
	sf::Vertex *vertices;
	sf::VertexBuffer *buffers;
	sf::Texture **textures;
	sf::Transformable *transformable;
	void generate_vertices();
#endif
	bool *active_sprites;
	bool drawable = false;
	bool visible = true;
	unsigned long tile_width = 0;
	unsigned long tile_count = 0;
	void dispose_stack();
public:
	CSpriteMap_Element() = default;
	CSpriteMap_Element(CSpriteMap_Element&&) = default;
	CSpriteMap_Element(const CSpriteMap_Element&) = delete;
	CSpriteMap_Element& operator=(const CSpriteMap_Element&) = delete;
	CSpriteMap_Element& operator=(CSpriteMap_Element&&) = default;
	virtual ~CSpriteMap_Element();

	void draw(sf::RenderTarget& target) const override;
	void drawFast(sf::RenderTarget& target) const override;
	bool isViewport() const override { return false; };
	bool isPureSprite() const override { return true; };
	bool isShape() const override { return false; };
	void setVisible(bool value) { visible = value;};
	bool getVisible() { return visible; };
	void define_map(unsigned long tileWidth, unsigned long tileCount);
	void reset();
	void setTile(long index, sf::IntRect& tile, sf::Texture& texture);
	void setTileRect(long index, sf::IntRect& tile);
	void setPosition(float x, float y);
	void setOrigin(float ox, float oy);
	void setTileScale(float scale);

	/* Instance variable for Ruby */
	VALUE rX = LONG2FIX(0);
	VALUE rY = LONG2FIX(0);
	VALUE rZ = LONG2FIX(0);
	VALUE rOX = LONG2FIX(0);
	VALUE rOY = LONG2FIX(0);
	VALUE rScale = LONG2FIX(1);
};


namespace meta {
	template<>
	struct Log<CSpriteMap_Element> {
		static constexpr auto classname = "SpriteMap";
	};
}

#endif
