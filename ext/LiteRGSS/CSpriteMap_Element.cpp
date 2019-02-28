#include "CViewport_Element.h"

void CSpriteMap_Element::draw(sf::RenderTarget& target) const 
{
	CSpriteMap_Element::drawFast(target);
}

void CSpriteMap_Element::drawFast(sf::RenderTarget& target) const 
{
    if(drawable && visible)
    {
        for(int i = 0; i < tile_count; i++)
        {
            if(active_sprites[i])
                target.draw(sprites[i]);
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
    sprites = new sf::Sprite[tile_count];
    active_sprites = new bool[tile_count];
    drawable = true;
    reset();
}

void CSpriteMap_Element::setTile(long index, sf::IntRect& tile, sf::Texture& texture)
{
    if(index < 0 || index >= tile_count)
        return;
    sprites[index].setTexture(texture);
    sprites[index].setTextureRect(tile);
    active_sprites[index] = true;
}

void CSpriteMap_Element::reset()
{
    for(long i = 0; i < tile_count; i++)
        active_sprites[i] = false;
}

void CSpriteMap_Element::dispose_stack()
{
    if(sprites != nullptr)
        delete[] sprites;
    sprites = nullptr;
    if(active_sprites != nullptr)
        delete[] active_sprites;
    active_sprites = nullptr;
}

void CSpriteMap_Element::setPosition(float x, float y)
{
    if(!drawable)
        return;
    for(int i = 0; i < tile_count; i++)
    {
        sprites[i].setPosition(x, y);
        x += tile_width;
    }
}

void CSpriteMap_Element::setOrigin(float ox, float oy)
{
    if(!drawable)
        return;
    for(int i = 0; i < tile_count; i++)
    {
        sprites[i].setOrigin(ox, oy);
    }
}

void CSpriteMap_Element::setTileScale(float scale)
{
    if(!drawable)
        return;
    for(int i = 0; i < tile_count; i++)
    {
        sprites[i].setScale(scale, scale);
    }
}

CSpriteMap_Element::~CSpriteMap_Element() {
    drawable = false;
    visible = false;
    dispose_stack();
}