#ifndef GR_HEADER
#define GR_HEADER
#include "CViewport_Element.h"
#include "CSprite_Element.h"
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
VALUE rb_Graphics_setWindowFramerate(VALUE self, VALUE framerate);

void global_Graphics_Bind(CDrawable_Element* element);

extern unsigned long Graphics_Scale;
#endif