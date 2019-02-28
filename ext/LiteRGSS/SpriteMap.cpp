#include "LiteRGSS.h"
#include "CBitmap_Element.h"
#include "CRect_Element.h"

VALUE rb_cSpriteMap = Qnil;

template<>
void rb::Mark<CSpriteMap_Element>(CSpriteMap_Element* sprite)
{
    if(sprite == nullptr)
        return;
    rb_gc_mark(sprite->rViewport);
    rb_gc_mark(sprite->rX);
    rb_gc_mark(sprite->rY);
    rb_gc_mark(sprite->rZ);
    rb_gc_mark(sprite->rOX);
    rb_gc_mark(sprite->rOY);
    rb_gc_mark(sprite->rScale);
}

void Init_SpriteMap()
{
    rb_cSpriteMap = rb_define_class_under(rb_mLiteRGSS, "SpriteMap", rb_cDrawable);
    rb_define_alloc_func(rb_cSpriteMap, rb::AllocDrawable<CSpriteMap_Element>);

    rb_define_method(rb_cSpriteMap, "initialize", _rbf rb_SpriteMap_Initialize, -1);
    rb_define_method(rb_cSpriteMap, "dispose", _rbf rb_SpriteMap_Dispose, 0);
    rb_define_method(rb_cSpriteMap, "viewport", _rbf rb_SpriteMap_Viewport, 0);
    rb_define_method(rb_cSpriteMap, "x", _rbf rb_SpriteMap_X, 0);
    rb_define_method(rb_cSpriteMap, "x=", _rbf rb_SpriteMap_SetX, 1);
    rb_define_method(rb_cSpriteMap, "y", _rbf rb_SpriteMap_Y, 0);
    rb_define_method(rb_cSpriteMap, "y=", _rbf rb_SpriteMap_SetY, 1);
    rb_define_method(rb_cSpriteMap, "set_position", _rbf rb_SpriteMap_SetPosition, 2);
    rb_define_method(rb_cSpriteMap, "z", _rbf rb_SpriteMap_Z, 0);
    rb_define_method(rb_cSpriteMap, "z=", _rbf rb_SpriteMap_SetZ, 1);
    rb_define_method(rb_cSpriteMap, "ox", _rbf rb_SpriteMap_OX, 0);
    rb_define_method(rb_cSpriteMap, "ox=", _rbf rb_SpriteMap_SetOX, 1);
    rb_define_method(rb_cSpriteMap, "oy", _rbf rb_SpriteMap_OY, 0);
    rb_define_method(rb_cSpriteMap, "oy=", _rbf rb_SpriteMap_SetOY, 1);
    rb_define_method(rb_cSpriteMap, "tile_scale", _rbf rb_SpriteMap_TileScale, 0);
    rb_define_method(rb_cSpriteMap, "tile_scale=", _rbf rb_SpriteMap_TileScaleSet, 1);
    rb_define_method(rb_cSpriteMap, "set_origin", _rbf rb_SpriteMap_SetOrigin, 2);
    rb_define_method(rb_cSpriteMap, "reset", _rbf rb_SpriteMap_Reset, 0);
    rb_define_method(rb_cSpriteMap, "set", _rbf rb_SpriteMap_Set, -1);

    rb_define_method(rb_cSprite, "clone", _rbf rb_SpriteMap_Copy, 0);
    rb_define_method(rb_cSprite, "dup", _rbf rb_SpriteMap_Copy, 0);
}

VALUE rb_SpriteMap_Initialize(int argc, VALUE* argv, VALUE self)
{
    auto& sprite = rb::Get<CSpriteMap_Element>(self);
    VALUE viewport, tile_width, tile_count;
    rb_scan_args(argc, argv, "30", &viewport, &tile_width, &tile_count);

	// Viewport push
	if (rb_obj_is_kind_of(viewport, rb_cViewport) != Qtrue)
		rb_raise(rb_eRGSSError, "SpriteMap require viewport to be initialized.");
    auto &viewport_el = rb::Get<CViewport_Element>(viewport);
	viewport_el.bind(sprite);
	sprite.rViewport = viewport;
    VALUE table = rb_ivar_get(viewport, rb_iElementTable);
    rb_ary_push(table, self);

    // Sprite definition
    sprite.define_map(NUM2ULONG(tile_width), NUM2ULONG(tile_count));
}

VALUE rb_SpriteMap_Dispose(VALUE self)
{
	return rb::Dispose<CSpriteMap_Element>(self);
}

VALUE rb_SpriteMap_Viewport(VALUE self)
{
    auto& sprite = rb::Get<CSpriteMap_Element>(self);
    return sprite.rViewport;
}

