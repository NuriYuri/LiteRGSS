#include "LiteRGSS.h"
#include "CViewport_Element.h"
#include "Graphics.local.h"

unsigned long Graphics_Scale = 1;
bool SmoothScreen = false;

void* local_Graphics_Update_Internal(void* data)
{
    GraphicUpdateMessage* message = nullptr;
    if(game_window->isOpen())
    {
        /* Draw Processing */
        game_window->clear();
        local_Graphics_Update_Draw((std::vector<CDrawable_Element*>*)data);
        game_window->display();
    }
    else
    {
        message = new GraphicUpdateMessage();
        message->errorObject = rb_eStoppedGraphics;
        message->message = "Game Window was closed during Graphics.update by a unknow cause...";
    }
    return message;
}

void local_Graphics_Update_Process_Event(GraphicUpdateMessage*& message)
{
    if(message != nullptr) // We'll try not to cause other error during this
        return;
    sf::Event event;
    while(game_window->pollEvent(event))
    {
        switch(event.type)
        {
            case sf::Event::EventType::Closed:
                message = new GraphicUpdateMessage();
                message->errorObject = rb_eClosedWindow;
                message->message = "Game Window has been closed by user.";
                return;
            case sf::Event::EventType::KeyPressed:
                L_Input_Update_Key(event.key.code, true);
                break;
            case sf::Event::EventType::KeyReleased:
                L_Input_Update_Key(event.key.code, false);
                break;
            case sf::Event::EventType::JoystickButtonPressed:
                L_Input_Update_Joy(event.joystickButton.joystickId, event.joystickButton.button, true);
                break;
            case sf::Event::EventType::JoystickButtonReleased:
                L_Input_Update_Joy(event.joystickButton.joystickId, event.joystickButton.button, false);
                break;
            case sf::Event::EventType::JoystickMoved:
                L_Input_Update_JoyPos(event.joystickMove.joystickId,
                    event.joystickMove.axis,
                    event.joystickMove.position);
                break;
            case sf::Event::EventType::MouseMoved:
                L_Input_Mouse_Pos_Update(event.mouseMove.x, event.mouseMove.y);
                break;
            case sf::Event::EventType::MouseButtonPressed:
                L_Input_Mouse_Button_Update(event.mouseButton.button, true);
                L_Input_Mouse_Pos_Update(event.mouseButton.x, event.mouseButton.y);
                break;
            case sf::Event::EventType::MouseButtonReleased:
                L_Input_Mouse_Button_Update(event.mouseButton.button, false);
                L_Input_Mouse_Pos_Update(event.mouseButton.x, event.mouseButton.y);
                break;
            case sf::Event::EventType::MouseWheelScrolled:
                if(event.mouseWheelScroll.wheel == sf::Mouse::Wheel::VerticalWheel)
                    L_Input_Mouse_Wheel_Update(static_cast<long>(event.mouseWheelScroll.delta));
                L_Input_Mouse_Pos_Update(event.mouseWheelScroll.x, event.mouseWheelScroll.y);
                break;
            case sf::Event::EventType::MouseLeft:
                L_Input_Mouse_Pos_Update(-256, -256);
                break;
        }
    }
}

VALUE local_Graphics_Update_RaiseError(VALUE self, GraphicUpdateMessage* message)
{
    /* If the error is ClosedWindowError, we manage the window closing 
     * When @on_close is defined to a proc, @on_close can decide if the window closing is allowed or not
     * or do things before closing the window
     * That's a kind of rescue process 
     */
    if(message->errorObject == rb_eClosedWindow)
    {
        VALUE closeHandle = rb_iv_get(self, "@on_close");
        if(closeHandle != Qnil)
        {
            VALUE handleClass = rb_class_of(closeHandle);
            if(handleClass == rb_cProc)
                if(rb_proc_call(closeHandle, rb_ary_new()) == Qfalse)
                {
                    InsideGraphicsUpdate = false;
                    return self; /* If the proc returns false we doesn't show the exception */
                }
        }
        local_Graphics_Clear_Stack();
        game_window->close();
        delete game_window;
        game_window = nullptr;
    }
    /* We raise the message */
    InsideGraphicsUpdate = false;
    rb_raise(message->errorObject, "%s", message->message);
    return self;
}

void local_Graphics_Update_Draw(std::vector<CDrawable_Element*>* stack)
{
    bool was_viewport = false;
    CViewport_Element::reset_globalshader();
    sf::View defview = game_window->getDefaultView();
    defview.setSize(ScreenWidth, ScreenHeight);
    defview.setCenter(round(ScreenWidth / 2.0f), round(ScreenHeight / 2.0f));
    game_window->setView(defview);
    for(auto element = stack->begin();element != stack->end(); element++)
    {
        if(was_viewport && !(*element)->isViewport())
            game_window->setView(defview);
        was_viewport = (*element)->isViewport();
        (*element)->draw(*game_window);
    }
    if(Graphics_freeze_sprite != nullptr)
        game_window->draw(*Graphics_freeze_sprite);
}

