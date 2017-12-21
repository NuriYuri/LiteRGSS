#include "CViewport_Element.h"

void CViewport_Element::draw(sf::RenderTarget& target) const
{
    if(linkedTone)
    {
        if(!render)
            return;
        target.setView(target.getDefaultView());
        globalshader->setUniform("tone", tone);
        render->clear(sf::Color(0,0,0,0));
        render->setView(view);
        for(auto sp = stack.begin();sp != stack.end();sp++)
            (*sp)->drawFast(*render);
        render->display();
        sf::Sprite sp(render->getTexture());
        target.draw(sp, globalshader);
        reset_globalshader();
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
    stack.push_back(sprite);
}

void CViewport_Element::clearStack() 
{
    stack.clear();
}

/* ---- Shader Part ---- */
/* Thaks to https://github.com/Ancurio/mkxp/blob/master/shader/sprite.frag (Ancurio & urkle) */
const std::string ViewportGlobalFragmentShader = \
    "uniform sampler2D texture;" \
    "uniform vec4 tone;" \
    "const vec3 lumaF = vec3(.299, .587, .114);" \
    "void main()" \
    "{" \
    "   vec4 frag = texture2D(texture, gl_TexCoord[0].xy);" \
    /*"   frag.rgb = mix(frag.rgb, gl_Color.rgb, cola);" \*/
    "   float luma = dot(frag.rgb, lumaF);" \
    "   frag.rgb += tone.rgb;" \
    "   frag.rgb = mix(frag.rgb, vec3(luma), tone.w);" \
    "   frag.a *= gl_Color.a;" \
    "   gl_FragColor = frag;" \
    "}";

sf::Shader* CViewport_Element::globalshader = nullptr;

void CViewport_Element::load_globalshader()
{
    if(globalshader == nullptr)
        globalshader = new sf::Shader();
    if(globalshader->loadFromMemory(ViewportGlobalFragmentShader, sf::Shader::Fragment))
    {
        globalshader->setUniform("texture", sf::Shader::CurrentTexture);
        reset_globalshader();
    }
}


sf::Glsl::Vec4 __Viewport_reset_tone(0.0f, 0.0f, 0.0f, 0.0f);

void CViewport_Element::reset_globalshader()
{
    globalshader->setUniform("tone", __Viewport_reset_tone);
}

void CViewport_Element::reset_render()
{
    if(render == nullptr)
        return;
    const sf::Vector2f sz = getView()->getSize();
    render->create(static_cast<unsigned int>(sz.x), static_cast<unsigned int>(sz.y));
}

void CViewport_Element::create_render()
{
    render = new sf::RenderTexture();
    const sf::Vector2f sz = getView()->getSize();
    render->create(static_cast<unsigned int>(sz.x), static_cast<unsigned int>(sz.y));
}