#ifndef L_RECT_HEADER
#define L_RECT_HEADER

VALUE rb_Rect_initialize(int argc, VALUE* argv, VALUE self);
VALUE rb_Rect_initialize_copy(VALUE self, VALUE other);
VALUE rb_Rect_set(int argc, VALUE* argv, VALUE self);
VALUE rb_Rect_getX(VALUE self);
VALUE rb_Rect_setX(VALUE self, VALUE val);
VALUE rb_Rect_getY(VALUE self);
VALUE rb_Rect_setY(VALUE self, VALUE val);
VALUE rb_Rect_getWidth(VALUE self);
VALUE rb_Rect_setWidth(VALUE self, VALUE val);
VALUE rb_Rect_getHeight(VALUE self);
VALUE rb_Rect_setHeight(VALUE self, VALUE val);
VALUE rb_Rect_load(VALUE self, VALUE str);
VALUE rb_Rect_save(VALUE self, VALUE limit);
VALUE rb_Rect_eql(VALUE self, VALUE other);
VALUE rb_Rect_empty(VALUE self);
VALUE rb_Rect_to_s(VALUE self);

#endif