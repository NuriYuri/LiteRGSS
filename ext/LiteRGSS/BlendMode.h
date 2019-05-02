#ifndef L_BLENDMODE_HEADER
#define L_BLENDMODE_HEADER
#include "rbAdapter.h"

VALUE rb_BlendMode_setColorSrcFactor(VALUE self, VALUE val);
VALUE rb_BlendMode_getColorSrcFactor(VALUE self);
VALUE rb_BlendMode_setColorDestFactor(VALUE self, VALUE val);
VALUE rb_BlendMode_getColorDestFactor(VALUE self);
VALUE rb_BlendMode_setAlphaSrcFactor(VALUE self, VALUE val);
VALUE rb_BlendMode_getAlphaSrcFactor(VALUE self);
VALUE rb_BlendMode_setAlphaDestFactor(VALUE self, VALUE val);
VALUE rb_BlendMode_getAlphaDestFactor(VALUE self);
VALUE rb_BlendMode_setColorEquation(VALUE self, VALUE val);
VALUE rb_BlendMode_getColorEquation(VALUE self);
VALUE rb_BlendMode_setAlphaEquation(VALUE self, VALUE val);
VALUE rb_BlendMode_getAlphaEquation(VALUE self);
VALUE rb_BlendMode_setBlendType(VALUE self, VALUE val);
VALUE rb_BlendMode_getBlendType(VALUE self);
VALUE rb_BlendMode_Copy(VALUE self);

namespace meta {
	template<>
	struct Log<sf::RenderStates> {
		static constexpr auto classname = "BlendMode";
	};
}

#endif
