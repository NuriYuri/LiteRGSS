#include <cassert>
#include "LiteRGSS.h"
#include "CViewport_Element.h"
#include "CGraphics.h"
#include "CRect_Element.h"
#include "CTone_Element.h"
#include "CWindow_Element.h"
#include "CShaderFactory.h"

std::unique_ptr<sf::RenderTexture> CViewport_Element::render = nullptr;
std::unique_ptr<sf::Sprite> CViewport_Element::render_sprite = nullptr;

CViewport_Element::~CViewport_Element() 
{
	if(!CGraphics::Get().isGameWindowOpen()) {
		std::cerr << "Game window release thus viewport " << this << " not freed." << std::endl;
	}

	bindTone(nullptr);
	rViewport = Qnil;
}

void CViewport_Element::draw(sf::RenderTarget& target) const
{
	auto* render_states = getRenderState();
	if (!visible)
		return;
	if(render_states)
	{
		const sf::Color* col;
		CRect_Element* rect = getRect();
		// Loading Target view
		const sf::Vector2f view_size = view.getSize();
		sf::View tview = view;
		render->setView(tview);
		// Clearing render
		if (render_states->shader)
			col = check_up_color();
		else
			col = &sf::Color::Transparent;
		render->clear(*col);
		// Drawing sprites
		CView_Element::drawFast(*render);
		render->display();
		// Update internal texture & draw result to Window
		render_sprite->setTexture(render->getTexture());
		if (rect)
		{
			render_sprite->setTextureRect(rect->getRect());
			render_sprite->setPosition(rect->getRect().left, rect->getRect().top);
		}
		// Reset the target view
		setupView(target);
		// Draw the result
		target.draw(*render_sprite, *render_states);
	}
	else
	{
		target.setView(view);
		CView_Element::drawFast(target);
	}
}

void CViewport_Element::setupView(sf::RenderTarget& target) const 
{
	long height = CGraphics::Get().screenHeight();
	long width = CGraphics::Get().screenWidth();
	sf::View defview = target.getDefaultView();
	defview.setSize(width, height);
	defview.setCenter(round(width / 2.0f), round(height / 2.0f));
	target.setView(defview);
}

bool CViewport_Element::isViewport() const
{
	return true;
}

bool CViewport_Element::isPureSprite() const
{
	return false;
}

bool CViewport_Element::isShape() const
{
	return false;
}

void CViewport_Element::setRenderStates(sf::RenderStates* states)
{
	render_states = states;
	if (states == nullptr)
	{
		render_states_shader = nullptr;
		default_render_states = nullptr;
		default_render_states_shader = nullptr;
		color_copy = nullptr;
	}
	else
	{
		render_states_shader = const_cast<sf::Shader*>(states->shader);
	}
}

sf::RenderStates* CViewport_Element::getRenderState() const {
	if(render_states == nullptr) {
		return default_render_states.get();
	} 
	return render_states;
}

sf::Shader* CViewport_Element::getRenderStateShader() const {
	if(render_states_shader == nullptr) {
		return default_render_states_shader.get();
	} 
	return render_states_shader;
}

sf::Glsl::Vec4* CViewport_Element::getTone() 
{
	return &tone; 
};

void CViewport_Element::updatetone()
{
	if (getRenderStateShader())
	{
		getRenderStateShader()->setUniform("tone", tone);
	}
}

void CViewport_Element::bindTone(CTone_Element * tone) {
	if(tone != linkedTone) {
		auto lastLinked = linkedTone;
		linkedTone = nullptr;
		if(lastLinked != nullptr) {
			lastLinked->bindViewport(nullptr);
		}
		linkedTone = tone;
		if(linkedTone != nullptr) {
			linkedTone->bindViewport(this);
		}
	}
}

CTone_Element* CViewport_Element::getBoundTone()
{
	return linkedTone;
}

sf::Color* CViewport_Element::check_up_color() const
{
	sf::Color* col = reinterpret_cast<sf::Color*>(RDATA(rColor)->data);
	if (*col != *color_copy)
	{
		sf::Glsl::Vec4 color(col->r / 255.0f, col->g / 255.0f, col->b / 255.0f, col->a / 255.0f);
		getRenderStateShader()->setUniform("color", color);
		color_copy->r = col->r;
		color_copy->g = col->g;
		color_copy->b = col->b;
		color_copy->a = col->a;
	}
	return col;
}

/* ---- Shader Part ---- */
/* Thaks to https://github.com/Ancurio/mkxp/blob/master/shader/sprite.frag (Ancurio & urkle) */
const std::string ViewportGlobalFragmentShader = \
	"uniform sampler2D texture;" \
	"uniform vec4 tone;" \
	"float opacity = 0.1;" \ 
	"uniform vec4 color;" \
	"const vec3 lumaF = vec3(.299, .587, .114);" \
	"void main()" \
	"{" \
	"   vec4 frag = texture2D(texture, gl_TexCoord[0].xy);" \
	"   float luma = dot(frag.rgb, lumaF);" \
	"   frag.rgb = mix(frag.rgb, vec3(luma), tone.w);" \
	"   frag.a *= opacity;" \
	"   frag.rgb = mix(frag.rgb, color.rgb, color.a);" \
	"   gl_FragColor = frag;" \
	"}";


sf::Glsl::Vec4 __Viewport_reset_tone(0.0f, 0.0f, 0.0f, 0.0f);


/*void CViewport_Element::reset_render()
{
	if(render == nullptr)
		return;
	const sf::Vector2f sz = getView()->getSize();
	render->create(static_cast<unsigned int>(sz.x), static_cast<unsigned int>(sz.y));
}*/

void CViewport_Element::create_render()
{
	// If the global viewport render doesn't exist, we create it
	if (!render)
	{
		const auto& graphics = CGraphics::Get();
		render = std::make_unique<sf::RenderTexture>();
		render->create(graphics.screenWidth(), graphics.screenHeight());
		render->setSmooth(graphics.smoothScreen());
		render_sprite = std::make_unique<sf::Sprite>();
	}
	// Return if the render texture (internal_texture) is already created
	if(default_render_states.get() != nullptr)
		return;
	// Creation of the render states
	default_render_states_shader = CGraphics::Get().createUniqueShader();
	if(default_render_states_shader != nullptr) {
		// Shader initialialization
		sf::Shader& shader = *default_render_states_shader;
		if (shader.loadFromMemory(ViewportGlobalFragmentShader, sf::Shader::Fragment))
		{
			shader.setUniform("texture", sf::Shader::CurrentTexture);
			shader.setUniform("tone", __Viewport_reset_tone);
			shader.setUniform("color", __Viewport_reset_tone);
		}
	}
	default_render_states = std::make_unique<sf::RenderStates>(default_render_states_shader.get());
	color_copy = std::make_unique<sf::Color>(0, 0, 0, 0);
}

void CViewport_Element::setVisible(bool value)
{
	visible = value;
}

