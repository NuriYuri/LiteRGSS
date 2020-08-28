#include "ruby_common.h"
#include "common.h"
#include "rbAdapter.h"
#include "CGraphicsSnapshot.h"
#include "CBitmap_Element.h"
#include "CGraphics.h"
#include "CShaderFactory.h"

void CGraphicsSnapshot::init() {
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
		"	frag.a = max(0.0, sensibilite + pixel / sensibilite);" \
		"  }" \
		"  gl_FragColor = frag;" \
		"}";

	Graphics_freeze_shader = CGraphics::Get().createUniqueShader();
	if (Graphics_freeze_shader != nullptr) {
		Graphics_freeze_shader->loadFromMemory(GraphicsTransitionFragmentShader, sf::Shader::Type::Fragment);
	}
}

void CGraphicsSnapshot::freeze(sf::RenderWindow& window, VALUE self) {
	CGraphics::Get().protect();
	if(Graphics_freeze_texture != nullptr) {
		return;
	}
	CGraphics::Get().update(self);
	Graphics_freeze_texture = std::make_unique<sf::Texture>();
	takeSnapshotOn(window, *Graphics_freeze_texture);
	Graphics_freeze_sprite = std::make_unique<sf::Sprite>(*Graphics_freeze_texture);
	Graphics_freeze_sprite->setScale(1.0f / CGraphics::Get().scale(), 1.0f / CGraphics::Get().scale());
}

void CGraphicsSnapshot::takeSnapshotOn(sf::RenderWindow& window, sf::Texture& text) const {
	sf::Vector2u sc_sz = window.getSize();
	int x = 0;
	int y = 0;
	auto screenWidth = CGraphics::Get().screenWidth();
	auto screenHeight = CGraphics::Get().screenHeight();
	int sw = screenWidth * CGraphics::Get().scale();
	int sh = screenHeight * CGraphics::Get().scale();
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
	text.update(window, x, y);
	
	sf::RenderTexture renderTexture;
	auto blendMode = sf::BlendMode(
		sf::BlendMode::One,
		sf::BlendMode::Zero,
		sf::BlendMode::Add,
		sf::BlendMode::One,
		sf::BlendMode::One,
		sf::BlendMode::Add
	);
	renderTexture.create(text.getSize().x, text.getSize().y);
	renderTexture.clear(sf::Color::Black); // Needs to be non-transparent to compensate the transparence issue of texture
	renderTexture.draw(sf::Sprite(text), sf::RenderStates(blendMode));
	renderTexture.display();
	auto text2 = renderTexture.getTexture();
	text.swap(text2);
}

void CGraphicsSnapshot::transitionBasic(VALUE self, long time) {
	RGSSTransition = false;
	sf::Color freeze_color(255, 255, 225, 255);
	for (long i = 1; i <= time; i++)
	{
		freeze_color.a = 255 * (time - i) / time;
		Graphics_freeze_sprite->setColor(freeze_color);
		CGraphics::Get().update(self);
	}
}

void CGraphicsSnapshot::transitionRGSS(VALUE self, long time, VALUE bitmap) {
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

void CGraphicsSnapshot::transition(VALUE self, int argc, VALUE* argv) {
	CGraphics::Get().protect();
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

VALUE CGraphicsSnapshot::takeSnapshot(sf::RenderWindow& window) {
	CGraphics::Get().protect();

	//Allocates memory ruby-side to take a snapshot
	VALUE bmp = rb_obj_alloc(rb_cBitmap);
	CBitmap_Element* bitmap;
	Data_Get_Struct(bmp, CBitmap_Element, bitmap);
	if(bitmap == nullptr) {
		return Qnil;
	}

	//Then uses it in C++ code
	sf::Texture& text = bitmap->getTexture();
	takeSnapshotOn(window, text);
	return bmp;
}

void CGraphicsSnapshot::stop() {
	/* Unfreezing Graphics */
	Graphics_freeze_sprite = nullptr;	
	Graphics_freeze_texture = nullptr;
	Graphics_freeze_shader = nullptr;	
}

void CGraphicsSnapshot::draw(sf::RenderWindow& window) {
	//NO RUBY API ACCESS MUST BE DONE HERE
	// Display transition sprite
	if (Graphics_freeze_sprite != nullptr)
	{
		if (RGSSTransition)
			window.draw(*Graphics_freeze_sprite, Graphics_freeze_shader.get());
		else
			window.draw(*Graphics_freeze_sprite);
	}
}
