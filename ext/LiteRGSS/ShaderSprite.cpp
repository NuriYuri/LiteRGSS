#include "LiteRGSS.h"
#include "CBitmap_Element.h"
#include "CRect_Element.h"

VALUE rb_cShaderSprite = Qnil;

template<>
void rb::Mark<CShaderSprite_Element>(CShaderSprite_Element* sprite)
{
	if(sprite == nullptr)
		return;
	rb_gc_mark(sprite->rViewport);
	rb_gc_mark(sprite->rBitmap);
	rb_gc_mark(sprite->rX);
	rb_gc_mark(sprite->rY);
	rb_gc_mark(sprite->rZ);
	rb_gc_mark(sprite->rOX);
	rb_gc_mark(sprite->rOY);
	rb_gc_mark(sprite->rAngle);
	rb_gc_mark(sprite->rZoomX);
	rb_gc_mark(sprite->rZoomY);
	rb_gc_mark(sprite->rRect);
	rb_gc_mark(sprite->rRenderStates);
}

void Init_ShaderSprite() {
	rb_cShaderSprite = rb_define_class_under(rb_mLiteRGSS, "ShaderedSprite", rb_cSprite);
	rb_define_alloc_func(rb_cShaderSprite, rb::AllocDrawable<CShaderSprite_Element>);

	rb_define_method(rb_cShaderSprite, "shader", _rbf rb_ShaderSprite_getShader, 0);
	rb_define_method(rb_cShaderSprite, "shader=", _rbf rb_ShaderSprite_setShader, 1);

	rb_define_method(rb_cShaderSprite, "blendmode", _rbf rb_ShaderSprite_getShader, 0);
	rb_define_method(rb_cShaderSprite, "blendmode=", _rbf rb_ShaderSprite_setShader, 1);
}

VALUE rb_ShaderSprite_getShader(VALUE self)
{
	auto& sprite = rb::Get<CShaderSprite_Element>(self);
	return sprite.rRenderStates;
}

VALUE rb_ShaderSprite_setShader(VALUE self, VALUE shader)
{
	sf::RenderStates* render_state;
	auto& sprite = rb::Get<CShaderSprite_Element>(self);
	if (rb_obj_is_kind_of(shader, rb_cBlendMode) == Qtrue) 
	{
		Data_Get_Struct(shader, sf::RenderStates, render_state);
		if (render_state != nullptr)
		{
			sprite.rRenderStates = shader;
			sprite.setRenderState(render_state);
			return self;
		}
	}
	sprite.rRenderStates = Qnil;
	sprite.setRenderState(nullptr);
	return self;
}
