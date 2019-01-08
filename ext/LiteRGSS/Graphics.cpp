#include "LiteRGSS.h"
#include "ruby/thread.h"
#include "Graphics.local.h"
#include "CBitmap_Element.h"
#include "CViewport_Element.h"

/* Variables definition */
VALUE rb_mGraphics = Qnil;
VALUE rb_eStoppedGraphics = Qnil;
VALUE rb_eClosedWindow = Qnil;
ID rb_iGraphicsShader = Qnil;

long ScreenWidth = 640;
long ScreenHeight = 480;
unsigned long frame_count = 0;
unsigned long frame_rate = 60;

std::unique_ptr<sf::RenderWindow> game_window = nullptr;
bool InsideGraphicsUpdate = false;
std::vector<CDrawable_Element*> Graphics_stack;

std::unique_ptr<sf::Texture> Graphics_freeze_texture = nullptr;
std::unique_ptr<sf::Sprite> Graphics_freeze_sprite = nullptr;
std::unique_ptr<sf::Shader> Graphics_freeze_shader = nullptr;

void GraphicProtect()  {
    if(game_window == nullptr) {
        constexpr auto rawErrorMessage = "Graphics is not started, window closed thus no Graphics operation allowed. Please call Graphics.start before using other Graphics functions.";
        rb_raise(rb_eStoppedGraphics, rawErrorMessage);
        throw std::runtime_error(rawErrorMessage);
    }
}

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
    rb_ivar_set(rb_mGraphics, rb_iElementTable, rb_ary_new());
	rb_iGraphicsShader = rb_intern("@__GraphicsShader");
    /* Store the max texture size */
	rb_define_const(rb_mGraphics, "MAX_TEXTURE_SIZE", LONG2FIX(sf::Texture::getMaximumSize()));
}

VALUE rb_Graphics_start(VALUE self)
{
    unsigned int framerate;
    if(game_window != nullptr)
        return Qnil;
    /* Shader Testing */
    if (!sf::Shader::isAvailable())
        rb_raise(rb_eRGSSError, "Shaders are not available :(");
    /* Window Loading */
    sf::VideoMode vmode(640, 480, 32);
    local_LoadVideoModeFromConfigs(vmode);
    local_LoadSmoothScreenFromConfigs();
    sf::Uint32 style = sf::Style::Close | sf::Style::Titlebar; // sf::Style::Resize = text issues !
    if(local_LoadFullScreenFromConfigs())
        style = sf::Style::Fullscreen;
    game_window = std::make_unique<sf::RenderWindow>(vmode, local_LoadTitleFromConfigs(), style);
    game_window->setMouseCursorVisible(false);
	framerate = local_LoadFrameRateFromConfigs();
	/* VSYNC choice */
	if(local_LoadVSYNCFromConfigs())
		game_window->setVerticalSyncEnabled(true);
	else
	{
		game_window->setVerticalSyncEnabled(false);
		game_window->setFramerateLimit(framerate);
	}
	/* Input adjustement */
    L_Input_Reset_Clocks();
    L_Input_Setusec_threshold(1000000 / framerate);
    frame_count = 0;
	frame_rate = framerate;
	/* Shader loading */
	local_Graphics_LoadShader();
    return self;
}

VALUE rb_Graphics_stop(VALUE self)
{
    GraphicProtect();

    // Prevent a Thread from calling stop during the Graphics.update process
    if(InsideGraphicsUpdate) 
        return self;

    /* Clear the stack */
    local_Graphics_Clear_Stack();

    GraphicProtect();

    /* Close the window */
    game_window->close();
    game_window = nullptr;

    /* Unfreezing Graphics */    
    Graphics_freeze_sprite = nullptr;    
    Graphics_freeze_texture = nullptr;
    Graphics_freeze_shader = nullptr;
    return self;
}

