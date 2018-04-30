#include "LiteRGSS.h"
#include "CViewport_Element.h"
#include "Graphics.local.h"

unsigned long CViewport_Element::render_count = 0;

CViewport_Element::CViewport_Element() : CDrawable_Element() 
{
	render = nullptr;
	linkedTone = nullptr;
	shader = nullptr;
	color_copy = nullptr;
}

CViewport_Element::~CViewport_Element() 
{
    if(render != nullptr)
    {
        if(!game_window || !game_window->isOpen())
            std::cerr << "Game window release thus viewport " << this << " not freed." << std::endl;
        else
            delete render;
        render_count--;
		delete shader;
		delete color_copy;
    }
    render = nullptr;
};

void CViewport_Element::draw(sf::RenderTarget& target) const
{
	if (!visible)
		return;
    if(linkedTone && render)
    {
        /* Loading the Window View */
        sf::View wview = view;
        sf::View tview = view;
        wview.setRotation(0);
        target.setView(wview);
		sf::Color* col = check_up_color();
        render->clear(*col);
        /* Adjusting the Texture view */
        tview.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f));
        render->setView(tview);
        /* Drawing the sprites */
        for(auto sp = stack.begin();sp != stack.end();sp++)
            (*sp)->drawFast(*render);
        render->display();
        sf::Sprite sp(render->getTexture());
        target.draw(sp, shader);
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
	if(shader)
		shader->setUniform("tone", tone);
}

sf::Color* CViewport_Element::check_up_color() const
{
	sf::Color* col = reinterpret_cast<sf::Color*>(RDATA(rColor)->data);
	if (*col != *color_copy)
	{
		sf::Glsl::Vec4 color(col->r / 255.0f, col->g / 255.0f, col->b / 255.0f, col->a / 255.0f);
		shader->setUniform("color", color);
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


void CViewport_Element::reset_render()
{
    if(render == nullptr)
        return;
    const sf::Vector2f sz = getView()->getSize();
    render->create(static_cast<unsigned int>(sz.x), static_cast<unsigned int>(sz.y));
}

void CViewport_Element::create_render()
{
    if(render != nullptr)
        return;
    if(render_count >= 8)
        rb_raise(rb_eRGSSError, "Maximum render texture exceeded. Try to have a better management of your tone/color viewports.");
    render = new sf::RenderTexture();
    const sf::Vector2f sz = getView()->getSize();
    render->create(static_cast<unsigned int>(sz.x), static_cast<unsigned int>(sz.y));
    render->setSmooth(SmoothScreen);
    render_count++;
	/* Create shader at the same time */
	shader = new sf::Shader();
	if (shader->loadFromMemory(ViewportGlobalFragmentShader, sf::Shader::Fragment))
	{
		shader->setUniform("texture", sf::Shader::CurrentTexture);
		shader->setUniform("tone", __Viewport_reset_tone);
		shader->setUniform("color", __Viewport_reset_tone);
	}
	color_copy = new sf::Color(0, 0, 0, 0);
}