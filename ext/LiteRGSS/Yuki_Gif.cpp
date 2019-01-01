#include "LiteRGSS.h"

VALUE rb_cYukiGifReader = Qnil;
VALUE rb_cYukiGifError = Qnil;
double rb_YukiGifReaderFrameDelta = 16.666666666666666;

#define GIF_PROTECT if(RDATA(self)->data == nullptr) \
{\
    rb_raise(rb_eRGSSError, "Disposed GIF."); \
    return self; \
}

#define GET_GIF rb_yuki_gif_data* gif; \
    Data_Get_Struct(self, rb_yuki_gif_data, gif); \
    GIF_PROTECT \

void rb_Yuki_Gif_Free(void* data)
{
	rb_yuki_gif_data* gif = reinterpret_cast<rb_yuki_gif_data*>(data);
	if (gif != nullptr)
	{
		gif_finalise(&gif->gif);
		delete gif;
	}
}

//--- LibNgif util functions ---
static void *bitmap_create(int width, int height)
{
	/* ensure a stupidly large bitmap is not created */
	if (((long long)width * (long long)height) > (MAX_IMAGE_BYTES / BYTES_PER_PIXEL)) {
		return NULL;
	}
	return calloc(width * height, BYTES_PER_PIXEL);
}

static void bitmap_set_opaque(void *bitmap, bool opaque)
{
	// (void)opaque;  /* unused */
	// (void)bitmap;  /* unused */
	// assert(bitmap);
}


static bool bitmap_test_opaque(void *bitmap)
{
	//(void)bitmap;  /* unused */
	//assert(bitmap);
	return false;
}

static unsigned char *bitmap_get_buffer(void *bitmap)
{
	//assert(bitmap);
	return reinterpret_cast<unsigned char*>(bitmap);
}

static void bitmap_destroy(void *bitmap)
{
	//assert(bitmap);
	free(bitmap);
}

static void bitmap_modified(void *bitmap)
{
	//(void)bitmap;  /* unused */
	//assert(bitmap);
	return;
}

static gif_bitmap_callback_vt bitmap_callbacks = {
	bitmap_create,
	bitmap_destroy,
	bitmap_get_buffer,
	bitmap_set_opaque,
	bitmap_test_opaque,
	bitmap_modified
};

VALUE rb_Yuki_Gif_Alloc(VALUE klass)
{
	rb_yuki_gif_data* gif = new rb_yuki_gif_data();
	gif->frame = 0;
	gif->counter = 0;
	gif_create(&gif->gif, &bitmap_callbacks);
	return Data_Wrap_Struct(klass, NULL, rb_Yuki_Gif_Free, gif);
}

void Init_YukiGifReader()
{
	rb_cYukiGifReader = rb_define_class_under(rb_mYuki, "GifReader", rb_cObject);
	rb_define_alloc_func(rb_cYukiGifReader, rb_Yuki_Gif_Alloc);
	rb_cYukiGifError = rb_define_class_under(rb_cYukiGifReader, "Error", rb_eStandardError);
	rb_define_method(rb_cYukiGifReader, "initialize", _rbf rb_Yuki_GifReader_Initialize, -1);
	rb_define_method(rb_cYukiGifReader, "update", _rbf rb_Yuki_GifReader_Update, 1);
	rb_define_method(rb_cYukiGifReader, "draw", _rbf rb_Yuki_GifReader_Draw, 1);
	rb_define_method(rb_cYukiGifReader, "width", _rbf rb_Yuki_GifReader_Width, 0);
	rb_define_method(rb_cYukiGifReader, "height", _rbf rb_Yuki_GifReader_Height, 0);
	rb_define_method(rb_cYukiGifReader, "frame", _rbf rb_Yuki_GifReader_Frame, 0);
	rb_define_method(rb_cYukiGifReader, "frame=", _rbf rb_Yuki_GifReader_Frame_set, 1);
	rb_define_method(rb_cYukiGifReader, "frame_count", _rbf rb_Yuki_GifReader_FrameCount, 0);

	rb_define_method(rb_cYukiGifReader, "clone", _rbf rb_Yuki_GifReader_Copy, 0);
	rb_define_method(rb_cYukiGifReader, "dup", _rbf rb_Yuki_GifReader_Copy, 0);

	rb_define_singleton_method(rb_cYukiGifReader, "delta_counter=", _rbf rb_Yuki_GifReader_SetDeltaCounter, 1);
}

