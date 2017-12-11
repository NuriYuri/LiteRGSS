#include "ruby.h"
#include "LiteRGSS.h"
#include "Bitmap.h"
#include "CBitmap_Element.h"
#include <SFML/Graphics.hpp>

VALUE rb_cBitmap = Qnil;

#define BITMAP_PROTECT if(RDATA(self)->data == nullptr) \
{\
    rb_raise(rb_eRGSSError, "Disposed Bitmap."); \
    return self; \
}

void rb_Bitmap_Free(void* data)
{
    CBitmap_Element* bitmap = reinterpret_cast<CBitmap_Element*>(data);
    if(bitmap)
    {
        delete bitmap;
    }
}

VALUE rb_Bitmap_Alloc(VALUE klass)
{
    CBitmap_Element* bitmap = new CBitmap_Element();
    return Data_Wrap_Struct(klass, NULL, rb_Bitmap_Free, bitmap);
}

void Init_Bitmap()
{
    rb_cBitmap = rb_define_class_under(rb_mLiteRGSS, "Bitmap", rb_cObject);
    rb_define_alloc_func(rb_cBitmap, rb_Bitmap_Alloc);
    rb_define_method(rb_cBitmap, "initialize", _rbf rb_Bitmap_Initialize, -1);
    rb_define_method(rb_cBitmap, "initialize_copy", _rbf rb_Bitmap_Initialize_Copy, 1);
    rb_define_method(rb_cBitmap, "dispose", _rbf rb_Bitmap_Dispose, 0);
    rb_define_method(rb_cBitmap, "disposed?", _rbf rb_Bitmap_Disposed, 0);
}

VALUE rb_Bitmap_Initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE string, fromMemory;
    CBitmap_Element* bitmap;
    Data_Get_Struct(self, CBitmap_Element, bitmap);
    BITMAP_PROTECT
    rb_scan_args(argc, argv, "11", &string, &fromMemory);
    sf::Texture* text = bitmap->getTexture();
    /* Load From filename */
    if(NIL_P(fromMemory))
    {
        rb_check_type(string, T_STRING);
        if(!text->loadFromFile(RSTRING_PTR(string)))
        {
            errno = ENOENT;
            rb_sys_fail(RSTRING_PTR(string));
        }
    }
    /* Load From Memory */
    else if(fromMemory == Qtrue)
    {
        rb_check_type(string, T_STRING);
        if(!text->loadFromMemory(RSTRING_PTR(string), RSTRING_LEN(string)))
            rb_raise(rb_eRGSSError, "Failed to load bitmap from memory.");
    }
    else
    {
        rb_raise(rb_eRGSSError, "Bitmap no longer allow drawing, thus Bitmap.new(width, height) is not allowed.");
    }
    return self;
}

VALUE rb_Bitmap_Initialize_Copy(VALUE self, VALUE other)
{
    //rb_notimplement();
    rb_check_frozen(self);
    if(rb_obj_is_kind_of(other, rb_cBitmap) != Qtrue)
    {
        rb_raise(rb_eTypeError, "Cannot clone %s into Bitmap.", RSTRING_PTR(rb_class_name(CLASS_OF(other))));
        return self;
    }
    CBitmap_Element* bitmap;
    CBitmap_Element* bitmapo;
    Data_Get_Struct(self, CBitmap_Element, bitmap);
    Data_Get_Struct(other, CBitmap_Element, bitmapo);
    if(bitmapo == nullptr)
        rb_raise(rb_eRGSSError, "Disposed Bitmap.");
    bitmap->copy(bitmapo);
    return self;
}

VALUE rb_Bitmap_Dispose(VALUE self)
{
    CBitmap_Element* bitmap;
    Data_Get_Struct(self, CBitmap_Element, bitmap);
    BITMAP_PROTECT
    delete bitmap;
    RDATA(self)->data = nullptr;
    return self;
}

VALUE rb_Bitmap_Disposed(VALUE self)
{
    rb_check_type(self, T_DATA);
    return (RDATA(self)->data == nullptr ? Qtrue : Qfalse);
}