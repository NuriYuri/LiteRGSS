#ifndef L_IMAGE_HEADER
#define L_IMAGE_HEADER
#include "ruby.h"
#include <SFML/Graphics.hpp>
#include "lodepng.h"

VALUE rb_Image_Initialize(int argc, VALUE *argv, VALUE self);
VALUE rb_Image_Initialize_Copy(VALUE self, VALUE other);
VALUE rb_Image_Dispose(VALUE self);
VALUE rb_Image_Disposed(VALUE self);
VALUE rb_Image_Width(VALUE self);
VALUE rb_Image_Height(VALUE self);
VALUE rb_Image_Rect(VALUE self);
VALUE rb_Image_Copy_to_Bitmap(VALUE self, VALUE bitmap);
VALUE rb_Image_blt(VALUE self, VALUE x, VALUE y, VALUE src_image, VALUE rect);
VALUE rb_Image_blt_fast(VALUE self, VALUE x, VALUE y, VALUE src_image, VALUE rect);
VALUE rb_Image_clear_rect(VALUE self, VALUE x, VALUE y, VALUE width, VALUE height);
VALUE rb_Image_fill_rect(VALUE self, VALUE x, VALUE y, VALUE width, VALUE height, VALUE color);
VALUE rb_Image_toPNG(VALUE self);
VALUE rb_Image_toPNG_file(VALUE self, VALUE filename);
VALUE rb_Image_get_pixel(VALUE self, VALUE x, VALUE y);
VALUE rb_Image_set_pixel(VALUE self, VALUE x, VALUE y, VALUE color);
VALUE rb_Image_get_pixel_alpha(VALUE self, VALUE x, VALUE y);
VALUE rb_Image_stretch_blt(VALUE self, VALUE dest_rect, VALUE src_image, VALUE src_rect);
VALUE rb_Image_stretch_blt_fast(VALUE self, VALUE dest_rect, VALUE src_image, VALUE src_rect);
VALUE rb_Image_create_mask(VALUE self, VALUE color, VALUE alpha);

bool rb_Image_LoadLodePNG(sf::Image& img, char* str, long from_memory_size);

namespace meta {
    template<>
    struct Log<sf::Image> {
        static constexpr auto classname = "Image";
    };
}

#endif