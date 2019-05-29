#include "CViewport_Element.h"

void CSpriteMap_Element::draw(sf::RenderTarget& target) const 
{
	CSpriteMap_Element::drawFast(target);
}

void CSpriteMap_Element::drawFast(sf::RenderTarget& target) const 
{
	if(drawable && visible)
	{
		sf::RenderStates states = sf::RenderStates(transformable->getTransform());
		for(int i = 0; i < tile_count; i++)
		{
			if(active_sprites[i])
			{
#ifndef SPRITE_MAP_USE_VERTEX
				target.draw(sprites[i]);
#else
				states.texture = textures[i];
				if(buffers)
					target.draw(buffers[i], states);
				else
					target.draw(&(vertices[4 * i]),4, sf::PrimitiveType::TriangleStrip, states);
#endif

			}
		}
	}
}

void CSpriteMap_Element::define_map(unsigned long tileWidth, unsigned long tileCount)
{
	dispose_stack();
	if(tileCount == 0)
	{
		drawable = false;
		return;
	}
	tile_width = tileWidth;
	tile_count = tileCount;
#ifndef SPRITE_MAP_USE_VERTEX
	sprites = new sf::Sprite[tile_count];
#else
	generate_vertices();
#endif
	active_sprites = new bool[tile_count];
	drawable = true;
	reset();
}

#ifdef SPRITE_MAP_USE_VERTEX
void CSpriteMap_Element::generate_vertices()
{
	vertices = new sf::Vertex[tile_count * 4];
	textures = new sf::Texture*[tile_count];
	transformable = new sf::Transformable();
	if(sf::VertexBuffer::isAvailable())
		buffers = new sf::VertexBuffer[tile_count];
	float x = 0;
	float width = static_cast<float>(tile_width * 2);
	long sub_index;
	for(long i = 0;i < tile_count; i++)
	{
		if(buffers)
		{
			buffers[i].create(4);
			buffers[i].setPrimitiveType(sf::PrimitiveType::TriangleStrip);
		}
		sub_index = i << 2;
		vertices[sub_index].position = sf::Vector2f(x, 0);
		vertices[sub_index + 1].position = sf::Vector2f(x, width);
		x += width;
		vertices[sub_index + 2].position = sf::Vector2f(x, 0);
		vertices[sub_index + 3].position = sf::Vector2f(x, width);
	}
}
#endif

void CSpriteMap_Element::setTile(long index, sf::IntRect& tile, sf::Texture& texture)
{
	if(index < 0 || index >= tile_count)
		return;
#ifndef SPRITE_MAP_USE_VERTEX
	sprites[index].setTexture(texture);
	sprites[index].setTextureRect(tile);
#else
	textures[index] = &texture;
	setTileRect(index, tile);
#endif
	active_sprites[index] = true;
}

void CSpriteMap_Element::setTileRect(long index, sf::IntRect& tile)
{
	if(index < 0 || index >= tile_count)
		return;
#ifndef SPRITE_MAP_USE_VERTEX
	sprites[index].setTextureRect(tile);
#else
	float left   = static_cast<float>(tile.left);
    float right  = left + tile.width;
    float top    = static_cast<float>(tile.top);
    float bottom = top + tile.height;
	long sub_index = index << 2;
	vertices[sub_index].texCoords = sf::Vector2f(left, top);
	vertices[sub_index + 1].texCoords = sf::Vector2f(left, bottom);
	vertices[sub_index + 2].texCoords = sf::Vector2f(right, top);
	vertices[sub_index + 3].texCoords = sf::Vector2f(right, bottom);
	if(buffers)
		buffers[index].update(&vertices[sub_index]);
#endif
}

void CSpriteMap_Element::reset()
{
	for(long i = 0; i < tile_count; i++)
		active_sprites[i] = false;
}

void CSpriteMap_Element::dispose_stack()
{
#ifndef SPRITE_MAP_USE_VERTEX
	if(sprites != nullptr)
		delete[] sprites;
	sprites = nullptr;
#else
	if(vertices != nullptr)
		delete[] vertices;
	if(buffers != nullptr)
		delete[] buffers;
	if(textures)
		delete[] textures;
	if(transformable)
		delete transformable;
	vertices = nullptr;
	buffers = nullptr;
	textures = nullptr;
	transformable = nullptr;
#endif
	if(active_sprites != nullptr)
		delete[] active_sprites;
	active_sprites = nullptr;
}

void CSpriteMap_Element::setPosition(float x, float y)
{
	if(!drawable)
		return;
#ifndef SPRITE_MAP_USE_VERTEX
	for(int i = 0; i < tile_count; i++)
	{
		sprites[i].setPosition(x, y);
		x += tile_width;
	}
#else
	if(transformable != nullptr)
		transformable->setPosition(x, y);
#endif
}

void CSpriteMap_Element::setOrigin(float ox, float oy)
{
	if(!drawable)
		return;
#ifndef SPRITE_MAP_USE_VERTEX
	for(int i = 0; i < tile_count; i++)
	{
		sprites[i].setOrigin(ox, oy);
	}
#else
	if(transformable != nullptr)
		transformable->setOrigin(ox, oy);
#endif
}

void CSpriteMap_Element::setTileScale(float scale)
{
	if(!drawable)
		return;
#ifndef SPRITE_MAP_USE_VERTEX
	for(int i = 0; i < tile_count; i++)
	{
		sprites[i].setScale(scale, scale);
	}
#else
	if(transformable != nullptr)
		transformable->setScale(scale, scale);
#endif
}

CSpriteMap_Element::~CSpriteMap_Element() {
	drawable = false;
	visible = false;
	dispose_stack();
}