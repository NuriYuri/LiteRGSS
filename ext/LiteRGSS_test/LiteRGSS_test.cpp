#include <cassert>
#include <iostream>
#include <ruby.h>

#include "../LiteRGSS/LiteRGSS.h"
#include "../LiteRGSS/ruby_common.h"
#include "../LiteRGSS/CDrawableStack.h"
#include "../LiteRGSS/rbAdapter.h"
#include "../LiteRGSS/CSprite_Element.h"

extern VALUE rb_cSprite;
extern ID rb_cDrawable;
extern ID rb_iElementTable;

ID rb_cDrawableStackTest = Qnil;

struct DrawableStackTest {
  std::unique_ptr<CGraphicsStack> stack;
};

void rb_DrawableStackTest_Free(void* data) {
  auto* typedData = reinterpret_cast<DrawableStackTest*>(data);
  delete typedData;
}

VALUE rb_DrawableStackTest_initialize(int argc, VALUE* argv, VALUE self) {
  return self;
}

VALUE rb_DrawableStackTest_Alloc(VALUE klass) {
    auto wrapper = new DrawableStackTest();
    auto result = Data_Wrap_Struct(klass, NULL, rb_DrawableStackTest_Free, wrapper);
    wrapper->stack = std::make_unique<CGraphicsStack>(result);
    return result;
}

VALUE rb_DrawableStackTest_syncstack(VALUE self) {
  auto& wrapper = rb::Get<DrawableStackTest>(self);
  wrapper.stack->syncStackCppFromRuby();
  return self;
}

VALUE rb_DrawableStackTest_add(VALUE self, VALUE val) {
  auto& wrapper = rb::Get<DrawableStackTest>(self);
  assert(rb_obj_is_kind_of(val, rb_cSprite) == Qtrue);
    
  auto& sprite = rb::Get<CSprite_Element>(val);
  wrapper.stack->add(sprite);

  return self;
}

VALUE rb_DrawableStackTest_size(VALUE self) {
  auto& wrapper = rb::Get<DrawableStackTest>(self);
  return INT2NUM(wrapper.stack->size());
}

extern "C" {
  void Init_LiteRGSS();
}

void Init_DrawableStackTest() {
    rb_cDrawableStackTest = rb_define_class("DrawableStack", rb_cObject);
    rb_define_alloc_func(rb_cDrawableStackTest, rb_DrawableStackTest_Alloc);
    rb_define_method(rb_cDrawableStackTest, "initialize", _rbf rb_DrawableStackTest_initialize, -1);
    rb_define_method(rb_cDrawableStackTest, "syncstack", _rbf rb_DrawableStackTest_syncstack, 0);
    rb_define_method(rb_cDrawableStackTest, "add", _rbf rb_DrawableStackTest_add, 1);
    rb_define_method(rb_cDrawableStackTest, "size", _rbf rb_DrawableStackTest_size, 0);
}

extern "C" {
  void Init_LiteRGSS_test() {
    Init_LiteRGSS();
    
    /*
    Init_DrawableDisposable();
    Init_Graphics();
    Init_Bitmap();
    Init_Image();
    Init_Sprite();
    */

    Init_DrawableStackTest();
  }
}
