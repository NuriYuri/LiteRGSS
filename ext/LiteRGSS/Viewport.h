#ifndef L_VIEWPORT_H
#define L_VIEWPORT_H

VALUE rb_Viewport_Initialize(int argc, VALUE* argv, VALUE self);
VALUE rb_Viewport_Copy(VALUE self);
VALUE rb_Viewport_getOX(VALUE self);
VALUE rb_Viewport_setOX(VALUE self, VALUE val);
VALUE rb_Viewport_getOY(VALUE self);
VALUE rb_Viewport_setOY(VALUE self, VALUE val);
VALUE rb_Viewport_getRect(VALUE self);
VALUE rb_Viewport_setRect(VALUE self, VALUE val);
VALUE rb_Viewport_Dispose(VALUE self);
VALUE rb_Viewport_Disposed(VALUE self);


void Viewport_SetView(CViewport_Element* viewport, long x, long y, long width, long height);
#endif