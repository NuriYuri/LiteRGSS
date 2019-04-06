#include "LiteRGSS.h"
#include "Bitmap.h"
#include "CViewport_Element.h"
#include "CWindow_Element.h"
#include "CGraphics.h"
#include "CRect_Element.h"
#include <iostream>

CWindow_Element::CWindow_Element()
{
	rX = rY = rZ = rOX = rOY = LONG2FIX(0);
	rWidth = rHeight = LONG2FIX(-1);
	rBackOpacity = rContentOpacity = rOpacity = LONG2FIX(255);
	rActive = rPause = Qfalse;
	rStretch = Qfalse;
}

void CWindow_Element::draw(sf::RenderTarget& target) const {
	CWindow_Element::drawFast(target);
}

void CWindow_Element::drawFast(sf::RenderTarget& target) const 
{
    if(!vertices.empty() && visible)
    {
		/* Draw window */
		for(unsigned long i = 0; i < num_vertices_line; i++)
			target.draw(vertices[i], texture);
		/* draw internal */
		sf::View targetView = view;
		sf::View originalView = sf::View(target.getView());
		if (!NIL_P(rViewport))
			drawCalculateView(target, targetView);
		target.setView(targetView);
		CView_Element::drawFast(target);

		/* Draw cursor & reset view */
		if(RTEST(rActive) && rCursorSkin != Qnil)
			target.draw(cursor_sprite);
		target.setView(originalView);
		/* Draw pause */
		if(RTEST(rPause) && rPauseSkin != Qnil)
			target.draw(pause_sprite);
    }
}

void CWindow_Element::drawCalculateView(sf::RenderTarget & target, sf::View & targetView) const
{
	const sf::FloatRect originViewport = target.getView().getViewport();
	sf::FloatRect targetViewport = sf::FloatRect(targetView.getViewport());
	sf::Vector2f targetCenter = sf::Vector2f(targetView.getCenter());
	sf::Vector2f targetSize = sf::Vector2f(targetView.getSize());
	// Zoom adjustment
	float zoom = NUM2DBL(rb::Get<CViewport_Element>(rViewport).rZoom);
	targetViewport.left /= zoom;
	targetViewport.top /= zoom;
	targetViewport.width /= zoom;
	targetViewport.height /= zoom;
	// ---
	targetViewport.left += originViewport.left;
	targetViewport.top += originViewport.top;
	float dx = targetViewport.left + targetViewport.width - (originViewport.left + originViewport.width);
	if (dx > 0)
	{
		targetCenter.x -= targetSize.x / 2;
		targetSize.x *= ((targetViewport.width - dx) / targetViewport.width);
		targetViewport.width -= dx;
		targetCenter.x += targetSize.x / 2;
	}
	dx = originViewport.left - targetViewport.left;
	if (dx > 0)
	{
		targetViewport.left += dx;
	}
	float dy = targetViewport.top + targetViewport.height - (originViewport.top + originViewport.height);
	if (dy > 0)
	{
		targetCenter.y -= targetSize.y / 2;
		targetSize.y *= ((targetViewport.height - dy) / targetViewport.height);
		targetViewport.height -= dy;
		targetCenter.y += targetSize.y / 2;
	}
	dy = originViewport.top - targetViewport.top;
	if (dy > 0)
	{
		targetViewport.top += dy;
	}
	targetView.setSize(targetSize);
	targetView.setCenter(targetCenter);
	targetView.setViewport(targetViewport);
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
	if (locked)
		return;
	if (texture == nullptr)
		return;
	if (rb_DrawableDisposable_disposed(rBitmap) == Qtrue)
		return;
	if (NIL_P(rWindowBuilder))
		return;
	long wt = NUM2LONG(rWidth);
	long ht = NUM2LONG(rHeight);
	if (wt <= 0 || ht <= 0)
		return;
	if (RTEST(rStretch))
		updateVerticesStretch(wt, ht);
	else
		updateVerticesBlt(wt, ht);
	updateBackOpacity();
	resetPausePosition();
	if (!NIL_P(rCursorRect))
		resetCursorPosition(&rb::GetSafe<CRect_Element>(rCursorRect, rb_cRect).getRect());
}

