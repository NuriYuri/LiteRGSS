#include <cassert>
#include <iostream>
#include <ruby.h>

#include "../LiteRGSS/LiteRGSS.h"
#include "../LiteRGSS/ruby_common.h"
#include "../LiteRGSS/CDrawableStack.h"
#include "../LiteRGSS/rbAdapter.h"
#include "../LiteRGSS/CSprite_Element.h"
#include "../LiteRGSS/CGraphics.h"

#include "ShaderFactory_test.h"

extern VALUE rb_cSprite;
extern ID rb_cDrawable;

ID rb_cDrawableStackTest = Qnil;

class CGraphicsStackTest : public CGraphicsStack {
public:
    using CGraphicsStack::CGraphicsStack;
    
    auto& operator[](const std::size_t index) {
      return *(begin() + index);
    }
};

struct DrawableStackTest {
  std::unique_ptr<CGraphicsStackTest> stack;
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
    wrapper->stack = std::make_unique<CGraphicsStackTest>(result);
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

VALUE rb_DrawableStackTest_remove(VALUE self, VALUE val) {
  auto& wrapper = rb::Get<DrawableStackTest>(self);
  assert(rb_obj_is_kind_of(val, rb_cSprite) == Qtrue);
    
  auto& sprite = rb::Get<CSprite_Element>(val);
  CGraphics::Get().add(sprite);

  return self;
}

VALUE rb_DrawableStackTest_getIndex(VALUE self, VALUE val) {
  auto& wrapper = rb::Get<DrawableStackTest>(self);
  auto& element = (*wrapper.stack)[NUM2INT(val)];
  return INT2NUM(element->getDrawPriority());
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
    rb_define_method(rb_cDrawableStackTest, "remove", _rbf rb_DrawableStackTest_remove, 1);
    rb_define_method(rb_cDrawableStackTest, "getindex", _rbf rb_DrawableStackTest_getIndex, 1);
    rb_define_method(rb_cDrawableStackTest, "size", _rbf rb_DrawableStackTest_size, 0);
}

extern "C" {
  void Init_LiteRGSS_test() {
    Init_LiteRGSS();

    Init_DrawableStackTest();
    InitShaderFactoryTest();
  }
}