void local_LoadVideoModeFromConfigs(sf::VideoMode& vmode)
{
    ID swidth = rb_intern("ScreenWidth");
    ID sheight = rb_intern("ScreenHeight");
    ID sscale = rb_intern("ScreenScale");
    long max_width, max_height = 0xFFFFFF;
    long pixdepth = 32;
    long scale = 1;
    std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
    /* If there's a fullscreen mode */
    if(modes.size() > 0)
    {
        max_width = modes[0].width;
        max_height = modes[0].height;
        vmode.bitsPerPixel = modes[0].bitsPerPixel;
    }
    /* Adjust Width */
    if(rb_const_defined(rb_mConfig, swidth))
        vmode.width = normalize_long(rb_num2long(rb_const_get(rb_mConfig, swidth)), 160, max_width);
    /* Adjust Height */
    if(rb_const_defined(rb_mConfig, sheight))
        vmode.height = normalize_long(rb_num2long(rb_const_get(rb_mConfig, sheight)), 144, max_height);
    /* Adjust Scale */
    if(rb_const_defined(rb_mConfig, sscale))
        scale = normalize_long(rb_num2long(rb_const_get(rb_mConfig, sscale)), 1, 10);
    ScreenWidth = vmode.width;
    ScreenHeight = vmode.height;
    vmode.width *= scale;
    vmode.height *= scale;
    Graphics_Scale = scale;
}

const sf::String local_LoadTitleFromConfigs()
{
    ID title = rb_intern("Title");
    if(rb_const_defined(rb_mConfig, title))
    {
        VALUE str_title = rb_const_get(rb_mConfig, title);
        rb_check_type(str_title, T_STRING);
        std::string str(RSTRING_PTR(str_title));
        return sf::String::fromUtf8(str.begin(), str.end());
    }
    return sf::String("LiteRGSS");
}

unsigned int local_LoadFrameRateFromConfigs()
{
    ID framerate = rb_intern("FrameRate");
    if(rb_const_defined(rb_mConfig, framerate))
        return normalize_long(rb_num2long(rb_const_get(rb_mConfig, framerate)), 1, 120);
    return 60;
}

bool local_LoadVSYNCFromConfigs()
{
    ID vsync = rb_intern("Vsync");
    if(rb_const_defined(rb_mConfig, vsync))
        return RTEST(rb_const_get(rb_mConfig, vsync));
    return true;
}

bool local_LoadFullScreenFromConfigs()
{
    ID fsc = rb_intern("FullScreen");
    if(rb_const_defined(rb_mConfig, fsc))
        return RTEST(rb_const_get(rb_mConfig, fsc));
    return false;
}

void local_LoadSmoothScreenFromConfigs()
{
    ID fsc = rb_intern("SmoothScreen");
    if(rb_const_defined(rb_mConfig, fsc))
        SmoothScreen = RTEST(rb_const_get(rb_mConfig, fsc));
    else
        SmoothScreen = false;
}

void local_Graphics_Take_Snapshot(sf::Texture* text)
{
    sf::Vector2u sc_sz = game_window->getSize();
    int x = 0;
    int y = 0;
    int sw = ScreenWidth * Graphics_Scale;
    int sh = ScreenHeight * Graphics_Scale;
    if(sc_sz.x < sw)
    {
        x = sw - sc_sz.x;
    }
    else
        sw = sc_sz.x;
    if(sc_sz.y < sh)
    {
        y = sh - sc_sz.y;
    }
    else
        sh = sc_sz.y;
    text->create(sw, sh);
    text->update(*game_window, x, y);
}

VALUE local_Graphics_Dispose_Bitmap(VALUE block_arg, VALUE data, int argc, VALUE* argv)
{
    if(RDATA(block_arg)->data != nullptr)
        rb_Bitmap_Dispose(block_arg);
}

void local_Graphics_Clear_Stack()
{
    //std::cout << "CLEAN STACK" << std::endl;
    VALUE table = rb_ivar_get(rb_mGraphics, rb_iElementTable);
    long sz = RARRAY_LEN(table);
    VALUE* ori = RARRAY_PTR(table);
    /* Disposing each Sprite/Viewport/Text */
    for(long i = 0; i < sz; i++)
    {
        if(rb_obj_is_kind_of(ori[i], rb_cViewport) == Qtrue)
        {
            if(RDATA(ori[i])->data != nullptr)
                rb_Viewport_Dispose(ori[i]);
        }
        else if(rb_obj_is_kind_of(ori[i], rb_cSprite) == Qtrue)
            rb_Sprite_DisposeFromViewport(ori[i]);
        else if(rb_obj_is_kind_of(ori[i], rb_cText) == Qtrue)
            rb_Text_DisposeFromViewport(ori[i]);
    }
    rb_ary_clear(table);
    /* Disposing each Bitmap */
    rb_block_call(rb_const_get(rb_cObject, rb_intern("ObjectSpace")), rb_intern("each_object"), 1, &rb_cBitmap,
    (rb_block_call_func_t)local_Graphics_Dispose_Bitmap, Qnil);
    rb_gc_start();
}