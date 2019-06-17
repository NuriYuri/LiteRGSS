#ifndef L_SHADER_HEADER
#define L_SHADER_HEADER

VALUE rb_Shader_isAvailable(VALUE self);
VALUE rb_Shader_isGeometryAvailable(VALUE self);
VALUE rb_Shader_loadFromMemory(int argc, VALUE *argv, VALUE self);
VALUE rb_Shader_Copy(VALUE self);
VALUE rb_Shader_setFloatUniform(VALUE self, VALUE name, VALUE uniform);
VALUE rb_Shader_setIntUniform(VALUE self, VALUE name, VALUE uniform);
VALUE rb_Shader_setBoolUniform(VALUE self, VALUE name, VALUE uniform);
VALUE rb_Shader_setTextureUniform(VALUE self, VALUE name, VALUE uniform);
VALUE rb_Shader_setMatrixUniform(VALUE self, VALUE name, VALUE uniform);
VALUE rb_Shader_setFloatArrayUniform(VALUE self, VALUE name, VALUE uniform);
#endif // !L_SHADER_HEADER
