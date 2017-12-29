#ifndef L_BITMAP_H
#define L_BITMAP_H
#include "Graphics.h"

VALUE rb_Sprite_Initialize(int argc, VALUE* argv, VALUE self);
VALUE rb_Sprite_Copy(VALUE self);
VALUE rb_Sprite_Dispose(VALUE self);
VALUE rb_Sprite_Disposed(VALUE self);
VALUE rb_Sprite_setBitmap(VALUE self, VALUE bitmap);
VALUE rb_Sprite_getBitmap(VALUE self);
VALUE rb_Sprite_setX(VALUE self, VALUE val);
VALUE rb_Sprite_getX(VALUE self);
VALUE rb_Sprite_setY(VALUE self, VALUE val);
VALUE rb_Sprite_getY(VALUE self);
VALUE rb_Sprite_setZ(VALUE self, VALUE val);
VALUE rb_Sprite_getZ(VALUE self);
VALUE rb_Sprite_setOX(VALUE self, VALUE val);
VALUE rb_Sprite_getOX(VALUE self);
VALUE rb_Sprite_setOY(VALUE self, VALUE val);
VALUE rb_Sprite_getOY(VALUE self);
VALUE rb_Sprite_setVisible(VALUE self, VALUE val);
VALUE rb_Sprite_getVisible(VALUE self);
VALUE rb_Sprite_setAngle(VALUE self, VALUE val);
VALUE rb_Sprite_getAngle(VALUE self);
VALUE rb_Sprite_setZoomX(VALUE self, VALUE val);
VALUE rb_Sprite_getZoomX(VALUE self);
VALUE rb_Sprite_setZoomY(VALUE self, VALUE val);
VALUE rb_Sprite_getZoomY(VALUE self);
VALUE rb_Sprite_setPosition(VALUE self, VALUE x, VALUE y);
VALUE rb_Sprite_setOrigin(VALUE self, VALUE x, VALUE y);
VALUE rb_Sprite_setZoom(VALUE self, VALUE zoom);
VALUE rb_Sprite_setOpacity(VALUE self, VALUE val);
VALUE rb_Sprite_getOpacity(VALUE self);
VALUE rb_Sprite_getRect(VALUE self);
VALUE rb_Sprite_setRect(VALUE self, VALUE val);
VALUE rb_Sprite_DisposeFromViewport(VALUE self);
VALUE rb_Sprite_Viewport(VALUE self);
#endif