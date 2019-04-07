#ifndef CGraphicsDraw_H
#define CGraphicsDraw_H
#include <memory>
#include "ruby.h"
#include "CGraphicsUpdateMessage.h"

class CGraphicsStack_Element;
class CDrawable_Element;
struct CGraphicsConfig;
class CGraphicsSnapshot;

class CGraphicsDraw {
public:
    CGraphicsDraw(CGraphicsSnapshot& snapshot);
    ~CGraphicsDraw();

    long screenWidth() const { return ScreenWidth; }
    long screenHeight() const { return ScreenHeight; }
    bool smoothScreen() const { return SmoothScreen; }
    double scale() const { return Graphics_Scale; }
    unsigned char brightness() const { return Graphics_Brightness; }
    void setBrightness(unsigned char brightness) { Graphics_Brightness = brightness; }
    unsigned long frameRate() const { return frame_rate; }

    void init(VALUE self, sf::RenderWindow& window, const CGraphicsConfig& vSync);
    void resizeScreen(VALUE self, VALUE width, VALUE height); 
    void setShader(sf::RenderStates* shader);

    std::unique_ptr<GraphicsUpdateMessage> update();
    void updateInternal();
    bool isGameWindowOpen() const;
    void reloadStack();
    void bind(CDrawable_Element& element);
    void clearRubyStack();

    void stop();
private:
    void initRender();
    
    void drawBrightness();

    sf::RenderTarget& updateDrawPreProc(sf::View& defview);
    void updateDrawPostProc();

    std::unique_ptr<CGraphicsStack_Element> Graphics_stack;
    sf::RenderStates* Graphics_States = nullptr;
    sf::RenderWindow* game_window = nullptr;
    std::unique_ptr<sf::RenderTexture> Graphics_Render = nullptr;
    CGraphicsSnapshot& m_snapshot;

    unsigned long frame_rate = 60;
    unsigned char Graphics_Brightness = 255;
    long ScreenWidth = 640;
    long ScreenHeight = 480;
    double Graphics_Scale = 1;
    bool SmoothScreen = false;
};

#endif
