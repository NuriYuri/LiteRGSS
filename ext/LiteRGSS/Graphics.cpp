#include "utils/ruby_common.h"
#include "utils/common.h"
#include "Graphics.h"
#include "CGraphics.h"
#include "CBitmap_Element.h"
#include "CViewport_Element.h"

VALUE rb_mGraphics = Qnil;
VALUE rb_eStoppedGraphics = Qnil;
VALUE rb_eClosedWindow = Qnil;
ID rb_iGraphicsShader = Qnil;

void Init_Graphics()
{
    rb_mGraphics = rb_define_module_under(rb_mLiteRGSS, "Graphics");
    /* Defining the Stopped Graphics Error */
    rb_eStoppedGraphics = rb_define_class_under(rb_mGraphics, "StoppedError", rb_eStandardError);
    rb_eClosedWindow = rb_define_class_under(rb_mGraphics, "ClosedWindowError", rb_eStandardError);
    /* Defining the Graphics functions */
    rb_define_module_function(rb_mGraphics, "start", _rbf rb_Graphics_start, 0);
    rb_define_module_function(rb_mGraphics, "stop", _rbf rb_Graphics_stop, 0);
    rb_define_module_function(rb_mGraphics, "update", _rbf rb_Graphics_update, 0);
    rb_define_module_function(rb_mGraphics, "snap_to_bitmap", _rbf rb_Graphics_snap_to_bitmap, 0);
    rb_define_module_function(rb_mGraphics, "freeze", _rbf rb_Graphics_freeze, 0);
    rb_define_module_function(rb_mGraphics, "transition", _rbf rb_Graphics_transition, -1);
    rb_define_module_function(rb_mGraphics, "list_resolutions", _rbf rb_Graphics_list_res, 0);
    rb_define_module_function(rb_mGraphics, "frame_count", _rbf rb_Graphics_get_frame_count, 0);
    rb_define_module_function(rb_mGraphics, "frame_count=", _rbf rb_Graphics_set_frame_count, 1);
    rb_define_module_function(rb_mGraphics, "width", _rbf rb_Graphics_width, 0);
    rb_define_module_function(rb_mGraphics, "height", _rbf rb_Graphics_height, 0);
    rb_define_module_function(rb_mGraphics, "reload_stack", _rbf rb_Graphics_ReloadStack, 0);
    rb_define_module_function(rb_mGraphics, "update_no_input", _rbf rb_Graphics_update_no_input_count, 0);
    rb_define_module_function(rb_mGraphics, "update_only_input", _rbf rb_Graphics_update_only_input, 0);
    rb_define_module_function(rb_mGraphics, "brightness", _rbf rb_Graphics_getBrightness, 0);
    rb_define_module_function(rb_mGraphics, "brightness=", _rbf rb_Graphics_setBrightness, 1);
    rb_define_module_function(rb_mGraphics, "shader", _rbf rb_Graphics_getShader, 0);
    rb_define_module_function(rb_mGraphics, "shader=", _rbf rb_Graphics_setShader, 1);
	rb_define_module_function(rb_mGraphics, "resize_screen", _rbf rb_Graphics_resize_screen, 2);
    /* creating the element table */
    //rb_ivar_set(rb_mGraphics, rb_iElementTable, rb_ary_new());
	rb_iGraphicsShader = rb_intern("@__GraphicsShader");
    /* Store the max texture size */
	rb_define_const(rb_mGraphics, "MAX_TEXTURE_SIZE", LONG2FIX(sf::Texture::getMaximumSize()));
}

VALUE rb_Graphics_start(VALUE self) {
    return CGraphics::Get().init(self);
}

VALUE rb_Graphics_stop(VALUE self) {
    CGraphics::Get().stop();
    return self;
}

VALUE rb_Graphics_snap_to_bitmap(VALUE self) {
    return CGraphics::Get().takeSnapshot();
}

VALUE rb_Graphics_freeze(VALUE self) {
    return CGraphics::Get().freeze(self);
}

VALUE rb_Graphics_transition(int argc, VALUE* argv, VALUE self) {
    CGraphics::Get().transition(self, argc, argv);
    return self;
}


VALUE rb_Graphics_list_res(VALUE self)
{
    VALUE array = rb_ary_new();
    auto modes = sf::VideoMode::getFullscreenModes();
    for(int i = 0; i < modes.size(); i++) {
        if(modes[i].bitsPerPixel == 32)
            rb_ary_push(array, rb_ary_new3(2, rb_int2inum(modes[i].width), rb_int2inum(modes[i].height)));
    }
    return array;
}

VALUE rb_Graphics_update(VALUE self)
{
    return CGraphics::Get().update(self);
}

VALUE rb_Graphics_update_no_input_count(VALUE self)
{
    return CGraphics::Get().update(self, false);
}

VALUE rb_Graphics_update_only_input(VALUE self)
{
	return CGraphics::Get().updateOnlyInput(self);
}

VALUE rb_Graphics_get_frame_count(VALUE self)
{
    return RB_UINT2NUM(CGraphics::Get().frameCount());
}

VALUE rb_Graphics_set_frame_count(VALUE self, VALUE val)
{
    auto frame_count = rb_num2ulong(val);
    CGraphics::Get().setFrameCount(frame_count);
    return val;
}

VALUE rb_Graphics_width(VALUE self)
{
    return rb_int2inum(CGraphics::Get().screenWidth());
}

VALUE rb_Graphics_height(VALUE self)
{
    return rb_int2inum(CGraphics::Get().screenHeight());
}

VALUE rb_Graphics_ReloadStack(VALUE self)
{
    CGraphics::Get().reloadStack();
    return self;
}

VALUE rb_Graphics_getBrightness(VALUE self)
{
	return LONG2FIX(CGraphics::Get().brightness());
}

VALUE rb_Graphics_setBrightness(VALUE self, VALUE brightness)
{
    CGraphics::Get().setBrightness(normalize_long(rb_num2long(brightness), 0, 255));
	return self;
}

VALUE rb_Graphics_getShader(VALUE self)
{
	return rb_ivar_get(self, rb_iGraphicsShader);
}

VALUE rb_Graphics_setShader(VALUE self, VALUE shader)
{
	sf::RenderStates* render_state;
	if (rb_obj_is_kind_of(shader, rb_cBlendMode) == Qtrue)
	{
		rb_ivar_set(self, rb_iGraphicsShader, shader);
		Data_Get_Struct(shader, sf::RenderStates, render_state);
		CGraphics::Get().setShader(render_state);
	}
	else if (shader == Qnil)
	{
		CGraphics::Get().setShader(nullptr);
	}
    return self;
}

VALUE rb_Graphics_resize_screen(VALUE self, VALUE width, VALUE height)
{
	CGraphics::Get().resizeScreen(self, width, height);
	return self;
}

void global_Graphics_Bind(VALUE rubyElement, CDrawable_Element& element)
{
    CGraphics::Get().bind(rubyElement, element);
}
