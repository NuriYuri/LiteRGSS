#include "LiteRGSS.h"
#include "CTone_Element.h"

VALUE rb_cShader = Qnil;

#define SHADER_PROTECT if(RDATA(self)->data == nullptr) \
{\
    rb_raise(rb_eRGSSError, "Disposed Shader."); \
    return self; \
}

#define GET_SHADER sf::RenderStates* render_state; \
    Data_Get_Struct(self, sf::RenderStates, render_state); \
    SHADER_PROTECT \
	sf::Shader* shader = const_cast<sf::Shader*>(render_state->shader);

void rb_Shader_Free(void* data)
{
	sf::RenderStates* render_state = reinterpret_cast<sf::RenderStates*>(data);
	if (render_state != nullptr)
	{
		if (render_state->shader != nullptr)
		{
			delete render_state->shader;
		}
		delete render_state;
	}
}

VALUE rb_Shader_Alloc(VALUE klass)
{
	sf::Shader* shader = new sf::Shader();
	sf::RenderStates* render_state = new sf::RenderStates(shader);
	return Data_Wrap_Struct(klass, NULL, rb_Shader_Free, render_state);
}

void Init_Shader()
{
	rb_cShader = rb_define_class_under(rb_mLiteRGSS, "Shader", rb_cBlendMode); //<--- Should inherit of rb_cBlendMode
	rb_define_alloc_func(rb_cShader, rb_Shader_Alloc);
	rb_define_method(rb_cShader, "initialize", _rbf rb_Shader_loadFromMemory, -1);
	rb_define_method(rb_cShader, "load", _rbf rb_Shader_loadFromMemory, -1);
	rb_define_method(rb_cShader, "set_float_uniform", _rbf rb_Shader_setFloatUniform, 2);
	rb_define_method(rb_cShader, "set_int_uniform", _rbf rb_Shader_setIntUniform, 2);
	rb_define_method(rb_cShader, "set_bool_uniform", _rbf rb_Shader_setBoolUniform, 2);
	rb_define_method(rb_cShader, "set_texture_uniform", _rbf rb_Shader_setTextureUniform, 2);
	rb_define_method(rb_cShader, "set_matrix_uniform", _rbf rb_Shader_setMatrixUniform, 2);
	rb_define_method(rb_cShader, "set_float_array_uniform", _rbf rb_Shader_setFloatArrayUniform, 2);

	rb_define_method(rb_cShader, "clone", _rbf rb_Shader_Copy, 0);
	rb_define_method(rb_cShader, "dup", _rbf rb_Shader_Copy, 0);
	
	rb_define_singleton_method(rb_cShader, "is_geometry_available?", _rbf rb_Shader_isGeometryAvailable, 0);

	rb_define_const(rb_cShader, "Fragment", LONG2FIX(sf::Shader::Type::Fragment));
	rb_define_const(rb_cShader, "Vertex", LONG2FIX(sf::Shader::Type::Vertex));
	rb_define_const(rb_cShader, "Geometry", LONG2FIX(sf::Shader::Type::Geometry));
}

VALUE rb_Shader_loadFromMemory(int argc, VALUE *argv, VALUE self)
{
	VALUE arg1, arg2, arg3;
	GET_SHADER
	rb_scan_args(argc, argv, "12", &arg1, &arg2, &arg3);
	rb_check_type(arg1, T_STRING);
	if (RTEST(arg2))
	{
		if (rb_obj_is_kind_of(arg2, rb_cInteger) == Qtrue)
		{
			shader->loadFromMemory(rb_string_value_cstr(&arg1), static_cast<sf::Shader::Type>(NUM2LONG(arg2)));
		}
		else
		{
			rb_check_type(arg2, T_STRING);
			if (argc == 2)
			{
				shader->loadFromMemory(rb_string_value_cstr(&arg1), rb_string_value_cstr(&arg2));
			}
			else if (argc == 3)
			{
				rb_check_type(arg3, T_STRING);
				shader->loadFromMemory(rb_string_value_cstr(&arg1), rb_string_value_cstr(&arg2), rb_string_value_cstr(&arg3));
			}
		}
	}
	else
	{
		shader->loadFromMemory(rb_string_value_cstr(&arg1), sf::Shader::Type::Fragment);
	}
	//rb_obj_is_kind_of(other, rb_cBitmap)
	return self;
}

