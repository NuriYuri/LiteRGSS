#include "LiteRGSS.h"

VALUE rb_cBlendMode = Qnil;

#define BLENDMODE_PROTECT if(RDATA(self)->data == nullptr) \
{\
    rb_raise(rb_eRGSSError, "Disposed BlendMode."); \
    return self; \
}

#define GET_BLENDMODE sf::RenderStates* render_state; \
    Data_Get_Struct(self, sf::RenderStates, render_state); \
    BLENDMODE_PROTECT

void rb_BlendMode_Free(void* data)
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

VALUE rb_BlendMode_Alloc(VALUE klass)
{
	sf::RenderStates* render_state = new sf::RenderStates();
	return Data_Wrap_Struct(klass, NULL, rb_BlendMode_Free, render_state);
}

void Init_BlendMode()
{
	rb_cBlendMode = rb_define_class_under(rb_mLiteRGSS, "BlendMode", rb_cObject); 
	rb_define_alloc_func(rb_cBlendMode, rb_BlendMode_Alloc);

	rb_define_method(rb_cBlendMode, "color_src_factor", _rbf rb_BlendMode_getColorSrcFactor, 0);
	rb_define_method(rb_cBlendMode, "color_src_factor=", _rbf rb_BlendMode_setColorSrcFactor, 1);
	rb_define_method(rb_cBlendMode, "color_dest_factor", _rbf rb_BlendMode_getColorDestFactor, 0);
	rb_define_method(rb_cBlendMode, "color_dest_factor=", _rbf rb_BlendMode_setColorDestFactor, 1);
	rb_define_method(rb_cBlendMode, "alpha_src_factor", _rbf rb_BlendMode_getAlphaSrcFactor, 0);
	rb_define_method(rb_cBlendMode, "alpha_src_factor=", _rbf rb_BlendMode_setAlphaSrcFactor, 1);
	rb_define_method(rb_cBlendMode, "alpha_dest_factor", _rbf rb_BlendMode_getAlphaDestFactor, 0);
	rb_define_method(rb_cBlendMode, "alpha_dest_factor=", _rbf rb_BlendMode_setAlphaDestFactor, 1);
	rb_define_method(rb_cBlendMode, "color_equation", _rbf rb_BlendMode_getColorEquation, 0);
	rb_define_method(rb_cBlendMode, "color_equation=", _rbf rb_BlendMode_setColorEquation, 1);
	rb_define_method(rb_cBlendMode, "alpha_equation", _rbf rb_BlendMode_getAlphaEquation, 0);
	rb_define_method(rb_cBlendMode, "alpha_equation=", _rbf rb_BlendMode_setAlphaEquation, 1);
	rb_define_method(rb_cBlendMode, "blend_type", _rbf rb_BlendMode_getBlendType, 0);
	rb_define_method(rb_cBlendMode, "blend_type=", _rbf rb_BlendMode_setBlendType, 1);

	rb_define_method(rb_cBlendMode, "clone", _rbf rb_BlendMode_Copy, 0);
	rb_define_method(rb_cBlendMode, "dup", _rbf rb_BlendMode_Copy, 0);

	// Equation
	rb_define_const(rb_cBlendMode, "Add", LONG2FIX(sf::BlendMode::Equation::Add));
	rb_define_const(rb_cBlendMode, "Subtract", LONG2FIX(sf::BlendMode::Equation::Subtract));
	rb_define_const(rb_cBlendMode, "ReverseSubtract", LONG2FIX(sf::BlendMode::Equation::ReverseSubtract));

	// Factor
	rb_define_const(rb_cBlendMode, "Zero", LONG2FIX(sf::BlendMode::Factor::Zero));
	rb_define_const(rb_cBlendMode, "One", LONG2FIX(sf::BlendMode::Factor::One));
	rb_define_const(rb_cBlendMode, "SrcColor", LONG2FIX(sf::BlendMode::Factor::SrcColor));
	rb_define_const(rb_cBlendMode, "OneMinusSrcColor", LONG2FIX(sf::BlendMode::Factor::OneMinusSrcColor));
	rb_define_const(rb_cBlendMode, "DstColor", LONG2FIX(sf::BlendMode::Factor::DstColor));
	rb_define_const(rb_cBlendMode, "OneMinusDstColor", LONG2FIX(sf::BlendMode::Factor::OneMinusDstColor));
	rb_define_const(rb_cBlendMode, "SrcAlpha", LONG2FIX(sf::BlendMode::Factor::SrcAlpha));
	rb_define_const(rb_cBlendMode, "OneMinusSrcAlpha", LONG2FIX(sf::BlendMode::Factor::OneMinusSrcAlpha));
	rb_define_const(rb_cBlendMode, "DstAlpha", LONG2FIX(sf::BlendMode::Factor::DstAlpha));
	rb_define_const(rb_cBlendMode, "OneMinusDstAlpha", LONG2FIX(sf::BlendMode::Factor::OneMinusDstAlpha));

}

VALUE rb_BlendMode_setColorSrcFactor(VALUE self, VALUE val)
{
	GET_BLENDMODE;
	render_state->blendMode.colorSrcFactor = static_cast<sf::BlendMode::Factor>(NUM2LONG(val));
	return self;
}

