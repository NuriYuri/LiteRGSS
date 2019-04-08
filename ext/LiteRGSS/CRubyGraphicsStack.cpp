#include "utils/ruby_common.h"
#include "CRubyGraphicsStack.h"
#include "CGraphicsStack_Element.h"
#include "CDrawable_Element.h"
#include "SpriteDisposer.h"

CRubyGraphicsStack::CRubyGraphicsStack(VALUE self) {
    rb_ivar_set(self, rb_iElementTable, rb_ary_new());
    m_table = rb_ivar_get(self, rb_iElementTable);    
    rb_check_type(m_table, T_ARRAY);
}

CRubyGraphicsStack::~CRubyGraphicsStack() { 
    clearStack();
}

void CRubyGraphicsStack::add(VALUE el) {
    rb_ary_push(m_table, el);
}

void CRubyGraphicsStack::remove(VALUE el) {
    rb_ary_delete(m_table, el);
}

void CRubyGraphicsStack::syncStack(CGraphicsStack_Element& destStack) {
    destStack.detachSprites();  
    const long sz = RARRAY_LEN(m_table);
    VALUE* ori = RARRAY_PTR(m_table);
    for(long  i = 0; i < sz; i++) {
        const auto it = ori[i];
        if(rb_obj_is_kind_of(it, rb_cViewport) == Qtrue ||
            rb_obj_is_kind_of(it, rb_cSprite) == Qtrue ||
            rb_obj_is_kind_of(it, rb_cText) == Qtrue ||
			rb_obj_is_kind_of(it, rb_cWindow) == Qtrue) {
            if(RDATA(it)->data != nullptr) {
                auto& element = *reinterpret_cast<CDrawable_Element*>(RDATA(it)->data);
                destStack.bind(*this, element);
            }
        }
    }
}

void CRubyGraphicsStack::clearStack() {
    rb_check_type(m_table, T_ARRAY);
	const long sz = RARRAY_LEN(m_table);
	VALUE* ori = RARRAY_PTR(m_table);
	ID dispose_method = rb_intern("dispose");
	for (long i = 0u; i < sz; i++) {
        const auto it = ori[i];
		if(rb_obj_is_kind_of(it, rb_cDrawable) == Qtrue && RDATA(it)->data != nullptr) {
			rb_funcallv(it, dispose_method, 0, NULL);            
		}
    }
    rb_ary_clear(m_table);	
}
