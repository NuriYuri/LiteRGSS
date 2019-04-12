#include <iostream>
#include <ruby.h>

#include "ruby_common.h"
#include "CDrawableStack.h"

extern VALUE rb_mLiteRGSS;
extern VALUE rb_mConfig;
extern VALUE rb_eRGSSError;
extern ID rb_iElementTable;

ID rb_cToto = Qnil;

void rb_Toto_Free(void* data) {
}

VALUE rb_Toto_initialize(int argc, VALUE* argv, VALUE self) {
  return self;
}

VALUE rb_Toto_Alloc(VALUE klass) {
    auto toto = new int();
    auto result = Data_Wrap_Struct(klass, NULL, rb_Toto_Free, toto);
    return result;
}

VALUE rb_Toto_titi(VALUE self) {
  CDrawableStack dw(self);
    return self;
}

void Init_Toto()
{
    rb_cToto = rb_define_class("Toto", rb_cObject);
    rb_define_alloc_func(rb_cToto, rb_Toto_Alloc);
    rb_define_method(rb_cToto, "initialize", _rbf rb_Toto_initialize, -1);
    rb_define_method(rb_cToto, "titi", _rbf rb_Toto_titi, 0);
}



extern "C" {
  void Init_LiteRGSS_test() {
    

    rb_mLiteRGSS = rb_define_module("LiteRGSS_test");
    rb_iElementTable = rb_intern("@__elementtable");

    Init_Toto();
  }
}
