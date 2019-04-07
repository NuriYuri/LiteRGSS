#ifndef CGraphics_H
#define CGraphics_H
#include <memory>
#include "ruby.h"
#include "CGraphicsStack_Element.h"
#include "CGraphicsConfig.h"
#include "CGraphicsDraw.h"

struct GraphicUpdateMessage {
    VALUE errorObject;
    std::string message;
};

class CGraphics {
public:
    static CGraphics& Get() {
        static CGraphics cg;
        return cg;
    }

    virtual ~CGraphics();

    unsigned long frameCount() const { return frame_count; }
    void setFrameCount(unsigned long frameCount) { frame_count = frameCount; }
    void setBrightness(unsigned char brightness) { m_draw.setBrightness(brightness); }
    unsigned char brightness() const { return m_draw.brightness(); }
    long screenWidth() const { return m_draw.screenWidth(); }
    long screenHeight() const { return m_draw.screenHeight(); }
    bool smoothScreen() const { return m_draw.smoothScreen(); }
    double scale() const { return m_draw.scale(); }
    auto frameRate() const { return m_draw.frameRate(); }

    VALUE init(VALUE self);
    VALUE update(VALUE self);
    VALUE takeSnapshot();
    
    void stop();
    VALUE freeze(VALUE self);
    void transition(VALUE self, int argc, VALUE* argv);
    VALUE updateNoInputCount(VALUE self);
    VALUE updateOnlyInput(VALUE self);
    void reloadStack();
    void bind(CDrawable_Element& element);
    void resizeScreen(VALUE self, VALUE width, VALUE height); 
    void setShader(sf::RenderStates* shader);

    void draw();
    bool isGameWindowOpen() const;
private:
    CGraphics() = default;

    void initRender();
    void loadShader();
    void drawBrightness();

    void transitionBasic(VALUE self, long time);
    void transitionRGSS(VALUE self, long time, VALUE bitmap);

    VALUE updateRaiseError(VALUE self, const GraphicUpdateMessage& message);
    sf::RenderTarget& updateDrawPreProc(sf::View& defview);
    void updateDrawPostProc();
    void updateProcessEvent(GraphicUpdateMessage& message);
    
    bool clearStack();
    void protect();
   
    bool InsideGraphicsUpdate = false;
    unsigned long frame_count = 0;

    std::unique_ptr<sf::RenderWindow> game_window = nullptr;
    std::unique_ptr<CGraphicsStack_Element> Graphics_stack;

    CGraphicsConfigLoader m_configLoader;
    CGraphicsDraw m_draw;
};

#endif
