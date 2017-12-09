#include "ruby.h"
#include "test.h"

VALUE moduleTest = Qnil;

extern "C" {
void Init_module_Test()
{
  moduleTest = rb_define_module("Test");
  rb_define_singleton_method(moduleTest, "func", (VALUE (*)(...))moduleTestFunc, 0);
}
}
