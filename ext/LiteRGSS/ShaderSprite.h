#ifndef L_SHADER_SPRITE_HEADER
#define L_SHADER_SPRITE_HEADER
#include "Graphics.h"
#include "rbAdapter.h"

VALUE rb_ShaderSprite_getShader(VALUE self);
VALUE rb_ShaderSprite_setShader(VALUE self, VALUE shader);

namespace rb {
	template<>
	void Mark<CShaderSprite_Element>(CShaderSprite_Element* sprite);
}
#endif