VALUE rb_SpriteMap_X(VALUE self)
{
    auto& sprite = rb::Get<CSpriteMap_Element>(self);
    return sprite.rX;
}

VALUE rb_SpriteMap_SetX(VALUE self, VALUE val)
{
    auto& sprite = rb::Get<CSpriteMap_Element>(self);
    sprite.setPosition(NUM2DBL(val), NUM2DBL(sprite.rY));
    sprite.rX = val;
    return self;
}

VALUE rb_SpriteMap_Y(VALUE self)
{
    auto& sprite = rb::Get<CSpriteMap_Element>(self);
    return sprite.rY;
}

VALUE rb_SpriteMap_SetY(VALUE self, VALUE val)
{
    auto& sprite = rb::Get<CSpriteMap_Element>(self);
    sprite.setPosition(NUM2DBL(sprite.rX), NUM2DBL(val));
    sprite.rY = val;
    return self;
}

VALUE rb_SpriteMap_SetPosition(VALUE self, VALUE x, VALUE y)
{
    auto& sprite = rb::Get<CSpriteMap_Element>(self);
    sprite.setPosition(NUM2DBL(x), NUM2DBL(y));
    sprite.rX = x;
    sprite.rY = y;
    return self;
}

VALUE rb_SpriteMap_Z(VALUE self)
{
    auto& sprite = rb::Get<CSpriteMap_Element>(self);
    return sprite.rZ;
}

VALUE rb_SpriteMap_SetZ(VALUE self, VALUE val)
{
    auto& sprite = rb::Get<CSpriteMap_Element>(self);
    NUM2LONG(val);
    sprite.rZ = val;
    return self;
}

VALUE rb_SpriteMap_OX(VALUE self)
{
    auto& sprite = rb::Get<CSpriteMap_Element>(self);
    return sprite.rOX;
}

VALUE rb_SpriteMap_SetOX(VALUE self, VALUE val)
{
    auto& sprite = rb::Get<CSpriteMap_Element>(self);
    sprite.setOrigin(NUM2DBL(val), NUM2DBL(sprite.rOY));
    sprite.rOX = val;
    return self;
}

VALUE rb_SpriteMap_OY(VALUE self)
{
    auto& sprite = rb::Get<CSpriteMap_Element>(self);
    return sprite.rOY;
}

VALUE rb_SpriteMap_SetOY(VALUE self, VALUE val)
{
    auto& sprite = rb::Get<CSpriteMap_Element>(self);
    sprite.setOrigin(NUM2DBL(sprite.rOX), NUM2DBL(val));
    sprite.rOY = val;
    return self;
}

VALUE rb_SpriteMap_SetOrigin(VALUE self, VALUE x, VALUE y)
{
    auto& sprite = rb::Get<CSpriteMap_Element>(self);
    sprite.setOrigin(NUM2DBL(x), NUM2DBL(y));
    sprite.rOX = x;
    sprite.rOY = y;
    return self;
}

VALUE rb_SpriteMap_Reset(VALUE self)
{
    auto& sprite = rb::Get<CSpriteMap_Element>(self);
    sprite.reset();
    return self;
}

VALUE rb_SpriteMap_Set(int argc, VALUE* argv, VALUE self)
{
    auto& sprite = rb::Get<CSpriteMap_Element>(self);
    VALUE index, bitmap, rect;
    rb_scan_args(argc, argv, "30", &index, &bitmap, &rect);
    // Retreiving Bitmap
    auto& texture = rb_Bitmap_getTexture(bitmap);
    // Retreiving Rect
    if(rb_obj_is_kind_of(rect, rb_cRect) != Qtrue)
        rb_raise(rb_eRGSSError, "Expected Rect got %s", RSTRING_PTR(rb_class_name(CLASS_OF(self))));
    auto& rect_el = rb::Get<CRect_Element>(rect);

    sprite.setTile(NUM2LONG(index), rect_el.getRect(), texture);

    return self;
}

VALUE rb_SpriteMap_TileScale(VALUE self)
{
    auto& sprite = rb::Get<CSpriteMap_Element>(self);
    return sprite.rScale;
}

VALUE rb_SpriteMap_TileScaleSet(VALUE self, VALUE val)
{
    auto& sprite = rb::Get<CSpriteMap_Element>(self);
    sprite.setTileScale(NUM2DBL(val));
    sprite.rScale = val;
    return self;
}


VALUE rb_SpriteMap_Copy(VALUE self)
{
    rb_raise(rb_eRGSSError, "SpriteMaps cannot be cloned or duplicated.");
    return self;
}