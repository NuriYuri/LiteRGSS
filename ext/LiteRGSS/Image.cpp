#include "LiteRGSS.h"
#include "Bitmap.h"
#include "CBitmap_Element.h"
#include "CRect_Element.h"

VALUE rb_cImage = Qnil;

void Init_Image()
{
    rb_cImage = rb_define_class_under(rb_mLiteRGSS, "Image", rb_cDisposable);
    rb_define_alloc_func(rb_cImage, rb::Alloc<sf::Image>);
    rb_define_method(rb_cImage, "initialize", _rbf rb_Image_Initialize, -1);
    rb_define_method(rb_cImage, "initialize_copy", _rbf rb_Image_Initialize_Copy, 1);
    rb_define_method(rb_cImage, "dispose", _rbf rb_Image_Dispose, 0);
    rb_define_method(rb_cImage, "width", _rbf rb_Image_Width, 0);
    rb_define_method(rb_cImage, "height", _rbf rb_Image_Height, 0);
    rb_define_method(rb_cImage, "rect", _rbf rb_Image_Rect, 0);
    rb_define_method(rb_cImage, "copy_to_bitmap", _rbf rb_Image_Copy_to_Bitmap, 1);
    rb_define_method(rb_cImage, "blt", _rbf rb_Image_blt, 4);
    rb_define_method(rb_cImage, "blt!", _rbf rb_Image_blt_fast, 4);
	rb_define_method(rb_cImage, "stretch_blt", _rbf rb_Image_stretch_blt, 3);
	rb_define_method(rb_cImage, "stretch_blt!", _rbf rb_Image_stretch_blt_fast, 3);
	rb_define_method(rb_cImage, "get_pixel", _rbf rb_Image_get_pixel, 2);
	rb_define_method(rb_cImage, "get_pixel_alpha", _rbf rb_Image_get_pixel_alpha, 2);
	rb_define_method(rb_cImage, "set_pixel", _rbf rb_Image_set_pixel, 3);
    rb_define_method(rb_cImage, "clear_rect", _rbf rb_Image_clear_rect, 4);
    rb_define_method(rb_cImage, "fill_rect", _rbf rb_Image_fill_rect, 5);
	rb_define_method(rb_cImage, "create_mask", _rbf rb_Image_create_mask, 2);
    rb_define_method(rb_cImage, "to_png", _rbf rb_Image_toPNG, 0);
    rb_define_method(rb_cImage, "to_png_file", _rbf rb_Image_toPNG_file, 1);
}

VALUE rb_Image_Initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE string;
	VALUE fromMemory;
    auto& img = rb::Get<sf::Image>(self);
    rb_scan_args(argc, argv, "11", &string, &fromMemory);
    /* Load From filename */
    if(NIL_P(fromMemory))
    {
        rb_check_type(string, T_STRING);
		if(!rb_Image_LoadLodePNG(img, RSTRING_PTR(string), 0))
			if(!img.loadFromFile(RSTRING_PTR(string)))
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
			if(!img.loadFromMemory(RSTRING_PTR(string), RSTRING_LEN(string)))
				rb_raise(rb_eRGSSError, "Failed to load image from memory.");
    }
    else
    {
        rb_check_type(string, T_FIXNUM);
        rb_check_type(fromMemory, T_FIXNUM);
        img.create(rb_num2long(string), rb_num2long(fromMemory), sf::Color(0, 0, 0, 0));
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
	return rb::Dispose<sf::Image>(self);
}

VALUE rb_Image_Width(VALUE self)
{
    auto& img = rb::Get<sf::Image>(self);
    sf::Vector2u size = img.getSize();
    return rb_int2inum(size.x);
}

VALUE rb_Image_Height(VALUE self)
{
    auto& img = rb::Get<sf::Image>(self);
    sf::Vector2u size = img.getSize();
    return rb_int2inum(size.y);
}

VALUE rb_Image_Rect(VALUE self)
{
    auto& img = rb::Get<sf::Image>(self);
    sf::Vector2u size = img.getSize();
    VALUE argv[4] = {LONG2FIX(0), LONG2FIX(0), rb_int2inum(size.x), rb_int2inum(size.y)};
    return rb_class_new_instance(4, argv, rb_cRect);
}

