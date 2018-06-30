#include "LiteRGSS.h"
#include "CBitmap_Element.h"
#include "CRect_Element.h"

VALUE rb_cShaderSprite = Qnil;

#define SHADER_SPRITE_PROTECT if(RDATA(self)->data == nullptr) \
{\
    rb_raise(rb_eRGSSError, "Disposed Sprite."); \
    return self; \
}

#define GET_SHADER_SPRITE CShaderSprite_Element* sprite; \
    Data_Get_Struct(self, CShaderSprite_Element, sprite); \
    SHADER_SPRITE_PROTECT

void rb_ShaderSprite_Mark(CShaderSprite_Element* sprite)
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

VALUE rb_ShaderSprite_Alloc(VALUE klass)
{
	CShaderSprite_Element* sprite = new CShaderSprite_Element();
    sprite->getSprite()->setColor(sf::Color(255, 255, 255, 255));
    sprite->setLinkedRect(nullptr);
	sprite->rRenderStates = Qnil;
    return Data_Wrap_Struct(klass, rb_ShaderSprite_Mark, rb_Sprite_Free, sprite);
}

void Init_ShaderSprite() {
	rb_cShaderSprite = rb_define_class_under(rb_mLiteRGSS, "ShaderedSprite", rb_cSprite);
    rb_define_alloc_func(rb_cShaderSprite, rb_ShaderSprite_Alloc);

	rb_define_method(rb_cShaderSprite, "shader", _rbf rb_ShaderSprite_getShader, 0);
	rb_define_method(rb_cShaderSprite, "shader=", _rbf rb_ShaderSprite_setShader, 1);

	rb_define_method(rb_cShaderSprite, "blendmode", _rbf rb_ShaderSprite_getShader, 0);
	rb_define_method(rb_cShaderSprite, "blendmode=", _rbf rb_ShaderSprite_setShader, 1);
}

VALUE rb_ShaderSprite_getShader(VALUE self)
{
	GET_SHADER_SPRITE;
	return sprite->rRenderStates;
}

VALUE rb_ShaderSprite_setShader(VALUE self, VALUE shader)
{
	sf::RenderStates* render_state;
	GET_SHADER_SPRITE;
	if (rb_obj_is_kind_of(shader, rb_cBlendMode) == Qtrue) 
	{
		Data_Get_Struct(shader, sf::RenderStates, render_state);
		if (render_state != nullptr)
		{
			sprite->rRenderStates = shader;
			sprite->setRenderState(render_state);
			return self;
		}
	}
	sprite->rRenderStates = Qnil;
	sprite->setRenderState(nullptr);
	return self;
}