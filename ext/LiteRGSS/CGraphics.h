#ifndef CGraphics_H
#define CGraphics_H
#include <memory>
#include "ruby.h"
#include "CGraphicsStack_Element.h"
#include "CGraphicsConfig.h"

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

    long screenWidth() const {
        return ScreenWidth;
    }

    long screenHeight() const {
        return ScreenHeight;
    }

    bool smoothScreen() const {
        return SmoothScreen;
    }

    unsigned long frameCount() const {
        return frame_count;
    }

    void setFrameCount(unsigned long frameCount) {
        frame_count = frameCount;
    }

    unsigned long frameRate() const {
        return frame_rate;
    }

    double scale() const {
        return Graphics_Scale;
    }

    unsigned char brightness() const {
        return Graphics_Brightness;
    }

    void setBrightness(unsigned char brightness) {
        Graphics_Brightness = brightness;
    }

    VALUE init(VALUE self);
    VALUE update(VALUE self);
    void loadShader();
    VALUE takeSnapshot();
    void initRender();
    void stop();
    VALUE freeze(VALUE self);
    void transition(VALUE self, int argc, VALUE* argv);
    VALUE updateNoInputCount(VALUE self);
    VALUE updateOnlyInput(VALUE self);
    void reloadStack();
    void bind(CDrawable_Element& element);
    void resizeScreen(VALUE self, VALUE width, VALUE height); 
    void setShader(sf::RenderStates* shader);

    void updateDraw();
    bool isGameWindowOpen() const;
private:
    CGraphics();

    void takeSnapshotOn(sf::Texture& text) const;
    void drawBrightness();

    void transitionBasic(VALUE self, long time);
    void transitionRGSS(VALUE self, long time, VALUE bitmap);

    void setSmoothScreen(bool smoothScreen) {
        SmoothScreen = smoothScreen;
    }

    void setSelf(VALUE self) {
        Graphics_stack = std::make_unique<CGraphicsStack_Element>(self);
    }

    VALUE updateRaiseError(VALUE self, const GraphicUpdateMessage& message);
    sf::RenderTarget& updateDrawPreProc(sf::View& defview);
    void updateDrawPostProc();
    void updateProcessEvent(GraphicUpdateMessage& message);
    
    bool clearStack();
    void protect();

    bool SmoothScreen = false;
    long ScreenWidth = 640;
    long ScreenHeight = 480;
    double Graphics_Scale = 1;
    unsigned char Graphics_Brightness = 255;
    unsigned long frame_count = 0;
    unsigned long frame_rate = 60;

    bool InsideGraphicsUpdate = false;

    std::unique_ptr<sf::Texture> Graphics_freeze_texture = nullptr;
    std::unique_ptr<sf::Sprite> Graphics_freeze_sprite = nullptr;
    std::unique_ptr<sf::Shader> Graphics_freeze_shader = nullptr;

    std::unique_ptr<sf::RenderWindow> game_window = nullptr;
    std::unique_ptr<CGraphicsStack_Element> Graphics_stack;
    
    bool RGSSTransition = false;
    std::unique_ptr<sf::RenderTexture> Graphics_Render = nullptr;
    sf::RenderStates* Graphics_States = nullptr;

    CGraphicsConfigLoader m_configLoader;
};

#endif
