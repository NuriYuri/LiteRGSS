#include "LiteRGSS.h"

VALUE rb_mFonts = Qnil;

ID rb_Fonts_ivSize = Qnil;
ID rb_Fonts_ivFColor = Qnil;
ID rb_Fonts_ivOColor = Qnil;
ID rb_Fonts_ivSColor = Qnil;

std::vector<unsigned int> rb_Fonts_Size_Tbl;
std::vector<sf::Font> rb_Fonts_font_tbl;

void Init_Fonts()
{
	rb_mFonts = rb_define_module_under(rb_mLiteRGSS, "Fonts");

	rb_define_module_function(rb_mFonts, "load_font", _rbf rb_Fonts_load_font, 2);
	rb_define_module_function(rb_mFonts, "set_default_size", _rbf rb_Fonts_set_default_size, 2);
	rb_define_module_function(rb_mFonts, "define_fill_color", _rbf rb_Fonts_define_fill_color, 2);
	rb_define_module_function(rb_mFonts, "define_outline_color", _rbf rb_Fonts_define_outline_color, 2);
	rb_define_module_function(rb_mFonts, "define_shadow_color", _rbf rb_Fonts_define_shadow_color, 2);
	rb_define_module_function(rb_mFonts, "get_default_size", _rbf rb_Fonts_get_default_size, 1);
	rb_define_module_function(rb_mFonts, "get_fill_color", _rbf rb_Fonts_get_fill_color, 1);
	rb_define_module_function(rb_mFonts, "get_outline_color", _rbf rb_Fonts_get_outline_color, 1);
	rb_define_module_function(rb_mFonts, "get_shadow_color", _rbf rb_Fonts_get_shadow_color, 1);

	rb_Fonts_ivSize = rb_intern("@default_size");
	rb_Fonts_ivFColor = rb_intern("@fill_color");
	rb_Fonts_ivOColor = rb_intern("@outline_color");
	rb_Fonts_ivSColor = rb_intern("@shadow_color");

	rb_ivar_set(rb_mFonts, rb_Fonts_ivSize, rb_ary_new());
	rb_ivar_set(rb_mFonts, rb_Fonts_ivFColor, rb_ary_new());
	rb_ivar_set(rb_mFonts, rb_Fonts_ivOColor, rb_ary_new());
	rb_ivar_set(rb_mFonts, rb_Fonts_ivSColor, rb_ary_new());
}


VALUE rb_Fonts_load_font(VALUE self, VALUE id, VALUE str)
{
	unsigned long position = normalize_long(rb_num2long(id), 0, 255);
	rb_check_type(str, T_STRING);
	while(rb_Fonts_font_tbl.size() <= position)
	{
		rb_Fonts_font_tbl.push_back(sf::Font());
	}
	rb_Fonts_font_tbl[position].loadFromFile(RSTRING_PTR(str));
	return self;
}

sf::Font& rb_Fonts_get_font(unsigned long id)
{
	if(rb_Fonts_font_tbl.size() <= id)
	{
		rb_raise(rb_eRGSSError, "Unable to fetch font n°%d.", static_cast<int>(id));
	}
	return rb_Fonts_font_tbl[id];
}

VALUE rb_Fonts_set_default_size(VALUE self, VALUE id, VALUE size)
{
	unsigned long position = rb_num2long(id);
	while(rb_Fonts_Size_Tbl.size() <= position)
		rb_Fonts_Size_Tbl.push_back(16);
	rb_Fonts_Size_Tbl[position] = static_cast<unsigned int>(rb_num2long(size));
	return self;
}

VALUE rb_Fonts_define_fill_color(VALUE self, VALUE id, VALUE color)
{
	if(rb_obj_is_kind_of(color, rb_cColor) != Qtrue)
	{
		rb_raise(rb_eTypeError, "Expected Color got %s.", RSTRING_PTR(rb_class_name(CLASS_OF(color))));
		return self;
	}
	rb_ary_store(rb_ivar_get(rb_mFonts, rb_Fonts_ivFColor), rb_num2long(id), color);
	return self;
}

VALUE rb_Fonts_define_outline_color(VALUE self, VALUE id, VALUE color)
{
	if(rb_obj_is_kind_of(color, rb_cColor) != Qtrue)
	{
		rb_raise(rb_eTypeError, "Expected Color got %s.", RSTRING_PTR(rb_class_name(CLASS_OF(color))));
		return self;
	}
	rb_ary_store(rb_ivar_get(rb_mFonts, rb_Fonts_ivOColor), rb_num2long(id), color);
	return self;
}

VALUE rb_Fonts_define_shadow_color(VALUE self, VALUE id, VALUE color)
{
	if(rb_obj_is_kind_of(color, rb_cColor) != Qtrue)
	{
		rb_raise(rb_eTypeError, "Expected Color got %s.", RSTRING_PTR(rb_class_name(CLASS_OF(color))));
		return self;
	}
	rb_ary_store(rb_ivar_get(rb_mFonts, rb_Fonts_ivSColor), rb_num2long(id), color);
	return self;
}


VALUE rb_Fonts_get_default_size(VALUE self, VALUE id)
{
	long lid = rb_num2long(id);
	if(lid >= rb_Fonts_Size_Tbl.size())
	{
		return LONG2FIX(16);
	}
	return rb_int2inum(static_cast<long>(rb_Fonts_Size_Tbl[lid]));
}

VALUE rb_Fonts_get_fill_color(VALUE self, VALUE id)
{
	return rb_ary_aref(1, &id, rb_ivar_get(rb_mFonts, rb_Fonts_ivFColor));
}

VALUE rb_Fonts_get_outline_color(VALUE self, VALUE id)
{
	return rb_ary_aref(1, &id, rb_ivar_get(rb_mFonts, rb_Fonts_ivOColor));
}

VALUE rb_Fonts_get_shadow_color(VALUE self, VALUE id)
{
	return rb_ary_aref(1, &id, rb_ivar_get(rb_mFonts, rb_Fonts_ivSColor));
}