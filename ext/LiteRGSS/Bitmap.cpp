#include "ruby.h"
#include "LiteRGSS.h"
#include "Bitmap.h"
#include "CBitmap_Element.h"
#include <SFML/Graphics.hpp>
#include <iostream>

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
}

VALUE rb_Bitmap_Initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE width, height;
    CBitmap_Element* bitmap;
    Data_Get_Struct(self, CBitmap_Element, bitmap);
    sf::Texture* text = bitmap->getTexture();
    rb_scan_args(argc, argv, "11", &width, &height);
    /* Load From filename */
    if(NIL_P(height))
    {
        rb_check_type(width, T_STRING);
        if(!text->loadFromFile(RSTRING_PTR(width)))
        {
            errno = ENOENT;
            rb_sys_fail(RSTRING_PTR(width));
        }
    }
    /* Load From Memory */
    else if(width == Qtrue)
    {
        rb_check_type(width, T_STRING);
        if(!text->loadFromMemory(RSTRING_PTR(width), RSTRING_LEN(width)))
            rb_raise(rb_eRGSSError, "Failed to load bitmap from memory.");
    }
    /* Create a new Bitmap */
    else
    {
        if(!text->create(rb_num2ulong(width), rb_num2ulong(height)))
            rb_raise(rb_eRGSSError, "Failed to create bitmap with such dimentions. (max: 8192x8192)");
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
    delete bitmap;
    RDATA(self)->data = nullptr;
    return self;
}