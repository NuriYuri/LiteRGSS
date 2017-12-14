#ifndef GR_HEADER
#define GR_HEADER
#include "CViewport_Element.h"
#include "CSprite_Element.h"
#include <SFML/Graphics.hpp>

VALUE rb_Graphics_start(VALUE self);
VALUE rb_Graphics_stop(VALUE self);
VALUE rb_Graphics_update(VALUE self);
VALUE rb_Graphics_snap_to_bitmap(VALUE self);

void __Graphics_Bind(CDrawable_Element* element);

#endif