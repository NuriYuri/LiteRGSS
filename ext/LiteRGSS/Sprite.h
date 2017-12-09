#ifndef L_BITMAP_H
#define L_BITMAP_H
#include "Graphics.h"

VALUE rb_Sprite_Initialize(int argc, VALUE* argv, VALUE self);
VALUE rb_Sprite_Copy(VALUE self);
VALUE rb_Sprite_Dispose(VALUE self);
VALUE rb_Sprite_setBitmap(VALUE self, VALUE bitmap);

#endif