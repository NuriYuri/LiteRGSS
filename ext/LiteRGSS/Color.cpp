#include "ruby.h"
#include "LiteRGSS.h"
#include "Color.h"
#include <SFML/Graphics.hpp>

VALUE rb_cColor = Qnil;

#define COLOR_PROTECT if(RDATA(self)->data == nullptr) \
{\
    rb_raise(rb_eRGSSError, "Freed Color."); \
    return self; \
}

#define GET_COLOR sf::Color* color; \
    Data_Get_Struct(self, sf::Color, color); \
    COLOR_PROTECT

void rb_Color_Free(void* data)
{
    sf::Color* color = reinterpret_cast<sf::Color*>(data);
    if(color)
    {
        delete color;
    }
}

VALUE rb_Color_Alloc(VALUE klass)
{
    sf::Color* color = new sf::Color();
    return Data_Wrap_Struct(klass, NULL, rb_Color_Free, color);
}

void Init_Color()
{
    rb_cColor = rb_define_class_under(rb_mLiteRGSS, "Color", rb_cObject);
    rb_define_alloc_func(rb_cColor, rb_Color_Alloc);

    rb_define_method(rb_cColor, "initialize", _rbf rb_Color_Initialize, -1);
    rb_define_method(rb_cColor, "set", _rbf rb_Color_Initialize, -1);
    rb_define_method(rb_cColor, "initialize_copy", _rbf rb_Color_InitializeCopy, 1);
    rb_define_method(rb_cColor, "red", _rbf rb_Color_getRed, 0);
    rb_define_method(rb_cColor, "red=", _rbf rb_Color_setRed, 1);
    rb_define_method(rb_cColor, "green", _rbf rb_Color_getGreen, 0);
    rb_define_method(rb_cColor, "green=", _rbf rb_Color_setGreen, 1);
    rb_define_method(rb_cColor, "blue", _rbf rb_Color_getBlue, 0);
    rb_define_method(rb_cColor, "blue=", _rbf rb_Color_setBlue, 1);
    rb_define_method(rb_cColor, "alpha", _rbf rb_Color_getAlpha, 0);
    rb_define_method(rb_cColor, "alpha=", _rbf rb_Color_setAlpha, 1);
}

VALUE rb_Color_Initialize(int argc, VALUE* argv, VALUE self)
{
    VALUE red, green, blue, alpha;
    rb_scan_args(argc, argv, "13", &red, &green, &blue, &alpha);
    GET_COLOR
    if(RTEST(red))
        color->r = normalize_long(rb_num2long(red), 0, 255);
    if(RTEST(green))
        color->g = normalize_long(rb_num2long(green), 0, 255);
    if(RTEST(blue))
        color->b = normalize_long(rb_num2long(blue), 0, 255);
    if(RTEST(alpha))
        color->a = normalize_long(rb_num2long(alpha), 0, 255);
    return self;
}

VALUE rb_Color_InitializeCopy(VALUE self, VALUE original)
{
    GET_COLOR
    sf::Color* coloro;
    Data_Get_Struct(original, sf::Color, coloro);
    if(RDATA(original)->data == nullptr)
        rb_raise(rb_eRGSSError, "Freed Color.");
    color->r = coloro->r;
    color->g = coloro->g;
    color->b = coloro->b;
    color->a = coloro->a;
    return self;
}

VALUE rb_Color_getRed(VALUE self)
{
    GET_COLOR
    return rb_int2inum(color->r);
}

VALUE rb_Color_setRed(VALUE self, VALUE red)
{
    GET_COLOR
    color->r = normalize_long(rb_num2long(red), 0, 255);
    return self;
}

VALUE rb_Color_getGreen(VALUE self)
{
    GET_COLOR
    return rb_int2inum(color->g);
}

VALUE rb_Color_setGreen(VALUE self, VALUE red)
{
    GET_COLOR
    color->g = normalize_long(rb_num2long(red), 0, 255);
    return self;
}

VALUE rb_Color_getBlue(VALUE self)
{
    GET_COLOR
    return rb_int2inum(color->b);
}

VALUE rb_Color_setBlue(VALUE self, VALUE red)
{
    GET_COLOR
    color->b = normalize_long(rb_num2long(red), 0, 255);
    return self;
}

VALUE rb_Color_getAlpha(VALUE self)
{
    GET_COLOR
    return rb_int2inum(color->a);
}

VALUE rb_Color_setAlpha(VALUE self, VALUE red)
{
    GET_COLOR
    color->a = normalize_long(rb_num2long(red), 0, 255);
    return self;
}
