#ifndef L_SPRITE_MAP_HEADER
#define L_SPRITE_MAP_HEADER
#include "Graphics.h"
#include "utils/rbAdapter.h"

VALUE rb_SpriteMap_Initialize(int argc, VALUE* argv, VALUE self);
VALUE rb_SpriteMap_Dispose(VALUE self);
VALUE rb_SpriteMap_Viewport(VALUE self);
VALUE rb_SpriteMap_X(VALUE self);
VALUE rb_SpriteMap_SetX(VALUE self, VALUE val);
VALUE rb_SpriteMap_Y(VALUE self);
VALUE rb_SpriteMap_SetY(VALUE self, VALUE val);
VALUE rb_SpriteMap_SetPosition(VALUE self, VALUE x, VALUE y);
VALUE rb_SpriteMap_Z(VALUE self);
VALUE rb_SpriteMap_SetZ(VALUE self, VALUE val);
VALUE rb_SpriteMap_OX(VALUE self);
VALUE rb_SpriteMap_SetOX(VALUE self, VALUE val);
VALUE rb_SpriteMap_OY(VALUE self);
VALUE rb_SpriteMap_SetOY(VALUE self, VALUE val);
VALUE rb_SpriteMap_SetOrigin(VALUE self, VALUE x, VALUE y);
VALUE rb_SpriteMap_Reset(VALUE self);
VALUE rb_SpriteMap_Set(int argc, VALUE* argv, VALUE self);
VALUE rb_SpriteMap_TileScale(VALUE self);
VALUE rb_SpriteMap_TileScaleSet(VALUE self, VALUE val);

VALUE rb_SpriteMap_Copy(VALUE self);


namespace rb {
    template<>
    void Mark<CSpriteMap_Element>(CSpriteMap_Element* sprite);
}

#endif
