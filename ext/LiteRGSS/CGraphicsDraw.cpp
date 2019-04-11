#include <cassert>
#include "CGraphicsUpdateMessage.h"
#include "utils/ruby_common.h"
#include "utils/common.h"
#include "ruby/thread.h"
#include "SpriteDisposer.h"
#include "CBitmap_Element.h"
#include "CViewport_Element.h"

#include "CGraphicsSnapshot.h"
#include "CGraphicsDraw.h"
#include "CGraphics.h"

CGraphicsDraw::CGraphicsDraw(CGraphicsSnapshot& snapshot) : 
	m_snapshot(snapshot) {
}

void CGraphicsDraw::init(sf::RenderWindow& window, const CGraphicsConfig& config, std::unique_ptr<CDrawableStack> stack) {
	m_stack = std::move(stack);
    m_gameWindow = &window;
    m_gameWindow->setMouseCursorVisible(false);

	/* VSYNC choice */
    m_gameWindow->setVerticalSyncEnabled(config.vSync);
	if(!config.vSync) {
		m_gameWindow->setFramerateLimit(config.frameRate);
	}
    
    m_screenWidth = config.video.width;
    m_screenHeight = config.video.height;
    m_smoothScreen = config.smoothScreen;
    m_scale = config.video.scale;
    m_frameRate = config.frameRate;
    
	/* Render resize */
	if (m_renderTexture != nullptr) {
		m_renderTexture->create(m_screenWidth, m_screenHeight);
    }
}

void CGraphicsDraw::resizeScreen(int width, int height) {
    CGraphics::Get().protect();

    /* Close the window */
    m_gameWindow->close();
    m_gameWindow = nullptr;
	
    /* Restart Graphics */
    CGraphics::Get().init(std::move(m_stack));
	
    /* Reset viewport render */
	if (CViewport_Element::render)
	{
		CViewport_Element::render->create(width, height);
		CViewport_Element::render->setSmooth(m_smoothScreen);
	}
}

void CGraphicsDraw::drawBrightness()
{
	//NO RUBY API ACCESS MUST BE DONE HERE
	sf::Vertex	vertices[4];
	sf::Vector2u size = m_gameWindow->getSize();
	vertices[0].position = sf::Vector2f(0, 0);
	vertices[1].position = sf::Vector2f(0, size.y);
	vertices[2].position = sf::Vector2f(size.x, 0);
	vertices[3].position = sf::Vector2f(size.x, size.y);
	vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = sf::Color(0, 0, 0, 255 - m_brightness);
	m_gameWindow->draw(vertices, 4, sf::PrimitiveType::TriangleStrip);
}

//
// This function retreive the right render to perform the draw operations
// It also resets the default view and transmit it to the right render
//
sf::RenderTarget& CGraphicsDraw::configureAndGetRenderTarget(sf::View& defview)
{
	//NO RUBY API ACCESS MUST BE DONE HERE
	// Setting the default view parameters
	defview.setSize(m_screenWidth, m_screenHeight);
	defview.setCenter(round(m_screenWidth / 2.0f), round(m_screenHeight / 2.0f));
	// Appying the default view to the Window (used in postProcessing())
	m_gameWindow->setView(defview);
	// If the m_renderTexture is defined, we use it instead of the m_gameWindow (shader processing)
	if (m_renderTexture) {
		// Set the default view
		m_renderTexture->setView(defview);
		// It's not cleard so we perform the clear operation
		m_renderTexture->clear();
		return *m_renderTexture.get();
	}
	return *m_gameWindow;
}

//
// This function perform the PostProc operation of the graphic UpdateDraw :
// - Draw the render to the Window if it's defined
// - Draw the transition sprite
//
void CGraphicsDraw::postProcessing() {
	//NO RUBY API ACCESS MUST BE DONE HERE
	// Drawing render to window if finished
	if (m_renderTexture)
	{
		m_renderTexture->display();
		sf::Sprite sp(m_renderTexture->getTexture());
		if (m_renderState == nullptr)
			m_gameWindow->draw(sp);
		else
			m_gameWindow->draw(sp, *m_renderState);
	}
	//Draw the "freeze" texture, if visible
    m_snapshot.draw(*m_gameWindow);

	// Update the brightness (applied to m_gameWindow)
	if (m_brightness != 255) {
		drawBrightness();
    }
}

bool CGraphicsDraw::isGameWindowOpen() const {
	//NO RUBY API ACCESS MUST BE DONE HERE
    return m_gameWindow != nullptr && m_gameWindow->isOpen();
}

void* GraphicsDraw_Update_Internal(void* dataPtr) {
    //NO RUBY API ACCESS MUST BE DONE HERE
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
	//NO RUBY API ACCESS MUST BE DONE HERE
    m_gameWindow->clear();
   
    sf::View defview = m_gameWindow->getDefaultView();
	auto& render_target = configureAndGetRenderTarget(defview);
	// Rendering C++ sprite stack
    m_stack->draw(defview, render_target);

	postProcessing();

    m_gameWindow->display();
}

void CGraphicsDraw::initRender() {
    CGraphics::Get().protect();
	if (m_renderTexture == nullptr && m_renderState != nullptr) {
		m_renderTexture = std::make_unique<sf::RenderTexture>();
		m_renderTexture->create(m_screenWidth, m_screenHeight);
	}
}

void CGraphicsDraw::setShader(sf::RenderStates* shader) {
    m_renderState = shader;
    if(shader != nullptr) {
        initRender();
    }
}

void CGraphicsDraw::syncStackCppFromRuby() {
    m_stack->syncStackCppFromRuby();
}

void CGraphicsDraw::add(CDrawable_Element& element) {
    m_stack->add(element);
}

void CGraphicsDraw::stop() {
    m_gameWindow = nullptr;

	m_stack->clear();
    m_globalBitmaps.clear();  
}

CGraphicsDraw::~CGraphicsDraw() {
    stop();
}
