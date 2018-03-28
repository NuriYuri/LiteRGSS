#include "LiteRGSS.h"
#include "CBitmap_Element.h"
#include "CRect_Element.h"

VALUE rb_cBitmap = Qnil;

#define BITMAP_PROTECT if(RDATA(self)->data == nullptr) \
{\
    rb_raise(rb_eRGSSError, "Disposed Bitmap."); \
    return self; \
}

#define GET_BITMAP CBitmap_Element* bitmap; \
    Data_Get_Struct(self, CBitmap_Element, bitmap); \
    BITMAP_PROTECT \

void rb_Bitmap_Free(void* data)
{
    CBitmap_Element* bitmap = reinterpret_cast<CBitmap_Element*>(data);
    if(bitmap != nullptr)
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
    rb_define_method(rb_cBitmap, "width", _rbf rb_Bitmap_Width, 0);
    rb_define_method(rb_cBitmap, "height", _rbf rb_Bitmap_Height, 0);
    rb_define_method(rb_cBitmap, "rect", _rbf rb_Bitmap_Rect, 0);
    rb_define_method(rb_cBitmap, "update", _rbf rb_Bitmap_Update, 0);
    rb_define_method(rb_cBitmap, "blt", _rbf rb_Bitmap_blt, 4);
	rb_define_method(rb_cBitmap, "clear_rect", _rbf rb_Bitmap_clear_rect, 4);
	rb_define_method(rb_cBitmap, "fill_rect", _rbf rb_Bitmap_fill_rect, 5);
	rb_define_method(rb_cBitmap, "to_png", _rbf rb_Bitmap_toPNG, 0);
	rb_define_method(rb_cBitmap, "to_png_file", _rbf rb_Bitmap_toPNG_file, 1);
}

VALUE rb_Bitmap_Initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE string, fromMemory;
    GET_BITMAP
    rb_scan_args(argc, argv, "11", &string, &fromMemory);
    sf::Texture* text = bitmap->getTexture();
    /* Load From filename */
    if(NIL_P(fromMemory))
    {
        rb_check_type(string, T_STRING);
		if(!rb_Bitmap_LoadLodePNG(text, RSTRING_PTR(string), 0))
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
		if(!rb_Bitmap_LoadLodePNG(text, RSTRING_PTR(string), RSTRING_LEN(string)))
			if(!text->loadFromMemory(RSTRING_PTR(string), RSTRING_LEN(string)))
				rb_raise(rb_eRGSSError, "Failed to load bitmap from memory.");
    }
    else
    {
        rb_check_type(string, T_FIXNUM);
        rb_check_type(fromMemory, T_FIXNUM);
        sf::Image* img = bitmap->getImage();
        img->create(rb_num2long(string), rb_num2long(fromMemory), sf::Color(0, 0, 0, 0));
        if(!text->loadFromImage(*img))
        {
            rb_raise(rb_eRGSSError, "Invalid texture size !");
        }
        //rb_raise(rb_eRGSSError, "Bitmap no longer allow drawing, thus Bitmap.new(width, height) is not allowed.");
    }
    text->setSmooth(SmoothScreen);
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
    GET_BITMAP
    delete bitmap;
    RDATA(self)->data = nullptr;
    return self;
}

VALUE rb_Bitmap_Disposed(VALUE self)
{
    rb_check_type(self, T_DATA);
    return (RDATA(self)->data == nullptr ? Qtrue : Qfalse);
}

VALUE rb_Bitmap_Width(VALUE self)
{
    GET_BITMAP
    sf::Vector2u size = bitmap->getTexture()->getSize();
    return rb_int2inum(size.x);
}

VALUE rb_Bitmap_Height(VALUE self)
{
    GET_BITMAP
    sf::Vector2u size = bitmap->getTexture()->getSize();
    return rb_int2inum(size.y);
}

VALUE rb_Bitmap_Rect(VALUE self)
{
    GET_BITMAP
    sf::Vector2u size = bitmap->getTexture()->getSize();
    VALUE argv[4] = {LONG2FIX(0), LONG2FIX(0), rb_int2inum(size.x), rb_int2inum(size.y)};
    return rb_class_new_instance(4, argv, rb_cRect);
}

VALUE rb_Bitmap_Update(VALUE self)
{
    GET_BITMAP
    if(bitmap->has_image())
    {
        bitmap->getTexture()->update(*bitmap->getImage());
    }
    return self;
}

