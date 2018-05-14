#include "LiteRGSS.h"

VALUE rb_mYuki = Qnil;

void Init_Yuki()
{
	rb_mYuki = rb_define_module("Yuki");
	rb_define_module_function(rb_mYuki, "get_clipboard", _rbf rb_Yuki_getClipboard, 0);
	rb_define_module_function(rb_mYuki, "set_clipboard", _rbf rb_Yuki_setClipboard, 1);
}

/* Clipboard */
VALUE rb_Yuki_getClipboard(VALUE self)
{
	sf::String str = sf::Clipboard::getString();
	if (str.getSize() == 0)
	{
		return Qnil;
	}
	return rb_utf8_str_new_cstr(reinterpret_cast<const char*>(str.toUtf8().c_str()));
}

VALUE rb_Yuki_setClipboard(VALUE self, VALUE val)
{
	rb_check_type(val, T_STRING);
	std::string stru8(RSTRING_PTR(val));
	sf::Clipboard::setString(sf::String::fromUtf8(stru8.begin(), stru8.end()));
	return self;
}