VALUE rb_Image_Copy_to_Bitmap(VALUE self, VALUE bitmap)
{
    auto& img = rb::Get<sf::Image>(self);
    rb_Bitmap_getTexture(bitmap).update(img);
    return self;
}

VALUE rb_Image_blt_fast(VALUE self, VALUE x, VALUE y, VALUE src_image, VALUE rect)
{
    auto& img = rb::Get<sf::Image>(self);
    auto& img2 = rb::GetSafe<sf::Image>(src_image, rb_cImage);
    auto& s_rect = rb::GetSafe<CRect_Element>(rect, rb_cRect);
    img.copy(
        img2,
        NUM2ULONG(x),
        NUM2ULONG(y),
        s_rect.getRect()
    );
    return self;
}

VALUE rb_Image_blt(VALUE self, VALUE x, VALUE y, VALUE src_image, VALUE rect)
{
    auto& img = rb::Get<sf::Image>(self);
    auto& img2 = rb::GetSafe<sf::Image>(src_image, rb_cImage);
    auto& s_rect = rb::GetSafe<CRect_Element>(rect, rb_cRect);
    img.copy(
        img2,
        NUM2ULONG(x),
        NUM2ULONG(y),
        s_rect.getRect(),
		true
    );
    return self;
}

VALUE rb_Image_clear_rect(VALUE self, VALUE x, VALUE y, VALUE width, VALUE height)
{
	auto& img = rb::Get<sf::Image>(self);
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
			img.setPixel(x3, y1, clr);
		y1++;
	}
	return self;
}

VALUE rb_Image_fill_rect(VALUE self, VALUE x, VALUE y, VALUE width, VALUE height, VALUE color)
{
	auto& img = rb::Get<sf::Image>(self);
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
			img.setPixel(x3, y1, *rcolor);
		y1++;
	}
	return self;
}

