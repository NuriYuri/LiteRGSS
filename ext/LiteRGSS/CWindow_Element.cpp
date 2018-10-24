#include "CViewport_Element.h"
#include "CWindow_Element.h"
#include "Graphics.h"
#include "Bitmap.h"
#include <iostream>

CWindow_Element::CWindow_Element() : CDrawable_Element()
{
	vertices = nullptr;
	visible = true;
	texture = nullptr;
	rViewport = Qnil;
	rBitmap = Qnil;
	rX = rY = rZ = rOX = rOY = LONG2FIX(0);
	rWidth = rHeight = LONG2FIX(-1);
	rCursorRect = Qnil;
	rBackOpacity = rContentOpacity = rOpacity = LONG2FIX(255);
	rWindowBuilder = rPauseSkin = rCursorSkin = Qnil;
	rPause = Qfalse;
	rPauseX = rPauseY = LONG2FIX(0);
}

CWindow_Element::~CWindow_Element()
{
	if (vertices)
		delete vertices;
	vertices = nullptr;
}

void CWindow_Element::draw(sf::RenderTarget& target) const
{
	CWindow_Element::drawFast(target);
}

void CWindow_Element::drawFast(sf::RenderTarget& target) const 
{
    if(vertices != nullptr && visible)
    {
		for(unsigned long i = 0; i < num_vertices_line; i++)
			target.draw(vertices[i], texture);
		/* draw internal */


		target.draw(pause_sprite);
		target.draw(cursor_sprite);
    }
}

sf::VertexArray * CWindow_Element::getVertices()
{
	return vertices;
}

void CWindow_Element::setVertices(sf::VertexArray * _vertices)
{
	vertices = _vertices;
}

sf::Texture * CWindow_Element::getTexture()
{
	return texture;
}

void CWindow_Element::setTexture(sf::Texture * text)
{
	texture = text;
}

void CWindow_Element::updateVertices()
{
	// Checkup part
	if (texture == nullptr)
		return;
	if (rb_Bitmap_Disposed(rBitmap) == Qtrue)
		return;
	if (NIL_P(rWindowBuilder))
		return;
	long wt = NUM2LONG(rWidth);
	long ht = NUM2LONG(rHeight);
	if (wt <= 0 || ht <= 0)
		return;
	updateVerticesBlt(wt, ht);
}

