#include <iostream>

#include <cassert>
#include "utils/ruby_common.h"
#include "ruby/thread.h"
#include "utils/common.h"

#include "SpriteDisposer.h"
#include "Input.h"

#include "CBitmap_Element.h"
#include "CViewport_Element.h"
#include "CGraphics.h"

CGraphics::CGraphics() {}

void CGraphics::loadVideoModeFromConfigs(sf::VideoMode& vmode) {
    ID swidth = rb_intern("ScreenWidth");
    ID sheight = rb_intern("ScreenHeight");
    ID sscale = rb_intern("ScreenScale");
    long max_width, max_height = 0xFFFFFF;
    double scale = 1;
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
        scale = normalize_double(NUM2DBL(rb_const_get(rb_mConfig, sscale)), 0.1, 10);
    setScreenWidth(vmode.width);
    setScreenHeight(vmode.height);
    vmode.width *= scale;
    vmode.height *= scale;
    Graphics_Scale = scale;
}

void CGraphics::loadSmoothScreenFromConfigs() {
    ID fsc = rb_intern("SmoothScreen");
    setSmoothScreen(rb_const_defined(rb_mConfig, fsc) && RTEST(rb_const_get(rb_mConfig, fsc)));
}

const sf::String CGraphics::loadTitleFromConfigs() {
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

VALUE CGraphics::init(VALUE self) {
    setSelf(self);

    if(game_window != nullptr) {
        return Qnil;
    }
    /* Shader Testing */
    if (!sf::Shader::isAvailable()) {
        rb_raise(rb_eRGSSError, "Shaders are not available :(");
    }
    /* Window Loading */
    sf::VideoMode vmode(640, 480, 32);
    loadVideoModeFromConfigs(vmode);
    loadSmoothScreenFromConfigs();
    auto title = loadTitleFromConfigs();
    
    sf::Uint32 style = sf::Style::Close | sf::Style::Titlebar; // sf::Style::Resize = text issues !
    if(loadFullScreenFromConfigs())
        style = sf::Style::Fullscreen;
    game_window = std::make_unique<sf::RenderWindow>(vmode, std::move(title), style);
    game_window->setMouseCursorVisible(false);
    
	frame_rate = loadFrameRateFromConfigs();
    frame_count = 0;

	/* VSYNC choice */
	if(loadVSYNCFromConfigs())
		game_window->setVerticalSyncEnabled(true);
	else
	{
		game_window->setVerticalSyncEnabled(false);
		game_window->setFramerateLimit(frame_rate);
	}

	/* Input adjustement */
    L_Input_Reset_Clocks();
    L_Input_Setusec_threshold(1000000 / frame_rate);
    
	/* Shader loading */
	loadShader();

	/* Render resize */
	if (Graphics_Render != nullptr)
		Graphics_Render->create(screenWidth(), screenHeight());
    return self;
}

void CGraphics::drawBrightness()
{
	sf::Vertex	vertices[4];
	sf::Vector2u size = game_window->getSize();
	vertices[0].position = sf::Vector2f(0, 0);
	vertices[1].position = sf::Vector2f(0, size.y);
	vertices[2].position = sf::Vector2f(size.x, 0);
	vertices[3].position = sf::Vector2f(size.x, size.y);
	vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = sf::Color(0, 0, 0, 255 - Graphics_Brightness);
	game_window->draw(vertices, 4, sf::PrimitiveType::TriangleStrip);
}

unsigned int CGraphics::loadFrameRateFromConfigs()
{
    ID framerate = rb_intern("FrameRate");
    if(rb_const_defined(rb_mConfig, framerate))
        return normalize_long(rb_num2long(rb_const_get(rb_mConfig, framerate)), 1, 120);
    return 60;
}

bool CGraphics::loadVSYNCFromConfigs()
{
    ID vsync = rb_intern("Vsync");
    if(rb_const_defined(rb_mConfig, vsync))
        return RTEST(rb_const_get(rb_mConfig, vsync));
    return true;
}

bool CGraphics::loadFullScreenFromConfigs()
{
    ID fsc = rb_intern("FullScreen");
    if(rb_const_defined(rb_mConfig, fsc))
        return RTEST(rb_const_get(rb_mConfig, fsc));
    return false;
}

void CGraphics::transitionBasic(VALUE self, long time)
{
	RGSSTransition = false;
	sf::Color freeze_color(255, 255, 225, 255);
	for (long i = 1; i <= time; i++)
	{
		freeze_color.a = 255 * (time - i) / time;
		Graphics_freeze_sprite->setColor(freeze_color);
		update(self);
	}
}

void CGraphics::transitionRGSS(VALUE self, long time, VALUE bitmap)
{
	Graphics_freeze_sprite->setColor(sf::Color(255, 255, 255, 255));
	RGSSTransition = true;
    if(Graphics_freeze_shader != nullptr) {
        auto& bitmap_ = rb::GetSafe<CBitmap_Element>(bitmap, rb_cBitmap);
        Graphics_freeze_shader->setUniform("transition", bitmap_.getTexture());
        for (long i = 1; i <= time; i++) {
            Graphics_freeze_shader->setUniform("param", static_cast<float>(i) / time);
            update(self);
        }
    }
}

void* Graphics_Update_Internal(void* data) {
    auto& self = *reinterpret_cast<CGraphics*>(data);
    
    if(self.isGameWindowOpen()) {       
        self.updateDraw();
        return nullptr;
    }

    auto message = std::make_unique<GraphicUpdateMessage>();
    message->errorObject = rb_eStoppedGraphics;
    message->message = "Game Window was closed during Graphics.update by a unknow cause...";
    return message.release();
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

//
// This function retreive the right render to perform the draw operations
// It also resets the default view and transmit it to the right render
//
sf::RenderTarget* CGraphics::updateDrawPreProc(sf::View& defview)
{
	// Getting the basic render target
	sf::RenderTarget* render_target = game_window.get();
	// Setting the default view parameters
    auto screenWidth = CGraphics::Get().screenWidth();
    auto screenHeight = CGraphics::Get().screenHeight();
	defview.setSize(screenWidth, screenHeight);
	defview.setCenter(round(screenWidth / 2.0f), round(screenHeight / 2.0f));
	// Appying the default view to the Window (used in updateDrawPostProc)
	game_window->setView(defview);
	// If the Graphics_Render is defined, we use it instead of the game_window (shader processing)
	if (Graphics_Render)
	{
		// Set the default view
		Graphics_Render->setView(defview);
		// It's not cleard so we perform the clear operation
		Graphics_Render->clear();
		render_target = Graphics_Render.get();
	}
	return render_target;
}

//
// This function perform the PostProc operation of the graphic UpdateDraw :
// - Draw the render to the Window if it's defined
// - Draw the transition sprite
//
void CGraphics::updateDrawPostProc() {
	// Drawing render to window if finished
	if (Graphics_Render)
	{
		Graphics_Render->display();
		sf::Sprite sp(Graphics_Render->getTexture());
		if (Graphics_States == nullptr)
			game_window->draw(sp);
		else
			game_window->draw(sp, *Graphics_States);
	}
	// Display transition sprite
	if (Graphics_freeze_sprite != nullptr)
	{
		if (RGSSTransition)
			game_window->draw(*Graphics_freeze_sprite, Graphics_freeze_shader.get());
		else
			game_window->draw(*Graphics_freeze_sprite);
	}
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

void CGraphics::updateDraw() {
    game_window->clear();
   
    sf::View defview = game_window->getDefaultView();
	sf::RenderTarget* render_target = updateDrawPreProc(defview);

	// Rendering stuff
    Graphics_stack->draw(defview, *render_target);
	
    // Perform the post proc
	updateDrawPostProc();
	// Update the brightness (applied to game_window)
	if (Graphics_Brightness != 255) {
		drawBrightness();
    }

    game_window->display();
}



const std::string GraphicsTransitionFragmentShader = \
	"uniform float param;" \
	"uniform sampler2D texture;" \
	"uniform sampler2D transition;" \
	"const float sensibilite = 0.05;" \
	"const float scale = 1.0 + sensibilite;" \
	"void main()" \
	"{" \
	"  vec4 frag = texture2D(texture, gl_TexCoord[0].xy);" \
	"  vec4 tran = texture2D(transition, gl_TexCoord[0].xy);" \
	"  float pixel = max(max(tran.r, tran.g), tran.b);" \
	"  pixel -= (param * scale);" \
	"  if(pixel < sensibilite)" \
	"  {" \
	"    frag.a = max(0.0, sensibilite + pixel / sensibilite);" \
	"  }" \
	"  gl_FragColor = frag;" \
	"}";

void CGraphics::loadShader() {
    Graphics_freeze_shader = std::make_unique<sf::Shader>();
	Graphics_freeze_shader->loadFromMemory(GraphicsTransitionFragmentShader, sf::Shader::Type::Fragment);
}

VALUE CGraphics::takeSnapshot() {
    protect();
    if(InsideGraphicsUpdate) {
        return Qnil;
    }

    //Allocates memory ruby-side to take a snapshot
    VALUE bmp = rb_obj_alloc(rb_cBitmap);
    CBitmap_Element* bitmap;
    Data_Get_Struct(bmp, CBitmap_Element, bitmap);
    if(bitmap == nullptr) {
        return Qnil;
    }

    //Then uses it in C++ code
    sf::Texture& text = bitmap->getTexture();
    takeSnapshotOn(text);
    return bmp;
}

void CGraphics::initRender()
{
    protect();
    auto screenWidth = CGraphics::Get().screenWidth();
    auto screenHeight = CGraphics::Get().screenHeight();
	if (Graphics_Render == nullptr && Graphics_States != nullptr) {
		Graphics_Render = std::make_unique<sf::RenderTexture>();
		Graphics_Render->create(screenWidth, screenHeight);
	}
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

    std::cout << "CLEARING Graphics Stack" << std::endl;
    DisposeAllSprites(rb_ivar_get(rb_mGraphics, rb_iElementTable));

    /* Disposing each Bitmap */
    auto objectSpace = rb_const_get(rb_cObject, rb_intern("ObjectSpace"));

    rb_block_call(objectSpace, rb_intern("each_object"), 1, &rb_cBitmap, (rb_block_call_func_t)local_Graphics_Dispose_Bitmap, Qnil);

    int state;
    rb_protect(local_Graphics_call_gc_start, Qnil, &state);

    return true;
}

void CGraphics::stop() {
    protect();
    if(!clearStack()) {
        return;
    }
    
    /* Close the window */
    game_window->close();
    game_window = nullptr;

    /* Unfreezing Graphics */    
    Graphics_freeze_sprite = nullptr;    
    Graphics_freeze_texture = nullptr;
    Graphics_freeze_shader = nullptr;
}

VALUE CGraphics::freeze(VALUE self) {
    protect();
    if(Graphics_freeze_texture != nullptr) {
        return Qnil;
    }
    auto result = update(self);
    Graphics_freeze_texture = std::make_unique<sf::Texture>();
    takeSnapshotOn(*Graphics_freeze_texture);
    Graphics_freeze_sprite = std::make_unique<sf::Sprite>(*Graphics_freeze_texture);
    Graphics_freeze_sprite->setScale(1.0f / Graphics_Scale, 1.0f / Graphics_Scale);
    return self;
}

void CGraphics::takeSnapshotOn(sf::Texture& text) const {
    sf::Vector2u sc_sz = game_window->getSize();
    int x = 0;
    int y = 0;
    auto screenWidth = CGraphics::Get().screenWidth();
    auto screenHeight = CGraphics::Get().screenHeight();
    int sw = screenWidth * Graphics_Scale;
    int sh = screenHeight * Graphics_Scale;
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
    text.create(sw, sh);
    text.update(*game_window, x, y);
}

void CGraphics::transition(VALUE self, int argc, VALUE* argv) {
    protect();
    if(Graphics_freeze_sprite == nullptr) {
        return;
    }

	long time = 8; //< RGSS doc
    if(argc >= 1)
		time = rb_num2long(argv[0]);
    time = normalize_long(time, 1, 0xFFFF);
	if (argc < 2 || rb_obj_is_kind_of(argv[1], rb_cBitmap) != Qtrue)
		transitionBasic(self, time);
	else
		transitionRGSS(self, time, argv[1]);
    
    Graphics_freeze_sprite = nullptr;
    Graphics_freeze_texture = nullptr;
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
                bind(*reinterpret_cast<CDrawable_Element*>(RDATA(ori[i])->data));
            }
        }
    }
}

void CGraphics::bind(CDrawable_Element& element) {
    Graphics_stack->bind(element);
}

void CGraphics::resizeScreen(VALUE self, VALUE width, VALUE height) {
    protect();
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
    init(self);
	
    /* Reset viewport render */
	if (CViewport_Element::render)
	{
		CViewport_Element::render->create(NUM2INT(width), NUM2INT(height));
		CViewport_Element::render->setSmooth(CGraphics::Get().smoothScreen());
	}
}

void CGraphics::protect()  {
    if(game_window == nullptr) {
        constexpr auto rawErrorMessage = "Graphics is not started, window closed thus no Graphics operation allowed. Please call Graphics.start before using other Graphics functions.";
        rb_raise(rb_eStoppedGraphics, rawErrorMessage);
        throw std::runtime_error(rawErrorMessage);
    }
}

void CGraphics::setShader(sf::RenderStates* shader) {
    Graphics_States = shader;
}

bool CGraphics::isGameWindowOpen() const {
    return game_window != nullptr && game_window->isOpen();
}

CGraphics::~CGraphics() {
    if(game_window != nullptr) {
        stop();
    }
}
