#ifndef GR_HEADER
#define GR_HEADER
#include "CViewport_Element.h"
#include "CSprite_Element.h"
#include "CShaderSprite_Element.h"
#include "CShape_Element.h"
#include "CWindow_Element.h"
#include <SFML/Graphics.hpp>

VALUE rb_Graphics_start(VALUE self);
VALUE rb_Graphics_stop(VALUE self);
VALUE rb_Graphics_update(VALUE self);
VALUE rb_Graphics_snap_to_bitmap(VALUE self);
VALUE rb_Graphics_freeze(VALUE self);
VALUE rb_Graphics_transition(int argc, VALUE* argv, VALUE self);
VALUE rb_Graphics_list_res(VALUE self);
VALUE rb_Graphics_get_frame_count(VALUE self);
VALUE rb_Graphics_set_frame_count(VALUE self, VALUE val);
VALUE rb_Graphics_width(VALUE self);
VALUE rb_Graphics_height(VALUE self);
VALUE rb_Graphics_ReloadStack(VALUE self);
VALUE rb_Graphics_update_no_input_count(VALUE self);
VALUE rb_Graphics_update_only_input(VALUE self);
VALUE rb_Graphics_getBrightness(VALUE self);
VALUE rb_Graphics_setBrightness(VALUE self, VALUE brightness);
VALUE rb_Graphics_getShader(VALUE self);
VALUE rb_Graphics_setShader(VALUE self, VALUE shader);
VALUE rb_Graphics_resize_screen(VALUE self, VALUE width, VALUE height);

#endif