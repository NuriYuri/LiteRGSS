#include <cassert>
#include "utils/ruby_common.h"
#include "ruby/thread.h"
#include "utils/common.h"

#include "SpriteDisposer.h"
#include "Input.h"

#include "CBitmap_Element.h"
#include "CViewport_Element.h"
#include "CGraphics.h"

VALUE CGraphics::init(VALUE self) {
    Graphics_stack = std::make_unique<CGraphicsStack_Element>(self);

    if(game_window != nullptr) {
        return Qnil;
    }
    /* Shader Testing */
    if (!sf::Shader::isAvailable()) {
        rb_raise(rb_eRGSSError, "Shaders are not available :(");
    }
    
    /* Window Loading */
    auto config = m_configLoader.load();
    frame_count = 0;
    
    sf::Uint32 style = sf::Style::Close | sf::Style::Titlebar; // sf::Style::Resize = text issues !
    if(config.fullscreen) {
        style = sf::Style::Fullscreen;
    }
    game_window = std::make_unique<sf::RenderWindow>(config.video.vmode, std::move(config.title), style);
    game_window->setMouseCursorVisible(false);

    m_draw.init(*game_window, config);

	/* Input adjustement */
    L_Input_Reset_Clocks();
    L_Input_Setusec_threshold(1000000 / config.frameRate);
    
    return self;
}

VALUE CGraphics::updateRaiseError(VALUE self, const GraphicUpdateMessage& message) {
    /* If the error is ClosedWindowError, we manage the window closing 
     * When @on_close is defined to a proc, @on_close can decide if the window closing is allowed or not
     * or do things before closing the window
     * That's a kind of rescue process 
     */
    if(message.errorObject == rb_eClosedWindow)
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
        clearStack();
        if(game_window != nullptr) {
            game_window->close();
            game_window = nullptr;
        }
    }
    /* We raise the message */
    InsideGraphicsUpdate = false;
    rb_raise(message.errorObject, "%s", message.message.c_str());
    return self;
}

void CGraphics::updateProcessEvent(GraphicUpdateMessage& message) {
    sf::Event event;
	L_EnteredText.clear();
    while(game_window->pollEvent(event))
    {
        switch(event.type)
        {
            case sf::Event::EventType::Closed:
                message.errorObject = rb_eClosedWindow;
                message.message = "Game Window has been closed by user.";
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
			case sf::Event::EventType::JoystickConnected:
			case sf::Event::EventType::JoystickDisconnected:
				L_Input_Reset_JoyPos(event.joystickConnect.joystickId);
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
			case sf::Event::EventType::TextEntered:
				L_EnteredText.append((char*)sf::String(event.text.unicode).toUtf8().c_str());
				break;
        }
    }
}

bool CGraphics::isGameWindowOpen() const {
    return game_window != nullptr && game_window->isOpen();
}

extern "C" {
    VALUE local_Graphics_call_gc_start(VALUE ignored) {
        rb_gc_start();
        return Qnil;
    }

    VALUE local_Graphics_Dispose_Bitmap(VALUE block_arg, VALUE data, int argc, VALUE* argv) {
        if(RDATA(block_arg)->data != nullptr) {
            rb::Dispose<CBitmap_Element>(block_arg);
        }
        return Qnil;
    }
}

bool CGraphics::clearStack() {
    // Prevent a Thread from calling a graphic reset
    if(InsideGraphicsUpdate) {
        return false;
    }

    DisposeAllSprites(rb_ivar_get(rb_mGraphics, rb_iElementTable));

    /* Disposing each Bitmap */
    auto objectSpace = rb_const_get(rb_cObject, rb_intern("ObjectSpace"));

    rb_block_call(objectSpace, rb_intern("each_object"), 1, &rb_cBitmap, (rb_block_call_func_t)local_Graphics_Dispose_Bitmap, Qnil);

    int state;
    rb_protect(local_Graphics_call_gc_start, Qnil, &state);

    return true;
}

void CGraphics::reloadStack() {
    VALUE table = rb_ivar_get(rb_mGraphics, rb_iElementTable);
    rb_check_type(table, T_ARRAY);
    Graphics_stack->detachSprites();  
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
                Graphics_stack->bind(*reinterpret_cast<CDrawable_Element*>(RDATA(ori[i])->data));
            }
        }
    }
}