VALUE rb_Image_toPNG(VALUE self)
{
	auto& img = rb::Get<sf::Image>(self);
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

VALUE rb_Image_toPNG_file(VALUE self, VALUE filename)
{
	rb_check_type(filename, T_STRING);
	auto& img = rb::Get<sf::Image>(self);
	if (lodepng_encode32_file(RSTRING_PTR(filename), img.getPixelsPtr(), img.getSize().x, img.getSize().y) != 0)
		return Qfalse;
	return Qtrue;
}

VALUE rb_Image_get_pixel(VALUE self, VALUE x, VALUE y)
{
	auto& img = rb::Get<sf::Image>(self);
	unsigned long px = NUM2ULONG(x);
	unsigned long py = NUM2ULONG(y);
	sf::Vector2u size = img.getSize();
	if (px < size.x && py < size.y)
	{
		sf::Color color = img.getPixel(px, py);
		VALUE argv[4] = { LONG2FIX(color.r), LONG2FIX(color.g), LONG2FIX(color.b), LONG2FIX(color.a) };
		return rb_class_new_instance(4, argv, rb_cColor);
	}
	return Qnil;
}

VALUE rb_Image_get_pixel_alpha(VALUE self, VALUE x, VALUE y)
{
	auto& img = rb::Get<sf::Image>(self);
	unsigned long px = NUM2ULONG(x);
	unsigned long py = NUM2ULONG(y);
	sf::Vector2u size = img.getSize();
	if (px < size.x && py < size.y)
	{
		sf::Color color = img.getPixel(px, py);
		return LONG2FIX(color.a);
	}
	return LONG2FIX(0);
}

VALUE rb_Image_set_pixel(VALUE self, VALUE x, VALUE y, VALUE color)
{
	auto& img = rb::Get<sf::Image>(self);
	unsigned long px = NUM2ULONG(x);
	unsigned long py = NUM2ULONG(y);
	sf::Vector2u size = img.getSize();
	if (px < size.x && py < size.y)
	{
		auto& color2 = rb::GetSafe<sf::Color>(color, rb_cColor);
		img.setPixel(px, py, color2);
	}
	return self;
}

VALUE rb_Image_stretch_blt_fast(VALUE self, VALUE dest_rect, VALUE src_image, VALUE src_rect)
{
	auto& img = rb::Get<sf::Image>(self);
	auto& dst_rc = rb::GetSafe<CRect_Element>(dest_rect, rb_cRect);
	auto& src_rc = rb::GetSafe<CRect_Element>(src_rect, rb_cRect);
	sf::Image& src_img = rb::GetSafe<sf::Image>(src_image, rb_cImage);
	int s_w = src_rc.getRect().width;
	int d_w = dst_rc.getRect().width;
	int s_h = src_rc.getRect().height;
	int d_h = dst_rc.getRect().height;
	int os_x = src_rc.getRect().left;
	int os_y = src_rc.getRect().top;
	int od_x = dst_rc.getRect().left;
	int od_y = dst_rc.getRect().top;
	sf::Vector2u simg_size = src_img.getSize();
	sf::Vector2u dimg_size = img.getSize();
	int s_x, s_y, d_x, d_y;
	for (int y = 0; y < d_h; y++)
	{
		d_y = y + od_y;
		if (d_y >= dimg_size.y)
			break;
		if (d_y < 0)
			continue;
		s_y = y * s_h / d_h + os_y;
		if (s_y >= simg_size.y)
			break;
		if (s_y < 0)
			continue;
		for (int x = 0; x < d_w; x++)
		{
			d_x = x + od_x;
			if (d_x >= dimg_size.x)
				break;
			if (d_x < 0)
				continue;
			s_x = x * s_w / d_w + os_x;
			if (s_x >= simg_size.x)
				break;
			if (s_x < 0)
				continue;
			img.setPixel(d_x, d_y, src_img.getPixel(s_x, s_y));
		}
	}
	return self;
}

VALUE rb_Image_stretch_blt(VALUE self, VALUE dest_rect, VALUE src_image, VALUE src_rect)
{
	auto& img = rb::Get<sf::Image>(self);
	auto& dst_rc = rb::GetSafe<CRect_Element>(dest_rect, rb_cRect);
	auto& src_rc = rb::GetSafe<CRect_Element>(src_rect, rb_cRect);
	sf::Image& src_img = rb::GetSafe<sf::Image>(src_image, rb_cImage);
	sf::Color src, dest;
	int s_w = src_rc.getRect().width;
	int d_w = dst_rc.getRect().width;
	int s_h = src_rc.getRect().height;
	int d_h = dst_rc.getRect().height;
	int os_x = src_rc.getRect().left;
	int os_y = src_rc.getRect().top;
	int od_x = dst_rc.getRect().left;
	int od_y = dst_rc.getRect().top;
	sf::Vector2u simg_size = src_img.getSize();
	sf::Vector2u dimg_size = img.getSize();
	int s_x, s_y, d_x, d_y;
	unsigned char mina;
	for (int y = 0; y < d_h; y++)
	{
		d_y = y + od_y;
		if (d_y >= dimg_size.y)
			break;
		if (d_y < 0)
			continue;
		s_y = y * s_h / d_h + os_y;
		if (s_y >= simg_size.y)
			break;
		if (s_y < 0)
			continue;
		for (int x = 0; x < d_w; x++)
		{
			d_x = x + od_x;
			if (d_x >= dimg_size.x)
				break;
			if (d_x < 0)
				continue;
			s_x = x * s_w / d_w + os_x;
			if (s_x >= simg_size.x)
				break;
			if (s_x < 0)
				continue;
			src = src_img.getPixel(s_x, s_y);
			dest = img.getPixel(d_x, d_y);
			if(dest.a > 0)
			{
				mina = 255 - src.a;
				src.r = (src.r * src.a + dest.r * mina) / 255;
				src.g = (src.g * src.a + dest.g * mina) / 255;
				src.b = (src.b * src.a + dest.b * mina) / 255;
				src.a += (dest.a * mina / 255);
			}
			img.setPixel(d_x, d_y, src);
		}
	}
	return self;
}

VALUE rb_Image_create_mask(VALUE self, VALUE color, VALUE alpha)
{
	auto& img = rb::Get<sf::Image>(self);
	auto& col = rb::GetSafe<sf::Color>(color, rb_cColor);
	img.createMaskFromColor(col, NUM2ULONG(alpha));
	return self;
}

bool rb_Image_LoadLodePNG(sf::Image& img, char* str, long from_memory_size)
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
	img.create(w, h, reinterpret_cast<sf::Uint8*>(out));
	free(out);
	return true;
}