VALUE rb_Graphics_snap_to_bitmap(VALUE self)
{
    GraphicProtect();
    if(InsideGraphicsUpdate)
        return Qnil;
    VALUE bmp = rb_obj_alloc(rb_cBitmap);
    CBitmap_Element* bitmap;
    Data_Get_Struct(bmp, CBitmap_Element, bitmap);
    if(bitmap == nullptr)
        return Qnil;
    sf::Texture& text = bitmap->getTexture();
    local_Graphics_Take_Snapshot(text);
    return bmp;
}

VALUE rb_Graphics_freeze(VALUE self)
{
    GraphicProtect();
    if(Graphics_freeze_texture != nullptr)
        return self;
    rb_Graphics_update(self);
    Graphics_freeze_texture = std::make_unique<sf::Texture>();
    local_Graphics_Take_Snapshot(*Graphics_freeze_texture);
    Graphics_freeze_sprite = std::make_unique<sf::Sprite>(*Graphics_freeze_texture);
    Graphics_freeze_sprite->setScale(1.0f / Graphics_Scale, 1.0f / Graphics_Scale);
    return self;
}

VALUE rb_Graphics_transition(int argc, VALUE* argv, VALUE self)
{
    GraphicProtect();
    if(Graphics_freeze_sprite == nullptr)
        return self;
	long time = 8; //< RGSS doc
    if(argc >= 1)
		time = rb_num2long(argv[0]);
    time = normalize_long(time, 1, 0xFFFF);
	if (argc < 2 || rb_obj_is_kind_of(argv[1], rb_cBitmap) != Qtrue)
		local_Graphics_TransitionBasic(self, time);
	else
		local_Graphics_TransitionRGSS(self, time, argv[1]);
    
    Graphics_freeze_sprite = nullptr;
    Graphics_freeze_texture = nullptr;
    return self;
}


VALUE rb_Graphics_list_res(VALUE self)
{
    VALUE array = rb_ary_new();
    auto modes = sf::VideoMode::getFullscreenModes();
    for(int i = 0; i < modes.size(); i++)
    {
        if(modes[i].bitsPerPixel == 32)
            rb_ary_push(array, rb_ary_new3(2, rb_int2inum(modes[i].width), rb_int2inum(modes[i].height)));
    }
    return array;
}

VALUE rb_Graphics_update(VALUE self)
{
    GraphicProtect();
    if(InsideGraphicsUpdate) // Prevent a Thread from calling Graphics.update during Graphics.update process
        return self;
    InsideGraphicsUpdate = true;
    /* Graphics.update real process */
    GraphicUpdateMessage* message = 
        reinterpret_cast<GraphicUpdateMessage*>(rb_thread_call_without_gvl(local_Graphics_Update_Internal, static_cast<void*>(&Graphics_stack), NULL, NULL));
    /* Message Processing */
    local_Graphics_Update_Process_Event(message);
    if(message != nullptr)
        return local_Graphics_Update_RaiseError(self, message);
    /* End of Graphics.update process */
    InsideGraphicsUpdate = false;
    /* Update the frame count */
    frame_count++;
    return self;
}

VALUE rb_Graphics_update_no_input_count(VALUE self)
{
	GraphicProtect();
		if (InsideGraphicsUpdate) // Prevent a Thread from calling Graphics.update during Graphics.update process
			return self;
	InsideGraphicsUpdate = true;
	/* Graphics.update real process */
	GraphicUpdateMessage* message =
		reinterpret_cast<GraphicUpdateMessage*>(rb_thread_call_without_gvl(local_Graphics_Update_Internal, static_cast<void*>(&Graphics_stack), NULL, NULL));
	if (message != nullptr)
		return local_Graphics_Update_RaiseError(self, message);
	/* End of Graphics.update process */
	InsideGraphicsUpdate = false;
	return self;
}

