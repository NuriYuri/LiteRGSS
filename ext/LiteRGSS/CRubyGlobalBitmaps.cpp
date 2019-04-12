#include "CRubyGlobalBitmaps.h"
#include "rbAdapter.h"
#include "ruby_common.h"
#include "CBitmap_Element.h"

CRubyGlobalBitmaps::~CRubyGlobalBitmaps() {
    clear();
}

extern "C" {
    VALUE local_Graphics_call_gc_start(VALUE ignored) {
        rb_gc_start();
        return Qnil;
    }

    VALUE local_Graphics_Dispose_Bitmap(VALUE block_arg, VALUE data, int argc, VALUE* argv) {
        if(RDATA(block_arg)->data != nullptr) {
            rb::Dispose<CBitmap_Element>(block_arg);
        }
        return Qnil;
    }
}

void CRubyGlobalBitmaps::clear() {
    /* Disposing each Bitmap */
    const auto objectSpace = rb_const_get(rb_cObject, rb_intern("ObjectSpace"));
    rb_block_call(objectSpace, rb_intern("each_object"), 1, &rb_cBitmap, (rb_block_call_func_t)local_Graphics_Dispose_Bitmap, Qnil);

    int state;
    rb_protect(local_Graphics_call_gc_start, Qnil, &state);
}