VALUE rb_BlendMode_getColorSrcFactor(VALUE self)
{
	GET_BLENDMODE;
	return LONG2NUM(render_state->blendMode.colorSrcFactor);
}

VALUE rb_BlendMode_setColorDestFactor(VALUE self, VALUE val)
{
	GET_BLENDMODE;
	render_state->blendMode.colorDstFactor = static_cast<sf::BlendMode::Factor>(NUM2LONG(val));
	return self;
}

VALUE rb_BlendMode_getColorDestFactor(VALUE self)
{
	GET_BLENDMODE;
	return LONG2NUM(render_state->blendMode.colorDstFactor);
}

VALUE rb_BlendMode_setAlphaSrcFactor(VALUE self, VALUE val)
{
	GET_BLENDMODE;
	render_state->blendMode.alphaSrcFactor = static_cast<sf::BlendMode::Factor>(NUM2LONG(val));
	return self;
}

VALUE rb_BlendMode_getAlphaSrcFactor(VALUE self)
{
	GET_BLENDMODE;
	return LONG2NUM(render_state->blendMode.alphaSrcFactor);
}

VALUE rb_BlendMode_setAlphaDestFactor(VALUE self, VALUE val)
{
	GET_BLENDMODE;
	render_state->blendMode.alphaDstFactor = static_cast<sf::BlendMode::Factor>(NUM2LONG(val));
	return self;
}

VALUE rb_BlendMode_getAlphaDestFactor(VALUE self)
{
	GET_BLENDMODE;
	return LONG2NUM(render_state->blendMode.alphaDstFactor);
}

VALUE rb_BlendMode_setColorEquation(VALUE self, VALUE val)
{
	GET_BLENDMODE;
	render_state->blendMode.colorEquation = static_cast<sf::BlendMode::Equation>(NUM2LONG(val));
	return self;
}

VALUE rb_BlendMode_getColorEquation(VALUE self)
{
	GET_BLENDMODE;
	return LONG2NUM(render_state->blendMode.colorEquation);
}

VALUE rb_BlendMode_setAlphaEquation(VALUE self, VALUE val)
{
	GET_BLENDMODE;
	render_state->blendMode.alphaEquation = static_cast<sf::BlendMode::Equation>(NUM2LONG(val));
	return self;
}

VALUE rb_BlendMode_getAlphaEquation(VALUE self)
{
	GET_BLENDMODE;
	return LONG2NUM(render_state->blendMode.alphaEquation);
}

VALUE rb_BlendMode_setBlendType(VALUE self, VALUE val)
{
	GET_BLENDMODE;
	long mode = NUM2LONG(val);
	switch (mode)
	{
	case 1: // Add
		render_state->blendMode.colorEquation = sf::BlendMode::Equation::Add;
		render_state->blendMode.alphaEquation = sf::BlendMode::Equation::Add;
		render_state->blendMode.colorSrcFactor = sf::BlendMode::Factor::SrcAlpha;
		render_state->blendMode.colorDstFactor = sf::BlendMode::Factor::One;
		render_state->blendMode.alphaSrcFactor = sf::BlendMode::Factor::One;
		render_state->blendMode.alphaDstFactor = sf::BlendMode::Factor::One;
		break;
	case 2: // Sub
		render_state->blendMode.colorEquation = sf::BlendMode::Equation::ReverseSubtract;
		render_state->blendMode.alphaEquation = sf::BlendMode::Equation::ReverseSubtract;
		render_state->blendMode.colorSrcFactor = sf::BlendMode::Factor::SrcAlpha;
		render_state->blendMode.colorDstFactor = sf::BlendMode::Factor::One;
		render_state->blendMode.alphaSrcFactor = sf::BlendMode::Factor::Zero;
		render_state->blendMode.alphaDstFactor = sf::BlendMode::Factor::One;
		break;
	default: // Normal
		render_state->blendMode.colorEquation = sf::BlendMode::Equation::Add;
		render_state->blendMode.alphaEquation = sf::BlendMode::Equation::Add;
		render_state->blendMode.colorSrcFactor = sf::BlendMode::Factor::SrcAlpha;
		render_state->blendMode.colorDstFactor = sf::BlendMode::Factor::OneMinusSrcAlpha;
		render_state->blendMode.alphaSrcFactor = sf::BlendMode::Factor::One;
		render_state->blendMode.alphaDstFactor = sf::BlendMode::Factor::OneMinusSrcAlpha;
		break;
	}
	return self;
}

VALUE rb_BlendMode_getBlendType(VALUE self)
{
	GET_BLENDMODE;
	if (render_state->blendMode.colorEquation == sf::BlendMode::Equation::ReverseSubtract)
		return LONG2FIX(2);
	if (render_state->blendMode.alphaDstFactor == sf::BlendMode::Factor::One)
		return LONG2FIX(1);
	return LONG2FIX(0);
}

VALUE rb_BlendMode_Copy(VALUE self)
{
	rb_raise(rb_eRGSSError, "BlendMode cannot be cloned or duplicated.");
	return self;
}