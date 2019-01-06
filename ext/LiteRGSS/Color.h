#ifndef L_COLOR_H
#define L_COLOR_H

VALUE rb_Color_Initialize(int argc, VALUE* argv, VALUE self); // Also .set
VALUE rb_Color_InitializeCopy(VALUE self, VALUE original);
VALUE rb_Color_getRed(VALUE self);
VALUE rb_Color_setRed(VALUE self, VALUE red);
VALUE rb_Color_getGreen(VALUE self);
VALUE rb_Color_setGreen(VALUE self, VALUE red);
VALUE rb_Color_getBlue(VALUE self);
VALUE rb_Color_setBlue(VALUE self, VALUE red);
VALUE rb_Color_getAlpha(VALUE self);
VALUE rb_Color_setAlpha(VALUE self, VALUE red);
VALUE rb_Color_eql(VALUE self, VALUE other);
VALUE rb_Color_Load(VALUE self, VALUE str);
VALUE rb_Color_Save(VALUE self, VALUE limit);
VALUE rb_Color_to_s(VALUE self);

sf::Color* rb_Color_get_color(VALUE self);
void rb_Color_test_color(VALUE self);

#endif