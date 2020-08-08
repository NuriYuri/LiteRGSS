#include <cassert>
#include "ruby_common.h"
#include "common.h"

#include "Input.h"

#include "CGraphics.h"

CGraphics::CGraphics() : 
	m_snapshot(), 
	m_draw(m_snapshot) {		 
}

CGraphics::~CGraphics() {
	if(game_window != nullptr) { stop(); }
}

std::unique_ptr<sf::Shader> CGraphics::createUniqueShader() const {
	return m_shaderFactory.createUnique();
}

sf::Shader* CGraphics::createNewShader() const {
	return m_shaderFactory.createNew();
}

bool CGraphics::areShadersEnabled() const {
	return m_shaderFactory.areEnabled();
}

void CGraphics::init() {
	m_shaderFactory.warnIfNotAvailable();

	/* Window Loading */
	auto config = m_configLoader.load();
	frame_count = 0;
	
	sf::Uint32 style = sf::Style::Close | sf::Style::Titlebar; // sf::Style::Resize = text issues !
	if(config.fullscreen) {
		style = sf::Style::Fullscreen;
	}
	
	/*sf::ContextSettings settings = sf::ContextSettings(0, 0, 0, 4, 5);*/

	game_window = std::make_unique<sf::RenderWindow>(config.video.vmode, std::move(config.title), style/*, settings*/);
	game_window->setMouseCursorVisible(false);

	m_draw.init(*game_window, config);
	m_snapshot.init();

	/* Input adjustement */
	L_Input_Reset_Clocks();
	L_Input_Setusec_threshold(1000000 / config.frameRate);

	/* Getting oGl Info */
	sf::ContextSettings real = game_window->getSettings();
	m_oGlMajor = real.majorVersion;
	m_oGlMinor = real.minorVersion;
}

void CGraphics::updateSelf(VALUE self) {
	m_draw.updateSelf(self);
}

void CGraphics::manageErrorMessage(VALUE self, const GraphicsUpdateMessage& message) {
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
				if(rb_proc_call(closeHandle, rb_ary_new()) == Qfalse) {
					InsideGraphicsUpdate = false;
					return; /* If the proc returns false we doesn't show the exception */
				}
		}
		stop();
	}
	/* We raise the message */
	InsideGraphicsUpdate = false;
	rb_raise(message.errorObject, "%s", message.message.c_str());
}

void CGraphics::updateProcessEvent(GraphicsUpdateMessage& message) {
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

void CGraphics::stop() {
	protect();
	m_draw.stop();
	m_snapshot.stop();

	/* Close the window */
	game_window->close();
	game_window = nullptr;
}

void CGraphics::update(VALUE self, bool input) {
	protect();
	// Prevent a Thread from calling Graphics.update during Graphics.update process
	if(InsideGraphicsUpdate) { 
		return;
	}
	InsideGraphicsUpdate = true;

	/* Graphics.update real process */
	auto message = m_draw.update();
	
	/* Message Processing */
	GraphicsUpdateMessage localMessage{};
	if(input) {
		updateProcessEvent(message == nullptr ? localMessage : *message);
	}
	localMessage = message == nullptr ? localMessage : *message;
	
	if(!localMessage.message.empty()) {
		manageErrorMessage(self, localMessage);
	}

	/* End of Graphics.update process */
	InsideGraphicsUpdate = false;

	if(input) {
		/* Update the frame count */
		frame_count++;
	}
}

void CGraphics::updateOnlyInput(VALUE self) {
	protect();
	if (InsideGraphicsUpdate) {
		return;
	}
	InsideGraphicsUpdate = true;

	GraphicsUpdateMessage message;
	updateProcessEvent(message);
	if (!message.message.empty()) {
		manageErrorMessage(self, message);
	}

	InsideGraphicsUpdate = false;
}

void CGraphics::protect()  {
	if(game_window == nullptr) {
		constexpr auto rawErrorMessage = "Graphics is not started, window closed thus no Graphics operation allowed. Please call Graphics.start before using other Graphics functions.";
		rb_raise(rb_eStoppedGraphics, rawErrorMessage);
		throw std::runtime_error(rawErrorMessage);
	}
}

void CGraphics::resizeScreen(int width, int height) {
	m_draw.resizeScreen(width, height);
}

void CGraphics::setShader(sf::RenderStates* shader) {
	m_draw.setShader(shader);
}

VALUE CGraphics::takeSnapshot() {
	if(InsideGraphicsUpdate) {
		return Qnil;
	}
	return m_snapshot.takeSnapshot(*game_window);
}

void CGraphics::transition(VALUE self, int argc, VALUE* argv) {
	m_snapshot.transition(self, argc, argv);
}

void CGraphics::freeze(VALUE self) {
	m_snapshot.freeze(*game_window, self);
}

void CGraphics::syncStackCppFromRuby() {
	warnIfGraphicsUpdate();
	m_draw.syncStackCppFromRuby();
}

void CGraphics::add(CDrawable_Element& element) {
	warnIfGraphicsUpdate();
	m_draw.add(element);
}

void CGraphics::warnIfGraphicsUpdate() const {
	if(InsideGraphicsUpdate) {
		rb_warn("WARNING : Accessing Ruby data while updating the screen");
	}
}
