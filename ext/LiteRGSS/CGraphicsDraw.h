#ifndef CGraphicsDraw_H
#define CGraphicsDraw_H
#include <memory>
#include "ruby.h"

class CGraphicsStack_Element;
struct CGraphicsConfig;
class CGraphicsDraw {
public:
    long screenWidth() const { return ScreenWidth; }
    long screenHeight() const { return ScreenHeight; }
    bool smoothScreen() const { return SmoothScreen; }
    double scale() const { return Graphics_Scale; }
    unsigned char brightness() const { return Graphics_Brightness; }
    void setBrightness(unsigned char brightness) { Graphics_Brightness = brightness; }
    unsigned long frameRate() const { return frame_rate; }

    void init(sf::RenderWindow& window, const CGraphicsConfig& vSync);
    VALUE takeSnapshot();

    VALUE freeze(VALUE self);
    void transition(VALUE self, int argc, VALUE* argv);
    void resizeScreen(VALUE self, VALUE width, VALUE height); 
    void setShader(sf::RenderStates* shader);

    void update(CGraphicsStack_Element& stack);

    void stop();
private:

    void initRender();
    void loadShader();
    void takeSnapshotOn(sf::Texture& text) const;
    void drawBrightness();

    void transitionBasic(VALUE self, long time);
    void transitionRGSS(VALUE self, long time, VALUE bitmap);

    sf::RenderTarget& updateDrawPreProc(sf::View& defview);
    void updateDrawPostProc();

    void protect();

    sf::RenderWindow* game_window = nullptr;

    std::unique_ptr<sf::Texture> Graphics_freeze_texture = nullptr;
    std::unique_ptr<sf::Sprite> Graphics_freeze_sprite = nullptr;
    std::unique_ptr<sf::Shader> Graphics_freeze_shader = nullptr;
    
    bool RGSSTransition = false;
    std::unique_ptr<sf::RenderTexture> Graphics_Render = nullptr;
    sf::RenderStates* Graphics_States = nullptr;

    unsigned long frame_rate = 60;

    unsigned char Graphics_Brightness = 255;
    long ScreenWidth = 640;
    long ScreenHeight = 480;
    double Graphics_Scale = 1;
    bool SmoothScreen = false;
};

#endif