VALUE rb_Graphics_update_only_input(VALUE self)
{
	GraphicProtect();
	InsideGraphicsUpdate = true;
	GraphicUpdateMessage* message = nullptr;
	local_Graphics_Update_Process_Event(message);
	if (message != nullptr)
		return local_Graphics_Update_RaiseError(self, message);
	InsideGraphicsUpdate = false;
	return self;
}

VALUE rb_Graphics_get_frame_count(VALUE self)
{
    return RB_UINT2NUM(frame_count);
}

VALUE rb_Graphics_set_frame_count(VALUE self, VALUE val)
{
    frame_count = rb_num2ulong(val);
    return val;
}

VALUE rb_Graphics_width(VALUE self)
{
    return rb_int2inum(ScreenWidth);
}

VALUE rb_Graphics_height(VALUE self)
{
    return rb_int2inum(ScreenHeight);
}

VALUE rb_Graphics_ReloadStack(VALUE self)
{
    VALUE table = rb_ivar_get(rb_mGraphics, rb_iElementTable);
    rb_check_type(table, T_ARRAY);
    for(auto& graphic : Graphics_stack) {
        graphic->overrideOrigineStack();
    }
    Graphics_stack.clear();
    long sz = RARRAY_LEN(table);
    VALUE* ori = RARRAY_PTR(table);
    for(long i = 0; i < sz; i++)
    {
        if(rb_obj_is_kind_of(ori[i], rb_cViewport) == Qtrue ||
            rb_obj_is_kind_of(ori[i], rb_cSprite) == Qtrue ||
            rb_obj_is_kind_of(ori[i], rb_cText) == Qtrue ||
			rb_obj_is_kind_of(ori[i], rb_cWindow) == Qtrue)
        {
            if(RDATA(ori[i])->data != nullptr)
            {
                global_Graphics_Bind(reinterpret_cast<CDrawable_Element*>(RDATA(ori[i])->data));
            }
        }
    }
    return self;
}

VALUE rb_Graphics_getBrightness(VALUE self)
{
	return LONG2FIX(Graphics_Brightness);
}

VALUE rb_Graphics_setBrightness(VALUE self, VALUE brightness)
{
	Graphics_Brightness = normalize_long(rb_num2long(brightness), 0, 255);
	return self;
}

VALUE rb_Graphics_getShader(VALUE self)
{
	return rb_ivar_get(self, rb_iGraphicsShader);
}

VALUE rb_Graphics_setShader(VALUE self, VALUE shader)
{
	GraphicProtect();
	sf::RenderStates* render_state;
	if (rb_obj_is_kind_of(shader, rb_cBlendMode) == Qtrue)
	{
		rb_ivar_set(self, rb_iGraphicsShader, shader);
		Data_Get_Struct(shader, sf::RenderStates, render_state);
		Graphics_States = render_state;
		local_Graphics_initRender();
	}
    return self;
}

VALUE rb_Graphics_resize_screen(VALUE self, VALUE width, VALUE height)
{
    GraphicProtect();
	ID swidth = rb_intern("ScreenWidth");
	ID sheight = rb_intern("ScreenHeight");
	/* Adjust screen resolution */
	if (rb_const_defined(rb_mConfig, swidth))
		rb_const_remove(rb_mConfig, swidth);
	if (rb_const_defined(rb_mConfig, sheight))
		rb_const_remove(rb_mConfig, sheight);
	rb_const_set(rb_mConfig, swidth, INT2NUM(NUM2INT(width)));
	rb_const_set(rb_mConfig, sheight, INT2NUM(NUM2INT(height)));
	
    /* Close the window */
    game_window->close();
    game_window = nullptr;
	
    /* Restart Graphics */
	rb_Graphics_start(self);
	
    /* Reset viewport render */
	if (CViewport_Element::render)
	{
		CViewport_Element::render->create(NUM2INT(width), NUM2INT(height));
		CViewport_Element::render->setSmooth(SmoothScreen);
	}
	return self;
}

void global_Graphics_Bind(CDrawable_Element* element)
{
    element->setOriginStack(&Graphics_stack);
}