void CGraphics::bind(CDrawable_Element& element) {
    Graphics_stack->bind(element);
}

void CGraphics::stop() {
    protect();
    Graphics_stack = nullptr;
    if(!clearStack()) {
        return;
    }
    
    /* Close the window */
    game_window->close();
    game_window = nullptr;

    m_draw.stop();
}

void* Graphics_Update_Internal(void* data) {
    auto& self = *reinterpret_cast<CGraphics*>(data);
    
    if(self.isGameWindowOpen()) {       
        self.draw();
        return nullptr;
    }

    auto message = std::make_unique<GraphicUpdateMessage>();
    message->errorObject = rb_eStoppedGraphics;
    message->message = "Game Window was closed during Graphics.update by a unknow cause...";
    return message.release();
}

void CGraphics::draw() {
    m_draw.update(*Graphics_stack);
}

VALUE CGraphics::update(VALUE self) {
    protect();
    // Prevent a Thread from calling Graphics.update during Graphics.update process
    if(InsideGraphicsUpdate) { 
        return self;
    }
    auto result = self;

    InsideGraphicsUpdate = true;
    /* Graphics.update real process */
    auto message = 
        std::unique_ptr<GraphicUpdateMessage>(reinterpret_cast<GraphicUpdateMessage*>(rb_thread_call_without_gvl(Graphics_Update_Internal, static_cast<void*>(this), NULL, NULL)));
    
    /* Message Processing */
    GraphicUpdateMessage localMessage{};
    updateProcessEvent(message == nullptr ? localMessage : *message);
    localMessage = message == nullptr ? localMessage : *message;
    if(!localMessage.message.empty()) {
        result = updateRaiseError(self, localMessage);
    }

    /* End of Graphics.update process */
    InsideGraphicsUpdate = false;

    /* Update the frame count */
    frame_count++;

    return result;
}

VALUE CGraphics::updateNoInputCount(VALUE self) {
    protect();
    if (InsideGraphicsUpdate) // Prevent a Thread from calling Graphics.update during Graphics.update process
        return self;

    auto result = self;
	InsideGraphicsUpdate = true;
	/* Graphics.update real process */
	GraphicUpdateMessage* message =
		reinterpret_cast<GraphicUpdateMessage*>(rb_thread_call_without_gvl(Graphics_Update_Internal, static_cast<void*>(this), NULL, NULL));
	
    if (message != nullptr) {
		result = updateRaiseError(self, *message);
    }
    
    /* End of Graphics.update process */
	InsideGraphicsUpdate = false;

    return result;
}

VALUE CGraphics::updateOnlyInput(VALUE self) {
    protect();
    if (InsideGraphicsUpdate)
        return self;
    InsideGraphicsUpdate = true;
	auto result = self;
    GraphicUpdateMessage message;
	updateProcessEvent(message);
	if (!message.message.empty()) {
		result = updateRaiseError(self, message);
    }
	InsideGraphicsUpdate = false;
    return result;
}

void CGraphics::protect()  {
    if(game_window == nullptr) {
        constexpr auto rawErrorMessage = "Graphics is not started, window closed thus no Graphics operation allowed. Please call Graphics.start before using other Graphics functions.";
        rb_raise(rb_eStoppedGraphics, rawErrorMessage);
        throw std::runtime_error(rawErrorMessage);
    }
}

void CGraphics::resizeScreen(VALUE self, VALUE width, VALUE height) {
    m_draw.resizeScreen(self, width, height);
}

void CGraphics::setShader(sf::RenderStates* shader) {
    m_draw.setShader(shader);
}

VALUE CGraphics::takeSnapshot() {
    if(InsideGraphicsUpdate) {
        return Qnil;
    }
    return m_draw.takeSnapshot();
}

void CGraphics::transition(VALUE self, int argc, VALUE* argv) {
    m_draw.transition(self, argc, argv);
}

VALUE CGraphics::freeze(VALUE self) {
    return m_draw.freeze(self);
}

CGraphics::~CGraphics() {
    if(game_window != nullptr) {
        stop();
    }
}
