#include "LiteRGSS.h"
#include "CBitmap_Element.h"
#include "CRect_Element.h"

VALUE rb_cImage = Qnil;

#define IMAGE_PROTECT if(RDATA(self)->data == nullptr) \
{\
    rb_raise(rb_eRGSSError, "Disposed Image."); \
    return self; \
}

#define GET_IMAGE sf::Image* img; \
    Data_Get_Struct(self, sf::Image, img); \
    IMAGE_PROTECT \

void rb_Image_Free(void* data)
{
    sf::Image* img = reinterpret_cast<sf::Image*>(data);
    if(img != nullptr)
    {
        delete img;
    }
}

VALUE rb_Image_Alloc(VALUE klass)
{
    sf::Image* img = new sf::Image();
    return Data_Wrap_Struct(klass, NULL, rb_Image_Free, img);
}

void Init_Image()
{
    rb_cImage = rb_define_class_under(rb_mLiteRGSS, "Image", rb_cObject);
    rb_define_alloc_func(rb_cImage, rb_Image_Alloc);
    rb_define_method(rb_cImage, "initialize", _rbf rb_Image_Initialize, -1);
    rb_define_method(rb_cImage, "initialize_copy", _rbf rb_Image_Initialize_Copy, 1);
    rb_define_method(rb_cImage, "dispose", _rbf rb_Image_Dispose, 0);
    rb_define_method(rb_cImage, "disposed?", _rbf rb_Image_Disposed, 0);
    rb_define_method(rb_cImage, "width", _rbf rb_Image_Width, 0);
    rb_define_method(rb_cImage, "height", _rbf rb_Image_Height, 0);
    rb_define_method(rb_cImage, "rect", _rbf rb_Image_Rect, 0);
    rb_define_method(rb_cImage, "copy_to_bitmap", _rbf rb_Image_Copy_to_Bitmap, 1);
    rb_define_method(rb_cImage, "blt", _rbf rb_Image_blt, 4);
    rb_define_method(rb_cImage, "clear_rect", _rbf rb_Image_clear_rect, 4);
    rb_define_method(rb_cImage, "fill_rect", _rbf rb_Image_fill_rect, 5);
    rb_define_method(rb_cImage, "to_png", _rbf rb_Image_toPNG, 0);
    rb_define_method(rb_cImage, "to_png_file", _rbf rb_Image_toPNG_file, 1);
}

VALUE rb_Image_Initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE string, fromMemory;
    GET_IMAGE
    rb_scan_args(argc, argv, "11", &string, &fromMemory);
    /* Load From filename */
    if(NIL_P(fromMemory))
    {
        rb_check_type(string, T_STRING);
		if(!rb_Image_LoadLodePNG(img, RSTRING_PTR(string), 0))
			if(!img->loadFromFile(RSTRING_PTR(string)))
			{
				errno = ENOENT;
				rb_sys_fail(RSTRING_PTR(string));
			}
    }
    /* Load From Memory */
    else if(fromMemory == Qtrue)
    {
        rb_check_type(string, T_STRING);
		if(!rb_Image_LoadLodePNG(img, RSTRING_PTR(string), RSTRING_LEN(string)))
			if(!img->loadFromMemory(RSTRING_PTR(string), RSTRING_LEN(string)))
				rb_raise(rb_eRGSSError, "Failed to load image from memory.");
    }
    else
    {
        rb_check_type(string, T_FIXNUM);
        rb_check_type(fromMemory, T_FIXNUM);
        img->create(rb_num2long(string), rb_num2long(fromMemory), sf::Color(0, 0, 0, 0));
        //rb_raise(rb_eRGSSError, "Bitmap no longer allow drawing, thus Bitmap.new(width, height) is not allowed.");
    }
    return self;
}

VALUE rb_Image_Initialize_Copy(VALUE self, VALUE other)
{
    //rb_notimplement();
    rb_check_frozen(self);
    if(rb_obj_is_kind_of(other, rb_cImage) != Qtrue)
    {
        rb_raise(rb_eTypeError, "Cannot clone %s into Image.", RSTRING_PTR(rb_class_name(CLASS_OF(other))));
        return self;
    }
    sf::Image* img;
    sf::Image* imgo;
    Data_Get_Struct(self, sf::Image, img);
    Data_Get_Struct(other, sf::Image, imgo);
    if(imgo == nullptr)
        rb_raise(rb_eRGSSError, "Disposed Image.");
    img->create(imgo->getSize().x, imgo->getSize().y, imgo->getPixelsPtr());
    return self;
}

