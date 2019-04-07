#include <cassert>
#include "utils/ruby_common.h"
#include "utils/common.h"

#include "CBitmap_Element.h"
#include "CViewport_Element.h"
#include "CGraphicsDraw.h"
#include "CGraphics.h"

void CGraphicsDraw::protect()  {
    if(game_window == nullptr) {
        constexpr auto rawErrorMessage = "Graphics is not started, window closed thus no Graphics operation allowed. Please call Graphics.start before using other Graphics functions.";
        rb_raise(rb_eStoppedGraphics, rawErrorMessage);
        throw std::runtime_error(rawErrorMessage);
    }
}

void CGraphicsDraw::init(sf::RenderWindow& window, const CGraphicsConfig& config) {      
    game_window = &window;
    game_window->setMouseCursorVisible(false);

	/* VSYNC choice */
    game_window->setVerticalSyncEnabled(config.vSync);
	if(!config.vSync) {
		game_window->setFramerateLimit(config.frameRate);
	}
    
    ScreenWidth = config.video.width;
    ScreenHeight = config.video.height;
    SmoothScreen = config.smoothScreen;
    Graphics_Scale = config.video.scale;
    frame_rate = config.frameRate;
    
	/* Shader loading */
	loadShader();

	/* Render resize */
	if (Graphics_Render != nullptr) {
		Graphics_Render->create(ScreenWidth, ScreenHeight);
    }
}

void CGraphicsDraw::loadShader() {
    static constexpr const char* GraphicsTransitionFragmentShader = \
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

    Graphics_freeze_shader = std::make_unique<sf::Shader>();
	Graphics_freeze_shader->loadFromMemory(GraphicsTransitionFragmentShader, sf::Shader::Type::Fragment);
}

void CGraphicsDraw::resizeScreen(VALUE self, VALUE width, VALUE height) {
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
    CGraphics::Get().init(self);
	
    /* Reset viewport render */
	if (CViewport_Element::render)
	{
		CViewport_Element::render->create(NUM2INT(width), NUM2INT(height));
		CViewport_Element::render->setSmooth(SmoothScreen);
	}
}

void CGraphicsDraw::drawBrightness()
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

//
// This function retreive the right render to perform the draw operations
// It also resets the default view and transmit it to the right render
//
sf::RenderTarget& CGraphicsDraw::updateDrawPreProc(sf::View& defview)
{
	// Setting the default view parameters
	defview.setSize(ScreenWidth, ScreenHeight);
	defview.setCenter(round(ScreenWidth / 2.0f), round(ScreenHeight / 2.0f));
	// Appying the default view to the Window (used in updateDrawPostProc)
	game_window->setView(defview);
	// If the Graphics_Render is defined, we use it instead of the game_window (shader processing)
	if (Graphics_Render)
	{
		// Set the default view
		Graphics_Render->setView(defview);
		// It's not cleard so we perform the clear operation
		Graphics_Render->clear();
		return *Graphics_Render.get();
	}
	return *game_window;
}

//
// This function perform the PostProc operation of the graphic UpdateDraw :
// - Draw the render to the Window if it's defined
// - Draw the transition sprite
//
void CGraphicsDraw::updateDrawPostProc() {
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

void CGraphicsDraw::update(CGraphicsStack_Element& stack) {
    game_window->clear();
   
    sf::View defview = game_window->getDefaultView();
	auto& render_target = updateDrawPreProc(defview);

	// Rendering stuff
    stack.draw(defview, render_target);
	
    // Perform the post proc
	updateDrawPostProc();

	// Update the brightness (applied to game_window)
	if (Graphics_Brightness != 255) {
		drawBrightness();
    }

    game_window->display();
}

void CGraphicsDraw::initRender() {
    protect();
	if (Graphics_Render == nullptr && Graphics_States != nullptr) {
		Graphics_Render = std::make_unique<sf::RenderTexture>();
		Graphics_Render->create(ScreenWidth, ScreenHeight);
	}
}

void CGraphicsDraw::setShader(sf::RenderStates* shader) {
    Graphics_States = shader;
    if(shader != nullptr) {
        initRender();
    }
}

VALUE CGraphicsDraw::freeze(VALUE self) {
    protect();
    if(Graphics_freeze_texture != nullptr) {
        return Qnil;
    }
    auto result = CGraphics::Get().update(self);
    Graphics_freeze_texture = std::make_unique<sf::Texture>();
    takeSnapshotOn(*Graphics_freeze_texture);
    Graphics_freeze_sprite = std::make_unique<sf::Sprite>(*Graphics_freeze_texture);
    Graphics_freeze_sprite->setScale(1.0f / Graphics_Scale, 1.0f / Graphics_Scale);
    return self;
}

void CGraphicsDraw::takeSnapshotOn(sf::Texture& text) const {
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

void CGraphicsDraw::transitionBasic(VALUE self, long time)
{
	RGSSTransition = false;
	sf::Color freeze_color(255, 255, 225, 255);
	for (long i = 1; i <= time; i++)
	{
		freeze_color.a = 255 * (time - i) / time;
		Graphics_freeze_sprite->setColor(freeze_color);
		CGraphics::Get().update(self);
	}
}

void CGraphicsDraw::transitionRGSS(VALUE self, long time, VALUE bitmap)
{
	Graphics_freeze_sprite->setColor(sf::Color(255, 255, 255, 255));
	RGSSTransition = true;
    if(Graphics_freeze_shader != nullptr) {
        auto& bitmap_ = rb::GetSafe<CBitmap_Element>(bitmap, rb_cBitmap);
        Graphics_freeze_shader->setUniform("transition", bitmap_.getTexture());
        for (long i = 1; i <= time; i++) {
            Graphics_freeze_shader->setUniform("param", static_cast<float>(i) / time);
            CGraphics::Get().update(self);
        }
    }
}

void CGraphicsDraw::transition(VALUE self, int argc, VALUE* argv) {
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

VALUE CGraphicsDraw::takeSnapshot() {
    protect();

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

void CGraphicsDraw::stop() {
    /* Unfreezing Graphics */
    Graphics_freeze_sprite = nullptr;    
    Graphics_freeze_texture = nullptr;
    Graphics_freeze_shader = nullptr;
}
