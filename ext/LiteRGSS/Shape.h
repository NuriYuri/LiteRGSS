#ifndef L_SHAPE_HEADER
#define L_SHAPE_HEADER
#include "Graphics.h"
#include "utils/rbAdapter.h"

VALUE rb_Shape_Initialize(int argc, VALUE* argv, VALUE self);
VALUE rb_Shape_Dispose(VALUE self);
VALUE rb_Shape_getBitmap(VALUE self);
VALUE rb_Shape_setBitmap(VALUE self, VALUE bitmap);
VALUE rb_Shape_getRect(VALUE self);
VALUE rb_Shape_setRect(VALUE self, VALUE rect);
VALUE rb_Shape_getX(VALUE self);
VALUE rb_Shape_setX(VALUE self, VALUE val);
VALUE rb_Shape_getY(VALUE self);
VALUE rb_Shape_setY(VALUE self, VALUE val);
VALUE rb_Shape_setPosition(VALUE self, VALUE x, VALUE y);
VALUE rb_Shape_getZ(VALUE self);
VALUE rb_Shape_setZ(VALUE self, VALUE val);
VALUE rb_Shape_getOX(VALUE self);
VALUE rb_Shape_setOX(VALUE self, VALUE val);
VALUE rb_Shape_getOY(VALUE self);
VALUE rb_Shape_setOY(VALUE self, VALUE val);
VALUE rb_Shape_setOrigin(VALUE self, VALUE ox, VALUE oy);
VALUE rb_Shape_getAngle(VALUE self);
VALUE rb_Shape_setAngle(VALUE self, VALUE val);
VALUE rb_Shape_getZoomX(VALUE self);
VALUE rb_Shape_setZoomX(VALUE self, VALUE val);
VALUE rb_Shape_getZoomY(VALUE self);
VALUE rb_Shape_setZoomY(VALUE self, VALUE val);
VALUE rb_Shape_setZoom(VALUE self, VALUE val);
VALUE rb_Shape_getIndex(VALUE self);
VALUE rb_Shape_getViewport(VALUE self);
VALUE rb_Shape_getVisible(VALUE self);
VALUE rb_Shape_setVisible(VALUE self, VALUE val);
VALUE rb_Shape_getPointCount(VALUE self);
VALUE rb_Shape_setPointCount(VALUE self, VALUE val);
VALUE rb_Shape_getPoint(VALUE self, VALUE index);
VALUE rb_Shape_setPoint(VALUE self, VALUE index, VALUE x, VALUE y);
VALUE rb_Shape_getColor(VALUE self);
VALUE rb_Shape_setColor(VALUE self, VALUE val);
VALUE rb_Shape_getOutlineColor(VALUE self);
VALUE rb_Shape_setOutlineColor(VALUE self, VALUE val);
VALUE rb_Shape_getOutlineThickness(VALUE self);
VALUE rb_Shape_setOutlineThickness(VALUE self, VALUE val);
VALUE rb_Shape_getRadius(VALUE self);
VALUE rb_Shape_setRadius(VALUE self, VALUE val);
VALUE rb_Shape_getType(VALUE self);
VALUE rb_Shape_getWidth(VALUE self);
VALUE rb_Shape_setWidth(VALUE self, VALUE val);
VALUE rb_Shape_getHeight(VALUE self);
VALUE rb_Shape_setHeight(VALUE self, VALUE val);
VALUE rb_Shape_getShader(VALUE self);
VALUE rb_Shape_setShader(VALUE self, VALUE shader);

VALUE rb_Shape_Copy(VALUE self);

VALUE rb_Shape_DisposeFromViewport(VALUE self);

namespace rb {
    template<>
    void Mark<CShape_Element>(CShape_Element* shape);
}
#endif