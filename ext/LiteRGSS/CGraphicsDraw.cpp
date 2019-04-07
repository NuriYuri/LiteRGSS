#include <cassert>
#include "utils/ruby_common.h"
#include "utils/common.h"
#include "ruby/thread.h"
#include "SpriteDisposer.h"
#include "CBitmap_Element.h"
#include "CViewport_Element.h"

#include "CGraphicsSnapshot.h"
#include "CGraphicsDraw.h"
#include "CGraphics.h"

CGraphicsDraw::CGraphicsDraw(CGraphicsSnapshot& snapshot) : m_snapshot(snapshot) {}

void CGraphicsDraw::init(VALUE self, sf::RenderWindow& window, const CGraphicsConfig& config) {      
    Graphics_stack = std::make_unique<CGraphicsStack_Element>(self);

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
    
	/* Render resize */
	if (Graphics_Render != nullptr) {
		Graphics_Render->create(ScreenWidth, ScreenHeight);
    }
}

void CGraphicsDraw::resizeScreen(VALUE self, VALUE width, VALUE height) {
    CGraphics::Get().protect();
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
	if (Graphics_Render) {
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
    m_snapshot.draw(*game_window);
}

bool CGraphicsDraw::isGameWindowOpen() const {
    return game_window != nullptr && game_window->isOpen();
}

void* GraphicsDraw_Update_Internal(void* dataPtr) {
    auto& self = *reinterpret_cast<CGraphicsDraw*>(dataPtr);
    if(self.isGameWindowOpen()) {       
        self.updateInternal();
        return nullptr;
    }

    auto message = std::make_unique<GraphicsUpdateMessage>();
    message->errorObject = rb_eStoppedGraphics;
    message->message = "Game Window was closed during Graphics.update by a unknow cause...";
    return message.release();
}

std::unique_ptr<GraphicsUpdateMessage> CGraphicsDraw::update() {
    const auto result = rb_thread_call_without_gvl(GraphicsDraw_Update_Internal, static_cast<void*>(this), NULL, NULL);
    return std::unique_ptr<GraphicsUpdateMessage>(reinterpret_cast<GraphicsUpdateMessage*>(result));
}

void CGraphicsDraw::updateInternal() {
    game_window->clear();
   
    sf::View defview = game_window->getDefaultView();
	auto& render_target = updateDrawPreProc(defview);

	// Rendering stuff
    Graphics_stack->draw(defview, render_target);
	
    // Perform the post proc
	updateDrawPostProc();

	// Update the brightness (applied to game_window)
	if (Graphics_Brightness != 255) {
		drawBrightness();
    }

    game_window->display();
}

void CGraphicsDraw::initRender() {
    CGraphics::Get().protect();
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

void CGraphicsDraw::clearRubyStack() {
    DisposeAllSprites(rb_ivar_get(rb_mGraphics, rb_iElementTable));

    /* Disposing each Bitmap */
    auto objectSpace = rb_const_get(rb_cObject, rb_intern("ObjectSpace"));

    rb_block_call(objectSpace, rb_intern("each_object"), 1, &rb_cBitmap, (rb_block_call_func_t)local_Graphics_Dispose_Bitmap, Qnil);

    int state;
    rb_protect(local_Graphics_call_gc_start, Qnil, &state);
}

void CGraphicsDraw::reloadStack() {
    VALUE table = rb_ivar_get(rb_mGraphics, rb_iElementTable);
    rb_check_type(table, T_ARRAY);
    Graphics_stack->detachSprites();  
    long sz = RARRAY_LEN(table);
    VALUE* ori = RARRAY_PTR(table);
    for(long i = 0; i < sz; i++) {
        if(rb_obj_is_kind_of(ori[i], rb_cViewport) == Qtrue ||
            rb_obj_is_kind_of(ori[i], rb_cSprite) == Qtrue ||
            rb_obj_is_kind_of(ori[i], rb_cText) == Qtrue ||
			rb_obj_is_kind_of(ori[i], rb_cWindow) == Qtrue) {
            if(RDATA(ori[i])->data != nullptr) {
                Graphics_stack->bind(*reinterpret_cast<CDrawable_Element*>(RDATA(ori[i])->data));
            }
        }
    }
}

void CGraphicsDraw::bind(CDrawable_Element& element) {
    Graphics_stack->bind(element);
}

void CGraphicsDraw::stop() {
    Graphics_stack = nullptr;

    game_window = nullptr;

    clearRubyStack();
}

CGraphicsDraw::~CGraphicsDraw() {
    if(Graphics_stack != nullptr) {
        stop();
    }
}