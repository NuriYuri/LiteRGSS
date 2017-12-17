#include "ruby.h"
#include "LiteRGSS.h"

VALUE rb_mLiteRGSS = Qnil;
VALUE rb_mConfig = Qnil;
VALUE rb_eRGSSError = Qnil;
ID rb_iElementTable = Qnil;

extern "C" {
  void Init_LiteRGSS() {
    rb_mLiteRGSS = rb_define_module("LiteRGSS");
    rb_mConfig = rb_define_module_under(rb_mLiteRGSS, "Config");
    rb_eRGSSError = rb_define_class_under(rb_mLiteRGSS, "Error",rb_eStandardError);
    rb_iElementTable = rb_intern("@__elementtable");

    Init_Graphics();
    Init_Bitmap();
    Init_Sprite();
    Init_Rect();
    Init_Viewport();
    Init_Color();
    Init_Tone();
    Init_Fonts();
    Init_Text();
  }
}
