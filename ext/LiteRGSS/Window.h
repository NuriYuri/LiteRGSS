#ifndef L_WINDOW_HEADER
#define L_WINDOW_HEADER

#include "Graphics.h"


VALUE rb_Window_Initialize(int argc, VALUE* argv, VALUE self);

VALUE rb_Window_setWindowSkin(VALUE self, VALUE val);
VALUE rb_Window_getWindowSkin(VALUE self);
VALUE rb_Window_setWidth(VALUE self, VALUE val);
VALUE rb_Window_getWidth(VALUE self);
VALUE rb_Window_setHeight(VALUE self, VALUE val);
VALUE rb_Window_getHeight(VALUE self);
VALUE rb_Window_setSize(VALUE self, VALUE x, VALUE y);
VALUE rb_Window_setWindowBuilder(VALUE self, VALUE val);
VALUE rb_Window_getWindowBuilder(VALUE self);
VALUE rb_Window_setX(VALUE self, VALUE val);
VALUE rb_Window_getX(VALUE self);
VALUE rb_Window_setY(VALUE self, VALUE val);
VALUE rb_Window_getY(VALUE self);
VALUE rb_Window_setPosition(VALUE self, VALUE x, VALUE y);
VALUE rb_Window_setZ(VALUE self, VALUE val);
VALUE rb_Window_getZ(VALUE self);
VALUE rb_Window_setOX(VALUE self, VALUE val);
VALUE rb_Window_getOX(VALUE self);
VALUE rb_Window_setOY(VALUE self, VALUE val);
VALUE rb_Window_getOY(VALUE self);
VALUE rb_Window_setOrigin(VALUE self, VALUE x, VALUE y);
VALUE rb_Window_getIndex(VALUE self);



VALUE rb_Window_Dispose(VALUE self);
VALUE rb_Window_Disposed(VALUE self);
VALUE rb_Window_Copy(VALUE self);
CWindow_Element* rb_Window_get_window(VALUE self);
void rb_Window_test_window(VALUE self);

#endif // !L_WINDOW_HEADER