VALUE rb_Yuki_GifReader_Initialize(int argc, VALUE *argv, VALUE self)
{
	GET_GIF;
	VALUE str, from_memory;
	rb_scan_args(argc, argv, "11", &str, &from_memory);
	rb_check_type(str, T_STRING);
	if (RTEST(from_memory))
	{
		rb_str_freeze(str);
		rb_iv_set(self, "@__gif_data", str);
		gif_result code = gif_initialise(&gif->gif, RSTRING_LEN(str), reinterpret_cast<unsigned char*>(RSTRING_PTR(str)));
		if (code != GIF_OK && code != GIF_WORKING)
		{
			rb_raise(rb_cYukiGifError, "Failed to load GIF from Memory");
		}
	}
	else
	{
		VALUE file = rb_file_open(RSTRING_PTR(str), "rb");
		VALUE data = rb_funcall(file, rb_intern("size"), 0);
		data = rb_funcall(file, rb_intern("read"), 1, data);
		rb_io_close(file);
		rb_str_freeze(data);
		rb_iv_set(self, "@__gif_data", data);
		gif_result code = gif_initialise(&gif->gif, RSTRING_LEN(data), reinterpret_cast<unsigned char*>(RSTRING_PTR(data)));
		if (code != GIF_OK && code != GIF_WORKING)
		{
			rb_raise(rb_cYukiGifError, "Failed to load GIF from File (%s)", RSTRING_PTR(str));
		}
	}
	return self;
}

VALUE rb_Yuki_GifReader_Update(VALUE self, VALUE bitmap)
{
	GET_GIF;
	gif_animation* gifa = &gif->gif;
	if (((gifa->frames[gif->frame].frame_delay * 10) <= gif->counter) || (gif->frame == 0 && gif->counter == 0))
	{
		if (gif->frame != 0 || gif->counter != 0)
		{
			gif->counter -= gifa->frames[gif->frame].frame_delay * 10;
			gif->frame++;
			if (gif->frame >= gifa->frame_count)
				gif->frame = 0;
		}
		if (gif_decode_frame(gifa, gif->frame) == GIF_OK)
		{
			rb_Yuki_GifReader_Draw(self, bitmap);
		}
		else
			rb_raise(rb_cYukiGifError, "Failed to decode GIF frame");
	}
	gif->counter += rb_YukiGifReaderFrameDelta;
	return self;
}

VALUE rb_Yuki_GifReader_Draw(VALUE self, VALUE bitmap)
{
	GET_GIF;
	if (rb_obj_is_kind_of(bitmap, rb_cBitmap) == Qtrue)
	{
		sf::Texture& text = rb_Bitmap_getTexture(bitmap);
		text.update(reinterpret_cast<sf::Uint8*>(gif->gif.frame_image), gif->gif.width, gif->gif.height, 0, 0);
	}
	return self;
}

VALUE rb_Yuki_GifReader_Width(VALUE self)
{
	GET_GIF;
	return UINT2NUM(gif->gif.width);
}

VALUE rb_Yuki_GifReader_Height(VALUE self)
{
	GET_GIF;
	return UINT2NUM(gif->gif.height);
}

VALUE rb_Yuki_GifReader_Frame(VALUE self)
{
	GET_GIF;
	return ULONG2NUM(gif->frame);
}

VALUE rb_Yuki_GifReader_Frame_set(VALUE self, VALUE frame)
{
	GET_GIF;
	gif->frame = NUM2ULONG(frame);
	return self;
}

VALUE rb_Yuki_GifReader_FrameCount(VALUE self)
{
	GET_GIF;
	return UINT2NUM(gif->gif.frame_count);
}

VALUE rb_Yuki_GifReader_SetDeltaCounter(VALUE self, VALUE delta)
{
	rb_YukiGifReaderFrameDelta = NUM2DBL(delta);
	return delta;
}

VALUE rb_Yuki_GifReader_Copy(VALUE self)
{
	rb_raise(rb_eRGSSError, "Gif cannot be cloned or duplicated.");
	return self;
}