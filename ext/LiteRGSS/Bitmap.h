#ifndef L_BITMAP_HEADER
#define L_BITMAP_HEADER
#include "ruby.h"
#include <SFML/Graphics.hpp>

VALUE rb_Bitmap_Initialize(int argc, VALUE *argv, VALUE self);
VALUE rb_Bitmap_Initialize_Copy(VALUE self, VALUE other);
VALUE rb_Bitmap_Dispose(VALUE self);
VALUE rb_Bitmap_Width(VALUE self);
VALUE rb_Bitmap_Height(VALUE self);
VALUE rb_Bitmap_Rect(VALUE self);
VALUE rb_Bitmap_Update(VALUE self);
VALUE rb_Bitmap_blt(VALUE self, VALUE x, VALUE y, VALUE src_bitmap, VALUE rect);
VALUE rb_Bitmap_clear_rect(VALUE self, VALUE x, VALUE y, VALUE width, VALUE height);
VALUE rb_Bitmap_fill_rect(VALUE self, VALUE x, VALUE y, VALUE width, VALUE height, VALUE color);
VALUE rb_Bitmap_toPNG(VALUE self);
VALUE rb_Bitmap_toPNG_file(VALUE self, VALUE filename);
sf::Texture& rb_Bitmap_getTexture(VALUE self);

#endif