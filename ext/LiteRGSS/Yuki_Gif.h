#ifndef YUKI_GIF_H
#define YUKI_GIF_H
#include "libnsgif.hpp"

#define BYTES_PER_PIXEL 4
#define MAX_IMAGE_BYTES (48 * 1024 * 1024)

VALUE rb_Yuki_GifReader_Initialize(int argc, VALUE *argv, VALUE self);
VALUE rb_Yuki_GifReader_Update(VALUE self, VALUE bitmap);
VALUE rb_Yuki_GifReader_Draw(VALUE self, VALUE bitmap);
VALUE rb_Yuki_GifReader_Width(VALUE self);
VALUE rb_Yuki_GifReader_Height(VALUE self);
VALUE rb_Yuki_GifReader_Frame(VALUE self);
VALUE rb_Yuki_GifReader_Frame_set(VALUE self, VALUE frame);
VALUE rb_Yuki_GifReader_FrameCount(VALUE self);
VALUE rb_Yuki_GifReader_Copy(VALUE self);

VALUE rb_Yuki_GifReader_SetDeltaCounter(VALUE self, VALUE delta);

struct rb_yuki_gif_data {
	gif_animation gif;
	unsigned long frame = 0;
	double counter = 0.0;
};

namespace meta {
    template<>
    struct Log<rb_yuki_gif_data> {
        static constexpr auto classname = "GIF";
    };
}

#endif // !YUKI_GIF_H
