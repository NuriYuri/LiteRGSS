#include "LiteRGSS.h"
#include "CViewport_Element.h"
#include "CRect_Element.h"
#include "Graphics.local.h"

sf::RenderTexture* CViewport_Element::render = nullptr;
sf::Sprite* CViewport_Element::render_sprite = nullptr;

CViewport_Element::CViewport_Element() : CDrawable_Element() 
{
	linkedTone = nullptr;
	render_states = nullptr;
	color_copy = nullptr;
	rRenderState = Qnil;
}

CViewport_Element::~CViewport_Element() 
{
    if(!game_window || !game_window->isOpen())
        std::cerr << "Game window release thus viewport " << this << " not freed." << std::endl;
	if (rRenderState == Qnil && render_states)
	{
		if (render_states->shader)
		{
			delete render_states->shader;
			render_states->shader = nullptr;
		}
		delete render_states;
	}
	if(color_copy)
		delete color_copy;
};

void CViewport_Element::draw(sf::RenderTarget& target) const
{
	if (!visible)
		return;
    if(render_states)
    {
		sf::Color* col;
		CRect_Element* rect = getLinkedRect();
		sf::IntRect* intrect = rect ? rect->getRect() : nullptr;
        // Loading Window View
       /* sf::View wview = view;
        wview.setRotation(0);
		wview.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
		wview.setSize(ScreenWidth, ScreenHeight);
        target.setView(wview);*/
		// Loading Target view
		const sf::Vector2f view_size = view.getSize();
		sf::View tview = view;
		//tview.setViewport(sf::FloatRect(0.0f, 0.0f, view_size.x / ScreenWidth, view_size.y / ScreenHeight));
		render->setView(tview);
		// Clearing render
		if (render_states->shader)
			col = check_up_color();
		else
			col = const_cast<sf::Color*>(&sf::Color::Transparent);
        render->clear(*col);
        // Drawing sprites
        for(auto sp = stack.begin();sp != stack.end();sp++)
            (*sp)->drawFast(*render);
        render->display();
		// Update internal texture & draw result to Window
		render_sprite->setTexture(render->getTexture());
		if (rect)
		{
			render_sprite->setTextureRect(*intrect);
			render_sprite->setPosition(intrect->left, intrect->top);
		}
		//render_sprite->setTextureRect(sf::IntRect(0, 0, static_cast<int>(view_size.x), static_cast<int>(view_size.y)));
		target.draw(*render_sprite, *render_states);
		/*sf::Sprite sp(render->getTexture());
		sp.setTextureRect(sf::IntRect(0, 0, static_cast<int>(view_size.x), static_cast<int>(view_size.y)));
        target.draw(sp, *render_states);*/
    }
    else
    {
        target.setView(view);
        drawFast(target);
    }
}

void CViewport_Element::drawFast(sf::RenderTarget& target) const 
{
    for(auto sp = stack.begin();sp != stack.end();sp++)
    {
        (*sp)->drawFast(target);
    }
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

sf::RenderStates * CViewport_Element::getRenderStates()
{
	return render_states;
}

void CViewport_Element::setRenderStates(sf::RenderStates * states)
{
	if (rRenderState == Qnil && render_states)
	{
		if (render_states->shader)
		{
			delete render_states->shader;
			render_states->shader = nullptr;
		}
	}
	render_states = states;
}

void CViewport_Element::bind(CDrawable_Element* sprite)
{
    //stack.push_back(sprite);
    sprite->setOriginStack(&stack);
}

void CViewport_Element::clearStack() 
{
    stack.clear();
}

sf::Glsl::Vec4* CViewport_Element::getTone() 
{
	return &tone; 
};

void CViewport_Element::updatetone()
{
	if(render_states && render_states->shader)
		const_cast<sf::Shader*>(render_states->shader)->setUniform("tone", tone);
}

void CViewport_Element::setLinkedTone(CTone_Element * _tone)
{
	linkedTone = _tone;
}

CTone_Element * CViewport_Element::getLinkedTone()
{
	return linkedTone;
}

sf::Color* CViewport_Element::check_up_color() const
{
	sf::Color* col = reinterpret_cast<sf::Color*>(RDATA(rColor)->data);
	if (*col != *color_copy)
	{
		sf::Glsl::Vec4 color(col->r / 255.0f, col->g / 255.0f, col->b / 255.0f, col->a / 255.0f);
		const_cast<sf::Shader*>(render_states->shader)->setUniform("color", color);
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
    "uniform vec4 color;" \
    "const vec3 lumaF = vec3(.299, .587, .114);" \
    "void main()" \
    "{" \
    "   vec4 frag = texture2D(texture, gl_TexCoord[0].xy);" \
    /*"   frag.rgb = mix(frag.rgb, gl_Color.rgb, cola);" \*/
    "   frag.rgb = mix(frag.rgb, color.rgb, color.a);" \
    "   float luma = dot(frag.rgb, lumaF);" \
    "   frag.rgb += tone.rgb;" \
    "   frag.rgb = mix(frag.rgb, vec3(luma), tone.w);" \
    "   frag.a *= gl_Color.a;" \
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
		render = new sf::RenderTexture();
		render->create(ScreenWidth, ScreenHeight);
		render->setSmooth(SmoothScreen);
		render_sprite = new sf::Sprite();
	}
	// Return if the render texture (internal_texture) is already created
    if(render_states != nullptr)
        return;
	// Creation of the render states
	render_states = new sf::RenderStates(new sf::Shader());
	// Shader initialialization
	sf::Shader* shader = const_cast<sf::Shader*>(render_states->shader);
	if (shader->loadFromMemory(ViewportGlobalFragmentShader, sf::Shader::Fragment))
	{
		shader->setUniform("texture", sf::Shader::CurrentTexture);
		shader->setUniform("tone", __Viewport_reset_tone);
		shader->setUniform("color", __Viewport_reset_tone);
	}
	color_copy = new sf::Color(0, 0, 0, 0);
}

void CViewport_Element::setVisible(bool value)
{
	visible = value;
}

const std::vector<CDrawable_Element*>* CViewport_Element::getStack()
{
	return &stack;
}
