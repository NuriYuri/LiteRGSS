#include "SpriteDisposer.h"

#include "CSprite_Element.h"
#include "CText_Element.h"
#include "CShape_Element.h"
#include "CWindow_Element.h"
#include "CViewport_Element.h"

extern VALUE rb_cDrawable;
/*extern VALUE rb_cSprite;
extern VALUE rb_cText;
extern VALUE rb_cShape;
extern VALUE rb_cWindow;
extern VALUE rb_cViewport;*/

void DisposeAllSprites(VALUE table) {
    rb_check_type(table, T_ARRAY);
	const auto sz = RARRAY_LEN(table);
	VALUE* ori = RARRAY_PTR(table);
	ID dispose_method = rb_intern("dispose");
	for (auto i = 0u; i < sz; i++) {
		if(rb_obj_is_kind_of(ori[i], rb_cDrawable) == Qtrue)
		{
			if(RDATA(ori[i])->data != nullptr)
				rb_funcallv(ori[i], dispose_method, 0, NULL);
		}
/*
// OLD Version (in case of the new version isn't good enough)
		if(RDATA(ori[i])->data != nullptr) {
			if (rb_obj_is_kind_of(ori[i], rb_cSprite) == Qtrue) {
                DisposeAs<CSprite_Element>(ori[i]);
			} else if (rb_obj_is_kind_of(ori[i], rb_cText) == Qtrue) {
                DisposeAs<CText_Element>(ori[i]);
			} else if (rb_obj_is_kind_of(ori[i], rb_cShape) == Qtrue) {
				DisposeAs<CShape_Element>(ori[i]);
			} else if (rb_obj_is_kind_of(ori[i], rb_cWindow) == Qtrue) {
				DisposeAs<CWindow_Element>(ori[i]);
			} else if(rb_obj_is_kind_of(ori[i], rb_cViewport) == Qtrue) {
				DisposeAs<CViewport_Element>(ori[i]);
            }
		}
*/
	}
	
	rb_ary_clear(table);
}