VALUE rb_Shader_setFloatUniform(VALUE self, VALUE name, VALUE uniform)
{
	GET_SHADER;
	rb_check_type(name, T_STRING);
	if (rb_obj_is_kind_of(uniform, rb_cArray) == Qtrue)
	{
		long arylen = RARRAY_LEN(uniform);
		if (arylen == 4)
		{
			sf::Glsl::Vec4 vect4(NUM2DBL(rb_ary_entry(uniform, 0)), NUM2DBL(rb_ary_entry(uniform, 1)), NUM2DBL(rb_ary_entry(uniform, 2)), NUM2DBL(rb_ary_entry(uniform, 3)));
			shader->setUniform(rb_string_value_cstr(&name), vect4);
		}
		else if (arylen == 3)
		{
			sf::Glsl::Vec3 vect3(NUM2DBL(rb_ary_entry(uniform, 0)), NUM2DBL(rb_ary_entry(uniform, 1)), NUM2DBL(rb_ary_entry(uniform, 2)));
			shader->setUniform(rb_string_value_cstr(&name), vect3);
		}
		else if(arylen == 2)
		{
			sf::Glsl::Vec2 vect2(NUM2DBL(rb_ary_entry(uniform, 0)), NUM2DBL(rb_ary_entry(uniform, 1)));
			shader->setUniform(rb_string_value_cstr(&name), vect2);
		}
	}
	else if (rb_obj_is_kind_of(uniform, rb_cColor) == Qtrue)
	{
		sf::Color* color;
		Data_Get_Struct(uniform, sf::Color, color);
		if (color != nullptr)
		{
			sf::Glsl::Vec4 vect4(color->r / 255.0, color->g / 255.0, color->b / 255.0, color->a / 255.0);
			shader->setUniform(rb_string_value_cstr(&name), vect4);
		}
	}
	else if (rb_obj_is_kind_of(uniform, rb_cTone) == Qtrue)
	{
		CTone_Element* tone;
		Data_Get_Struct(uniform, CTone_Element, tone);
		if(tone != nullptr)
			shader->setUniform(rb_string_value_cstr(&name), *tone->getTone());
	}
	else
	{
		shader->setUniform(rb_string_value_cstr(&name), static_cast<float>(NUM2DBL(uniform)));
	}
	return self;
}

VALUE rb_Shader_setIntUniform(VALUE self, VALUE name, VALUE uniform)
{
	GET_SHADER;
	rb_check_type(name, T_STRING);
	if (rb_obj_is_kind_of(uniform, rb_cArray) == Qtrue)
	{
		long arylen = RARRAY_LEN(uniform);
		if (arylen == 4)
		{
			sf::Glsl::Ivec4 vect4(rb_num2long(rb_ary_entry(uniform, 0)), rb_num2long(rb_ary_entry(uniform, 1)), rb_num2long(rb_ary_entry(uniform, 2)), rb_num2long(rb_ary_entry(uniform, 3)));
			shader->setUniform(rb_string_value_cstr(&name), vect4);
		}
		else if (arylen == 3)
		{
			sf::Glsl::Ivec3 vect3(rb_num2long(rb_ary_entry(uniform, 0)), rb_num2long(rb_ary_entry(uniform, 1)), rb_num2long(rb_ary_entry(uniform, 2)));
			shader->setUniform(rb_string_value_cstr(&name), vect3);
		}
		else if(arylen == 2)
		{
			sf::Glsl::Ivec2 vect2(rb_num2long(rb_ary_entry(uniform, 0)), rb_num2long(rb_ary_entry(uniform, 1)));
			shader->setUniform(rb_string_value_cstr(&name), vect2);
		}
	}
	else
	{
		shader->setUniform(rb_string_value_cstr(&name), static_cast<int>(rb_num2long(uniform)));
	}
	return self;
}