VALUE rb_Image_Dispose(VALUE self)
{
    GET_IMAGE
    delete img;
    RDATA(self)->data = nullptr;
    return self;
}

VALUE rb_Image_Disposed(VALUE self)
{
    rb_check_type(self, T_DATA);
    return (RDATA(self)->data == nullptr ? Qtrue : Qfalse);
}

VALUE rb_Image_Width(VALUE self)
{
    GET_IMAGE
    sf::Vector2u size = img->getSize();
    return rb_int2inum(size.x);
}

VALUE rb_Image_Height(VALUE self)
{
    GET_IMAGE
    sf::Vector2u size = img->getSize();
    return rb_int2inum(size.y);
}

VALUE rb_Image_Rect(VALUE self)
{
    GET_IMAGE
    sf::Vector2u size = img->getSize();
    VALUE argv[4] = {LONG2FIX(0), LONG2FIX(0), rb_int2inum(size.x), rb_int2inum(size.y)};
    return rb_class_new_instance(4, argv, rb_cRect);
}

VALUE rb_Image_Copy_to_Bitmap(VALUE self, VALUE bitmap)
{
    GET_IMAGE
    rb_Bitmap_getTexture(bitmap)->update(*img);
    return self;
}

VALUE rb_Image_blt(VALUE self, VALUE x, VALUE y, VALUE src_image, VALUE rect)
{
    GET_IMAGE
    if(rb_obj_is_kind_of(rect, rb_cRect) != Qtrue)
    {
        rb_raise(rb_eTypeError, "Expected Rect got %s.", RSTRING_PTR(rb_class_name(CLASS_OF(rect))));
        return Qnil;
    }
    if(rb_obj_is_kind_of(src_image, rb_cImage) != Qtrue)
    {
        rb_raise(rb_eTypeError, "Expected Image got %s", RSTRING_PTR(rb_class_name(CLASS_OF(src_image))));
        return self;
    }
    sf::Image* img2;
    Data_Get_Struct(src_image, sf::Image, img2);
    CRect_Element* s_rect;
    Data_Get_Struct(rect, CRect_Element, s_rect);
    if(RDATA(src_image)->data == nullptr) 
    {
        rb_raise(rb_eRGSSError, "Disposed Image."); 
        return self; 
    }
    img->copy(
        *img2,
        NUM2ULONG(x),
        NUM2ULONG(y),
        *s_rect->getRect()
    );
    return self;
}

VALUE rb_Image_clear_rect(VALUE self, VALUE x, VALUE y, VALUE width, VALUE height)
{
	GET_IMAGE
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
	sf::Color clr = sf::Color(0, 0, 0, 0);
	while (y1 < y2)
	{
		for (long x3 = x1; x3 < x2; x3++)
			img->setPixel(x3, y1, clr);
		y1++;
	}
	return self;
}

VALUE rb_Image_fill_rect(VALUE self, VALUE x, VALUE y, VALUE width, VALUE height, VALUE color)
{
	GET_IMAGE;
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
	while (y1 < y2)
	{
		for (long x3 = x1; x3 < x2; x3++)
			img->setPixel(x3, y1, *rcolor);
		y1++;
	}
	return self;
}

VALUE rb_Image_toPNG(VALUE self)
{
	GET_IMAGE;
	unsigned char* out;
	size_t size;
	if (lodepng_encode32(&out, &size, img->getPixelsPtr(), img->getSize().x, img->getSize().y) != 0)
	{
		if (out)
			free(out);
		return Qnil;
	}
	VALUE ret_val = rb_str_new(reinterpret_cast<const char*>(out), size);
	free(out);
	return ret_val;
}

VALUE rb_Image_toPNG_file(VALUE self, VALUE filename)
{
	rb_check_type(filename, T_STRING);
	GET_IMAGE;
	if (lodepng_encode32_file(RSTRING_PTR(filename), img->getPixelsPtr(), img->getSize().x, img->getSize().y) != 0)
		return Qfalse;
	return Qtrue;
}

bool rb_Image_LoadLodePNG(sf::Image* img, char* str, long from_memory_size)
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
	img->create(w, h, reinterpret_cast<sf::Uint8*>(out));
	free(out);
	return true;
}
