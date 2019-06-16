#include "../LiteRGSS/ruby_common.h"
#include "../LiteRGSS/rbAdapter.h"

#include "../LiteRGSS/CShaderFactory.h"
#include "ShaderFactory_test.h"

static std::unique_ptr<sf::Shader> ShaderFactoryTest_shader;
static sf::Shader* ShaderFactoryTest_shaderRaw;

void rb_ShaderFactoryTest_Free(void* data) {
  auto* typedData = reinterpret_cast<CShaderFactory*>(data);
  delete typedData;
}

VALUE rb_ShaderFactoryTest_initialize(int argc, VALUE* argv, VALUE self) {
  return self;
}

VALUE rb_ShaderFactoryTest_Alloc(VALUE klass) {
    return Data_Wrap_Struct(klass, NULL, rb_ShaderFactoryTest_Free, new CShaderFactory());
}

VALUE rb_ShaderFactoryTest_enabled(VALUE self) {
  auto& wrapper = rb::Get<CShaderFactory>(self);
  return wrapper.areEnabled() ? Qtrue : Qfalse;
}

VALUE rb_ShaderFactoryTest_disable(VALUE self) {
  auto& wrapper = rb::Get<CShaderFactory>(self);
  wrapper.toggle(false);
  return self;
}

VALUE rb_ShaderFactoryTest_create(VALUE self) {
  auto& wrapper = rb::Get<CShaderFactory>(self);
  ShaderFactoryTest_shader = wrapper.createUnique();
  ShaderFactoryTest_shaderRaw = wrapper.createNew();
  return self;
}

VALUE rb_ShaderFactoryTest_shader_exists(VALUE self) {
  return ShaderFactoryTest_shader != nullptr && ShaderFactoryTest_shaderRaw != nullptr ? Qtrue : Qfalse;
}

ID rb_cShaderFactoryTest = Qnil;

void InitShaderFactoryTest() {
    rb_cShaderFactoryTest = rb_define_class("ShaderFactory", rb_cObject);
    rb_define_alloc_func(rb_cShaderFactoryTest, rb_ShaderFactoryTest_Alloc);
    rb_define_method(rb_cShaderFactoryTest, "initialize", _rbf rb_ShaderFactoryTest_initialize, -1);
    rb_define_method(rb_cShaderFactoryTest, "disable", _rbf rb_ShaderFactoryTest_disable, 0);
    rb_define_method(rb_cShaderFactoryTest, "enabled?", _rbf rb_ShaderFactoryTest_enabled, 0);
    rb_define_method(rb_cShaderFactoryTest, "create", _rbf rb_ShaderFactoryTest_create, 0);
    rb_define_method(rb_cShaderFactoryTest, "shader_exists?", _rbf rb_ShaderFactoryTest_shader_exists, 0);
}