void CWindow_Element::updateVerticesBlt(long wt, long ht)
{
	sf::Vector2i a(0, 0);
	sf::IntRect rect;
	long x = NUM2LONG(rX);
	long y = NUM2LONG(rY);
	long xm = NUM2LONG(rb_ary_entry(rWindowBuilder, 0));
	long ym = NUM2LONG(rb_ary_entry(rWindowBuilder, 1));
	long wm = NUM2LONG(rb_ary_entry(rWindowBuilder, 2));
	long hm = NUM2LONG(rb_ary_entry(rWindowBuilder, 3));
	long ws = texture->getSize().x;
	long hs = texture->getSize().y;
	// 1 2 3
	// 4 5 6
	// 7 8 9
	long w3 = ws - xm - wm; // <=> w6 & w9
	long w1 = xm; // <=> w4 & w7
	long delta_w = wt - w1 - w3; // <=> w2 w5 & w8 before the remaining adjustment
	// If delta_w < 0, the middle of the window doesn't exist, we try to fix the overlap of 1 on 3
	if (delta_w < 0)
	{
		delta_w /= 2;
		w1 += delta_w;
		w3 += delta_w;
		delta_w = wt - w1 - w3;
	}
	// Number of tiles on the part 2 of the window
	long nb2 = delta_w / wm;
	// Ajustment of the remaining pixels on the part 2 of the window
	delta_w = delta_w - (nb2*wm);

	long h1 = ym; // <=> h2 & h3
	long h7 = hs - hm - ym; // <=> h8 & h9
	long delta_h = ht - h1 - h7; // <=> h4, h5 & h6 before the adjustment
	// If delta_h < 0, the middle of the window doesn't exist, we try to fix the overlap of 1 on 7
	if (delta_h < 0)
	{
		delta_h /= 2;
		h1 += delta_h;
		h7 += delta_h;
		delta_h = ht - h1 - h7;
	}
	// Number of tiles on the part 4 of the window
	long nb4 = delta_h / hm;
	// Adjustment of the remaining pixels on the part 4 of the window
	delta_h = delta_h - (nb4*hm);

	// Allocate the vertices to perform the job
	allocateVerticesBlt(delta_w, nb2, delta_h, nb4);

	// 4 corner vertice calculation
	// [ 1, ..., ...] / [ ..., ..., ...] / [ ..., ..., ...]
	rectSet(rect, 0, 0, w1, h1);
	calculateVertices(x, y, 0, 0, a, rect);
	// [ ..., ..., 3] / [ ..., ..., ...] / [ ..., ..., ...]
	a.x = wt - w3;
	rectSet(rect, ws - w3, 0, w3, h1);
	calculateVertices(x, y, 0, -1, a, rect);
	// [ ..., ..., ...] / [ ..., ..., ...] / [ ..., ..., 9]
	a.y = ht - h7;
	rectSet(rect, ws - w3, hs - h7, w3, h7);
	calculateVertices(x, y, num_vertices_line - 1, -1, a, rect);
	// [ ..., ..., ...] / [ ..., ..., ...] / [ 7, ..., ...]
	a.x = 0;
	rectSet(rect, 0, hs - h7, w1, h7);
	calculateVertices(x, y, num_vertices_line - 1, 0, a, rect);

	// Side vertice calculation
	int count;
	// [ ..., 2, ...] / [ ..., ..., ...] / [ ..., ..., ...]
	a.x = w1;
	a.y = 0;
	rectSet(rect, xm, 0, wm, h1);
	for (count = nb2; count > 0; count--)
	{
		calculateVertices(x, y, 0, count, a, rect);
		a.x += wm;
	}
	if (delta_w > 0)
	{
		rect.width = delta_w;
		calculateVertices(x, y, 0, -2, a, rect);
	}
	// [ ..., ..., ...] / [ 4, ..., ...] / [ ..., ..., ...]
	a.x = 0;
	a.y = h1;
	rectSet(rect, 0, ym, w1, hm);
	for (count = nb4; count > 0; count--)
	{
		calculateVertices(x, y, count, 0, a, rect);
		a.y += hm;
	}
	if (delta_h > 0)
	{
		rect.height = delta_h;
		calculateVertices(x, y, nb4 + 1, 0, a, rect);
	}
	// [ ..., ..., ...] / [ ..., ..., 6] / [ ..., ..., ...]
	a.x = wt - w3;
	a.y = h1;
	rectSet(rect, ws - w3, ym, w3, hm);
	for (count = nb4; count > 0; count--)
	{
		calculateVertices(x, y, count, -1, a, rect);
		a.y += hm;
	}
	if (delta_h > 0)
	{
		rect.height = delta_h;
		calculateVertices(x, y, nb4 + 1, -1, a, rect);
	}
	// [ ..., ..., ...] / [ ..., ..., ...] / [ ..., 8, ...]
	a.x = w1;
	a.y = ht - h7;
	rectSet(rect, xm, hs - h7, wm, h7);
	for (count = nb2; count > 0; count--)
	{
		calculateVertices(x, y, num_vertices_line - 1, count, a, rect);
		a.x += wm;
	}
	if (delta_w > 0)
	{
		rect.width = delta_w;
		calculateVertices(x, y, num_vertices_line - 1, -2, a, rect);
	}

	// Inside vertices calculation
	int count2;
	// [ ..., ..., ...] / [ ..., 5|m , ...] / [ ..., ..., ...]
	a.x = w1;
	a.y = h1;
	rectSet(rect, xm, ym, wm, hm);
	for (count = 1; count <= nb2; count++)
	{
		for (count2 = 1; count2 <= nb4; count2++)
		{
			calculateVertices(x, y, count2, count, a, rect);
			a.y += hm;
		}
		a.y = h1;
		a.x += wm;
	}
	a.y += hm * nb4;
	if (delta_h > 0 && delta_w > 0)
	{
		rectSet(rect, xm, ym, delta_w, delta_h);
		calculateVertices(x, y, num_vertices_line - 2, -2, a, rect);
	}
	if (delta_h > 0)
	{
		a.x = w1;
		rectSet(rect, xm, ym, wm, delta_h);
		for (count = nb2; count > 0; count--)
		{
			calculateVertices(x, y, num_vertices_line - 2, count, a, rect);
			a.x += wm;
		}
	}
	if (delta_w > 0)
	{
		a.y = h1;
		rectSet(rect, xm, ym, delta_w, hm);
		for (count = nb4; count > 0; count--)
		{
			calculateVertices(x, y, count, -2, a, rect);
			a.y += hm;
		}
	}
}

void CWindow_Element::rectSet(sf::IntRect &rect, long x, long y, long width, long height)
{
	rect.left = x;
	rect.top = y;
	rect.width = width;
	rect.height = height;
}

void CWindow_Element::calculateVertices(long x, long y, long line, long cell, sf::Vector2i &a, sf::IntRect &rect)
{
	sf::VertexArray vertArr = vertices[line];
	int i = cell * 6;
	if (cell < 0)
		i += vertArr.getVertexCount();
	x += a.x;
	y += a.y;
	// Texture coords
	vertArr[i].texCoords = sf::Vector2f(rect.width + rect.left, rect.top);
	vertArr[i + 1].texCoords = vertArr[i + 3].texCoords = sf::Vector2f(rect.width + rect.left, rect.height + rect.top);
	vertArr[i + 2].texCoords = vertArr[i + 4].texCoords = sf::Vector2f(rect.left, rect.top);
	vertArr[i + 5].texCoords = sf::Vector2f(rect.left, rect.height + rect.top);
	// Coordinates
	vertArr[i].position = sf::Vector2f(x, y);
	vertArr[i + 1].position = vertArr[i + 3].position = sf::Vector2f(x, y + rect.height);
	vertArr[i + 2].position = vertArr[i + 4].position = sf::Vector2f(x + rect.width, y);
	vertArr[i + 5].position = sf::Vector2f(x + rect.width, y + rect.height);
}

void CWindow_Element::allocateVerticesBlt(long delta_w, long nb2, long delta_h, long nb4)
{
	if (vertices != nullptr)
		delete[] vertices;
	// Ajustment of the real amount of cell in x
	if (delta_w > 0)
		nb2 += 2;
	else
		nb2 += 1;
	// Calculate the real amount of vertices
	nb2 *= 6;
	// Ajustment of the real amount of cell in y
	if (delta_h > 0)
		nb4 += 2;
	else
		nb4 += 1;
	// Alloction of the vertex Arrays
	vertices = new sf::VertexArray[nb4];
	num_vertices_line = nb4;
	for (long i = 0; i < nb4; i++)
	{
		vertices[i].setPrimitiveType(sf::Triangles);
		vertices[i].resize(nb2);
	}
}

void CWindow_Element::updateContents()
{
}
