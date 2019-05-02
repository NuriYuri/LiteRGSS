#ifndef L_VIEWPORT_H
#define L_VIEWPORT_H
#include "rbAdapter.h"

class CViewport_Element;
VALUE rb_Viewport_Initialize(int argc, VALUE* argv, VALUE self);
VALUE rb_Viewport_Copy(VALUE self);
VALUE rb_Viewport_getOX(VALUE self);
VALUE rb_Viewport_setOX(VALUE self, VALUE val);
VALUE rb_Viewport_getOY(VALUE self);
VALUE rb_Viewport_setOY(VALUE self, VALUE val);
VALUE rb_Viewport_getRect(VALUE self);
VALUE rb_Viewport_setRect(VALUE self, VALUE val);
VALUE rb_Viewport_Dispose(VALUE self);
VALUE rb_Viewport_getTone(VALUE self);
VALUE rb_Viewport_setTone(VALUE self, VALUE val);
VALUE rb_Viewport_getColor(VALUE self);
VALUE rb_Viewport_setColor(VALUE self, VALUE val);
VALUE rb_Viewport_getVisible(VALUE self);
VALUE rb_Viewport_setVisible(VALUE self, VALUE val);
VALUE rb_Viewport_getAngle(VALUE self);
VALUE rb_Viewport_setAngle(VALUE self, VALUE val);
VALUE rb_Viewport_getZoom(VALUE self);
VALUE rb_Viewport_setZoom(VALUE self, VALUE val);
VALUE rb_Viewport_Update(VALUE self);
VALUE rb_Viewport_getZ(VALUE self);
VALUE rb_Viewport_setZ(VALUE self, VALUE val);
VALUE rb_Viewport_getRenderState(VALUE self);
VALUE rb_Viewport_setRenderState(VALUE self, VALUE val);
VALUE rb_Viewport_ReloadStack(VALUE self);
VALUE rb_Viewport_Index(VALUE self);

void Viewport_SetView(CViewport_Element& viewport, long x, long y, long width, long height);

namespace rb {
	template<>
	void Mark<CViewport_Element>(CViewport_Element* viewport);
}
#endif