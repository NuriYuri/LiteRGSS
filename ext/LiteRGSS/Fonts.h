#ifndef L_FONTS_HEADER
#define L_FONTS_HEADER

extern ID rb_Fonts_ivSize;
extern ID rb_Fonts_ivFColor;
extern ID rb_Fonts_ivOColor;
extern ID rb_Fonts_ivSColor;

VALUE rb_Fonts_load_font(VALUE self, VALUE id, VALUE str);
sf::Font& rb_Fonts_get_font(unsigned long id);
VALUE rb_Fonts_set_default_size(VALUE self, VALUE id, VALUE size);
VALUE rb_Fonts_define_fill_color(VALUE self, VALUE id, VALUE color);
VALUE rb_Fonts_define_outline_color(VALUE self, VALUE id, VALUE color);
VALUE rb_Fonts_define_shadow_color(VALUE self, VALUE id, VALUE color);
VALUE rb_Fonts_get_default_size(VALUE self, VALUE id);
VALUE rb_Fonts_get_fill_color(VALUE self, VALUE id);
VALUE rb_Fonts_get_outline_color(VALUE self, VALUE id);
VALUE rb_Fonts_get_shadow_color(VALUE self, VALUE id);

#endif