VALUE rb_Shader_setBoolUniform(VALUE self, VALUE name, VALUE uniform)
{
	GET_SHADER;
	rb_check_type(name, T_STRING);
	if (rb_obj_is_kind_of(uniform, rb_cArray) == Qtrue)
	{
		long arylen = RARRAY_LEN(uniform);
		if (arylen == 4)
		{
			sf::Glsl::Bvec4 vect4(RTEST(rb_ary_entry(uniform, 0)), RTEST(rb_ary_entry(uniform, 1)), RTEST(rb_ary_entry(uniform, 2)), RTEST(rb_ary_entry(uniform, 3)));
			shader->setUniform(rb_string_value_cstr(&name), vect4);
		}
		else if (arylen == 3)
		{
			sf::Glsl::Bvec3 vect3(RTEST(rb_ary_entry(uniform, 0)), RTEST(rb_ary_entry(uniform, 1)), RTEST(rb_ary_entry(uniform, 2)));
			shader->setUniform(rb_string_value_cstr(&name), vect3);
		}
		else if(arylen == 2)
		{ 
			sf::Glsl::Bvec2 vect2(RTEST(rb_ary_entry(uniform, 0)), RTEST(rb_ary_entry(uniform, 1)));
			shader->setUniform(rb_string_value_cstr(&name), vect2);
		}
	}
	else
	{
		shader->setUniform(rb_string_value_cstr(&name), RTEST(uniform));
	}
	return self;
}

VALUE rb_Shader_setTextureUniform(VALUE self, VALUE name, VALUE uniform)
{
	GET_SHADER;
	rb_check_type(name, T_STRING);
	if (rb_obj_is_kind_of(uniform, rb_cBitmap) == Qtrue)
	{
		sf::Texture* texture = rb_Bitmap_getTexture(uniform);
		if (texture != nullptr)
		{
			shader->setUniform(rb_string_value_cstr(&name), *texture);
		}
	}
	else
	{
		shader->setUniform(rb_string_value_cstr(&name), sf::Shader::CurrentTexture);
	}
	return self;
}

VALUE rb_Shader_setMatrixUniform(VALUE self, VALUE name, VALUE uniform)
{
	GET_SHADER;
	unsigned long i;
	rb_check_type(name, T_STRING);
	rb_check_type(uniform, T_ARRAY);
	long arylen = RARRAY_LEN(uniform);
	float floats[16];
	if (arylen == 9)
	{
		for (i = 0; i < 9; i++)
		{
			floats[i] = NUM2DBL(rb_ary_entry(uniform, i));
		}
		sf::Glsl::Mat3 matrix3(floats);
		shader->setUniform(rb_string_value_cstr(&name), matrix3);
	}
	else if(arylen == 16)
	{
		for (i = 0; i < 16; i++)
		{
			floats[i] = NUM2DBL(rb_ary_entry(uniform, i));
		}
		sf::Glsl::Mat4 matrix4(floats);
		shader->setUniform(rb_string_value_cstr(&name), matrix4);
	}

	return self;
}

VALUE rb_Shader_setFloatArrayUniform(VALUE self, VALUE name, VALUE uniform)
{
	GET_SHADER;
	rb_check_type(name, T_STRING);
	rb_check_type(uniform, T_ARRAY);
	unsigned int len = RARRAY_LEN(uniform);
	float* floats = new float[len];
	for (unsigned int i = 0; i < len; i++)
	{
		VALUE val = rb_ary_entry(uniform, i);
		rb_check_type(val, T_FLOAT);
		floats[i] = static_cast<float>(NUM2DBL(val));
	}
	shader->setUniformArray(rb_string_value_cstr(&name), floats, len);
	delete floats;
	return self;
}

VALUE rb_Shader_isGeometryAvailable(VALUE self)
{
	return sf::Shader::isAvailable() ? Qtrue : Qfalse;
}


VALUE rb_Shader_Copy(VALUE self)
{
	rb_raise(rb_eRGSSError, "Shaders cannot be cloned or duplicated.");
	return self;
}