#ifndef L_BITMAP_HEADER
#define L_BITMAP_HEADER
#include "ruby.h"
#include <SFML/Graphics.hpp>

VALUE rb_Bitmap_Initialize(int argc, VALUE *argv, VALUE self);
VALUE rb_Bitmap_Initialize_Copy(VALUE self, VALUE other);
VALUE rb_Bitmap_Dispose(VALUE self);
VALUE rb_Bitmap_Disposed(VALUE self);

#endif