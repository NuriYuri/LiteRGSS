#ifndef L_TEXT_HEADER
#define L_TEXT_HEADER
#include "CText_Element.h"
#include "Graphics.h"

VALUE rb_Text_Initialize(int argc, VALUE* argv, VALUE self);
VALUE rb_Text_Copy(VALUE self);
VALUE rb_Text_Dispose(VALUE self);
VALUE rb_Text_Disposed(VALUE self);
VALUE rb_Text_setPosition(VALUE self, VALUE x, VALUE y);
VALUE rb_Text_get_x(VALUE self);
VALUE rb_Text_set_x(VALUE self, VALUE val);
VALUE rb_Text_get_y(VALUE self);
VALUE rb_Text_set_y(VALUE self, VALUE val);
VALUE rb_Text_get_width(VALUE self);
VALUE rb_Text_set_width(VALUE self, VALUE val);
VALUE rb_Text_get_height(VALUE self);
VALUE rb_Text_set_height(VALUE self, VALUE val);
VALUE rb_Text_get_fill_color(VALUE self);
VALUE rb_Text_set_fill_color(VALUE self, VALUE val);
VALUE rb_Text_get_outline_color(VALUE self);
VALUE rb_Text_set_outline_color(VALUE self, VALUE val);
VALUE rb_Text_get_outline_thickness(VALUE self);
VALUE rb_Text_set_outline_thickness(VALUE self, VALUE val);
VALUE rb_Text_load_color(VALUE self, VALUE id);
VALUE rb_Text_get_align(VALUE self);
VALUE rb_Text_set_align(VALUE self, VALUE val);
VALUE rb_Text_get_size(VALUE self);
VALUE rb_Text_set_size(VALUE self, VALUE val);
VALUE rb_Text_set_Text(VALUE self, VALUE str);
VALUE rb_Text_get_Text(VALUE self);
VALUE rb_Text_DisposeFromViewport(VALUE self);
VALUE rb_Text_UpdateI(CText_Element* text);
VALUE rb_Text_get_visible(VALUE self);
VALUE rb_Text_set_visible(VALUE self, VALUE val);
VALUE rb_Text_set_num_char(VALUE self, VALUE val);
VALUE rb_Text_get_num_char(VALUE self);
VALUE rb_Text_set_draw_shadow(VALUE self, VALUE val);
VALUE rb_Text_get_draw_shadow(VALUE self);
VALUE rb_Text_getRealWidth(VALUE self);
VALUE rb_Text_getOpacity(VALUE self);
VALUE rb_Text_setOpacity(VALUE self, VALUE val);
#endif