void CWindow_Element::update()
{
	updatePauseSprite();
	updateCursorSprite();
	counter += 1;
	if (counter >= 128)
		counter = 0;
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

void CWindow_Element::updateVerticesStretch(long wt, long ht)
{
	sf::Vector2i a(0, 0);
	sf::Vector2i s(0, 0);
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
	long delta_w = wt - w1 - w3; // <=> w2 w5 & w8
								 // If delta_w < 0, the middle of the window doesn't exist, we try to fix the overlap of 1 on 3
	if (delta_w < 0)
	{
		delta_w /= 2;
		w1 += delta_w;
		w3 += delta_w;
		delta_w = wt - w1 - w3;
	}

	long h1 = ym; // <=> h2 & h3
	long h7 = hs - hm - ym; // <=> h8 & h9
	long delta_h = ht - h1 - h7; // <=> h4, h5 & h6
								 // If delta_h < 0, the middle of the window doesn't exist, we try to fix the overlap of 1 on 7
	if (delta_h < 0)
	{
		delta_h /= 2;
		h1 += delta_h;
		h7 += delta_h;
		delta_h = ht - h1 - h7;
	}

	// Allocate the vertices to perform the job
	allocateVerticesStretch();

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

	// 4 Side vertice calculation
	// [ ..., 2, ...] / [ ..., ..., ...] / [ ..., ..., ...]
	s.x = delta_w;
	s.y = h1;
	a.x = w1;
	a.y = 0;
	rectSet(rect, xm, 0, wm, h1);
	calculateVerticesStretch(x, y, 0, 1, s, a, rect);
	// [ ..., ..., ...] / [ ..., ..., ...] / [ ..., 8, ...]
	s.y = h7;
	a.x = w1;
	a.y = ht - h7;
	rectSet(rect, xm, hs - h7, wm, h7);
	calculateVerticesStretch(x, y, num_vertices_line - 1, 1, s, a, rect);
	// [ ..., ..., ...] / [ 4, ..., ...] / [ ..., ..., ...]
	s.x = w1;
	s.y = delta_h;
	a.x = 0;
	a.y = h1;
	rectSet(rect, 0, ym, w1, hm);
	calculateVerticesStretch(x, y, 1, 0, s, a, rect);
	// [ ..., ..., ...] / [ ..., ..., 6] / [ ..., ..., ...]
	s.x = w3;
	a.x = wt - w3;
	a.y = h1;
	rectSet(rect, ws - w3, ym, w3, hm);
	calculateVerticesStretch(x, y, 1, 2, s, a, rect);

	// Middle vertice calculation
	s.x = delta_w;
	a.x = w1;
	a.y = h1;
	rectSet(rect, xm, ym, wm, hm);
	calculateVerticesStretch(x, y, 1, 1, s, a, rect);
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
	auto& vertArr = vertices[line];
	int i = cell * 6;
	if (cell < 0)
		i += vertArr.getVertexCount();
	x += a.x;
	y += a.y;
	// Texture coords
	vertArr[i].texCoords = sf::Vector2f(rect.left, rect.top);
	vertArr[i + 1].texCoords = vertArr[i + 3].texCoords = sf::Vector2f(rect.left, rect.height + rect.top);
	vertArr[i + 2].texCoords = vertArr[i + 4].texCoords = sf::Vector2f(rect.width + rect.left, rect.top);
	vertArr[i + 5].texCoords = sf::Vector2f(rect.width + rect.left, rect.height + rect.top);
	// Coordinates
	vertArr[i].position = sf::Vector2f(x, y);
	vertArr[i + 1].position = vertArr[i + 3].position = sf::Vector2f(x, y + rect.height);
	vertArr[i + 2].position = vertArr[i + 4].position = sf::Vector2f(x + rect.width, y);
	vertArr[i + 5].position = sf::Vector2f(x + rect.width, y + rect.height);
}

void CWindow_Element::calculateVerticesStretch(long x, long y, long line, long cell, sf::Vector2i &s, sf::Vector2i &a, sf::IntRect &rect)
{
	auto& vertArr = vertices[line];
	int i = cell * 6;
	if (cell < 0)
		i += vertArr.getVertexCount();
	x += a.x;
	y += a.y;
	// Texture coords
	vertArr[i].texCoords = sf::Vector2f(rect.left, rect.top);
	vertArr[i + 1].texCoords = vertArr[i + 3].texCoords = sf::Vector2f(rect.left, rect.height + rect.top);
	vertArr[i + 2].texCoords = vertArr[i + 4].texCoords = sf::Vector2f(rect.width + rect.left, rect.top);
	vertArr[i + 5].texCoords = sf::Vector2f(rect.width + rect.left, rect.height + rect.top);
	// Coordinates
	vertArr[i].position = sf::Vector2f(x, y);
	vertArr[i + 1].position = vertArr[i + 3].position = sf::Vector2f(x, y + s.y);
	vertArr[i + 2].position = vertArr[i + 4].position = sf::Vector2f(x + s.x, y);
	vertArr[i + 5].position = sf::Vector2f(x + s.x, y + s.y);
}

void CWindow_Element::updatePauseSprite()
{
	if (NIL_P(rPauseSkin))
		return;
	long v = counter / 16;
	sf::Vector2u size = pause_sprite.getTexture()->getSize();
	size.x /= 2;
	size.y /= 2;
	pause_sprite.setTextureRect(sf::IntRect(
		(v & 0x01) == 1 ? size.x : 0,
		(v & 0x02) == 2 ? size.y : 0,
		size.x,
		size.y
	));
	sf::Color col = pause_sprite.getColor();
	col.a = NUM2LONG(rOpacity);
	pause_sprite.setColor(col);
}

void CWindow_Element::updateCursorSprite()
{
	if (NIL_P(rCursorSkin))
		return;
	long v = counter;
	sf::Color col = cursor_sprite.getColor();
	col.a = NUM2LONG(rOpacity);
	if (v > 64)
	{
		v -= 64;
		col.a = (RTEST(rActive) ? 128 + 2 * v : 128) * col.a / 255;
	}
	else
		col.a = (RTEST(rActive) ? 255 - 2 * v : 128) * col.a / 255;
	cursor_sprite.setColor(col);
}

void CWindow_Element::allocateVerticesBlt(long delta_w, long nb2, long delta_h, long nb4)
{
	// Ajustment of the real amount of cell in x
	if (delta_w > 0)
		nb2 += 3;
	else
		nb2 += 2;
	// Calculate the real amount of vertices
	nb2 *= 6;
	// Ajustment of the real amount of cell in y
	if (delta_h > 0)
		nb4 += 3;
	else
		nb4 += 2;
	// Delete the vertices if the number of line is different
	// Alloction of the vertex Arrays (if no vertices or different lines)
	if(vertices.empty() || nb4 != num_vertices_line)
		vertices = std::vector<sf::VertexArray>(nb4);
	// Update the number of lines
	num_vertices_line = nb4;
	for (long i = 0; i < nb4; i++)
	{
		vertices[i].setPrimitiveType(sf::Triangles);
		if(nb2 != vertices[i].getVertexCount())
			vertices[i].resize(nb2);
	}
}

void CWindow_Element::allocateVerticesStretch()
{
	allocateVerticesBlt(0, 1, 0, 1);
}

void CWindow_Element::updateContents()
{
	if (!NIL_P(rCursorRect))
		resetCursorPosition(&rb::GetSafe<CRect_Element>(rCursorRect, rb_cRect).getRect());
}

void CWindow_Element::updateBackOpacity()
{
	sf::Color col = sf::Color::White;
	col.a = NUM2LONG(rOpacity) * NUM2LONG(rBackOpacity) / 255;
	if (!vertices.empty())
	{
		long num_vertices = vertices[0].getVertexCount();
		for (int i = 0; i < num_vertices_line; i++)
		{
			auto& vertArr = vertices[i];
			for (int j = 0; j < num_vertices; j++)
			{
				vertArr[j].color = col;
			}
		}
	}
	updatePauseSprite();
	updateCursorSprite();
}

void CWindow_Element::updateView()
{
	if (NIL_P(rWindowBuilder))
		return;
	long offset_x = NUM2LONG(rb_ary_entry(rWindowBuilder, 4));
	long offset_y = NUM2LONG(rb_ary_entry(rWindowBuilder, 5));
	long x = NUM2LONG(rX) + offset_x;
	long y = NUM2LONG(rY) + offset_y;
	long width = NUM2LONG(rWidth) - 2 * offset_x;
	long height = NUM2LONG(rHeight) - 2 * offset_y;
	// Update rect
	sf::IntRect& rect = rb::GetSafe<CRect_Element>(rRect, rb_cRect).getRect();
	rect.left = x;
	rect.top = y;
	rect.width = width;
	rect.height = height;
	// Update Window view
	if (width & 1)
		width++;
	if (height & 1)
		height++;
	view.setCenter(static_cast<float>(NUM2LONG(rOX) + width / 2),
		static_cast<float>(NUM2LONG(rOY) + height / 2));
	view.setSize(static_cast<float>(width), static_cast<float>(height));
	float sw = static_cast<float>(CGraphics::Get().screenWidth());
	float sh = static_cast<float>(CGraphics::Get().screenHeight());
	sf::FloatRect frect(x / sw, y / sh, width / sw, height / sh);
	view.setViewport(frect);
}

void CWindow_Element::lock()
{
	locked = true;
}

void CWindow_Element::unlock()
{
	locked = false;
	updateVertices();
}

bool CWindow_Element::is_locked()
{
	return locked;
}

sf::Sprite& CWindow_Element::getPauseSprite()
{
	return pause_sprite;
}

sf::Sprite& CWindow_Element::getCursorSprite()
{
	return cursor_sprite;
}

void CWindow_Element::resetPausePosition()
{
	if (NIL_P(rPauseSkin))
		return;
	long pause_x, pause_y;
	if (NIL_P(rPauseX))
		pause_x = (NUM2LONG(rWidth) - rb_Bitmap_getTexture(rPauseSkin).getSize().x) / 2;
	else
		pause_x = NUM2LONG(rPauseX);
	if (NIL_P(rPauseY))
		pause_y = NUM2LONG(rHeight) - rb_Bitmap_getTexture(rPauseSkin).getSize().y / 2 - 2;
	else
		pause_y = NUM2LONG(rPauseY);
	pause_x += NUM2LONG(rX);
	pause_y += NUM2LONG(rY);
	pause_sprite.setPosition(sf::Vector2f(pause_x, pause_y));
}

void CWindow_Element::resetCursorPosition(sf::IntRect * rect)
{
	if (NIL_P(rCursorSkin))
		return;
	sf::Vector2f size = static_cast<sf::Vector2f>(cursor_sprite.getTexture()->getSize());
	cursor_sprite.setScale(sf::Vector2f(rect->width / size.x, rect->height / size.y));
	cursor_sprite.setPosition(sf::Vector2f(rect->left, rect->top));
}

void CWindow_Element::updateContentsOpacity() {
	long opacity = NUM2LONG(rOpacity) * NUM2LONG(rContentOpacity) / 255;
	
	CView_Element::updateContentsOpacity(opacity);
}