VALUE rb_Bitmap_blt(VALUE self, VALUE x, VALUE y, VALUE src_bitmap, VALUE rect)
{
    GET_BITMAP
    if(bitmap->has_image())
    {
        if(rb_obj_is_kind_of(rect, rb_cRect) != Qtrue)
        {
            rb_raise(rb_eTypeError, "Expected Rect got %s.", RSTRING_PTR(rb_class_name(CLASS_OF(rect))));
            return Qnil;
        }
        CBitmap_Element* s_bitmap;
        Data_Get_Struct(src_bitmap, CBitmap_Element, s_bitmap);
        CRect_Element* s_rect;
        Data_Get_Struct(rect, CRect_Element, s_rect);
        if(RDATA(src_bitmap)->data == nullptr) 
        {
            rb_raise(rb_eRGSSError, "Disposed Bitmap."); 
            return self; 
        }
        bitmap->getImage()->copy(
            *s_bitmap->getImage(),
            NUM2ULONG(x),
			NUM2ULONG(y),
            *s_rect->getRect()
        );
    }
    return self;
}

VALUE rb_Bitmap_clear_rect(VALUE self, VALUE x, VALUE y, VALUE width, VALUE height)
{
	GET_BITMAP
	rb_check_type(x, T_FIXNUM);
	rb_check_type(y, T_FIXNUM);
	rb_check_type(width, T_FIXNUM);
	rb_check_type(height, T_FIXNUM);
	long x1 = NUM2LONG(x);
	long x2 = NUM2LONG(width) + x1;
	if (x1 < 0)
		x1 = 0;
	long y1 = NUM2LONG(y);
	long y2 = NUM2LONG(height) + y1;
	if (y1 < 0)
		y1 = 0;
	sf::Image* img = bitmap->getImage();
	sf::Color clr = sf::Color(0, 0, 0, 0);
	while (y1 < y2)
	{
		for (long x3 = x1; x3 < x2; x3++)
			img->setPixel(x3, y1, clr);
		y1++;
	}
	return self;
}

VALUE rb_Bitmap_fill_rect(VALUE self, VALUE x, VALUE y, VALUE width, VALUE height, VALUE color)
{
	GET_BITMAP;
	if (rb_obj_is_kind_of(color, rb_cColor) != Qtrue)
		return self;
	sf::Color* rcolor;
	Data_Get_Struct(color, sf::Color, rcolor);
	rb_check_type(x, T_FIXNUM);
	rb_check_type(y, T_FIXNUM);
	rb_check_type(width, T_FIXNUM);
	rb_check_type(height, T_FIXNUM);
	long x1 = NUM2LONG(x);
	long x2 = NUM2LONG(width) + x1;
	if (x1 < 0)
		x1 = 0;
	long y1 = NUM2LONG(y);
	long y2 = NUM2LONG(height) + y1;
	if (y1 < 0)
		y1 = 0;
	sf::Image* img = bitmap->getImage();
	while (y1 < y2)
	{
		for (long x3 = x1; x3 < x2; x3++)
			img->setPixel(x3, y1, *rcolor);
		y1++;
	}
	return self;
}

VALUE rb_Bitmap_toPNG(VALUE self)
{
	GET_BITMAP;
	sf::Image img = bitmap->getTexture()->copyToImage();
	unsigned char* out;
	size_t size;
	if (lodepng_encode32(&out, &size, img.getPixelsPtr(), img.getSize().x, img.getSize().y) != 0)
	{
		if (out)
			free(out);
		return Qnil;
	}
	VALUE ret_val = rb_str_new(reinterpret_cast<const char*>(out), size);
	free(out);
	return ret_val;
}

VALUE rb_Bitmap_toPNG_file(VALUE self, VALUE filename)
{
	rb_check_type(filename, T_STRING);
	GET_BITMAP;
	sf::Image img = bitmap->getTexture()->copyToImage();
	if (lodepng_encode32_file(RSTRING_PTR(filename), img.getPixelsPtr(), img.getSize().x, img.getSize().y) != 0)
		return Qfalse;
	return Qtrue;
}

bool rb_Bitmap_LoadLodePNG(sf::Texture* text, char* str, long from_memory_size)
{
	unsigned char* out = nullptr;
	unsigned w;
	unsigned h;
	if (from_memory_size > 0)
	{
		if (lodepng_decode32(&out, &w, &h, reinterpret_cast<unsigned char*>(str), from_memory_size) != 0)
		{
			if (out)
				free(out);
			return false;
		}
	}
	else
	{
		if (lodepng_decode32_file(&out, &w, &h, str) != 0)
		{
			if (out)
				free(out);
			return false;
		}
	}
	text->create(w, h);
	text->update(reinterpret_cast<sf::Uint8*>(out), w, h, 0, 0);
	free(out);
	return true;
}

sf::Texture* rb_Bitmap_getTexture(VALUE self)
{
	CBitmap_Element* bitmap;
	Data_Get_Struct(self, CBitmap_Element, bitmap);
	if (bitmap == nullptr)
	{
		rb_raise(rb_eRGSSError, "Disposed Bitmap.");
		return nullptr;
	}
	return bitmap->getTexture();
}