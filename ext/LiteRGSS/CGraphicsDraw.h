#ifndef CGraphicsDraw_H
#define CGraphicsDraw_H
#include <memory>
#include "ruby.h"
#include "CGraphicsUpdateMessage.h"
#include "CRubyGlobalBitmaps.h"
#include "CRubyGraphicsStack.h"

class CGraphicsStack_Element;
class CDrawable_Element;
struct CGraphicsConfig;
class CGraphicsSnapshot;

class CGraphicsDraw {
public:
    CGraphicsDraw(CGraphicsSnapshot& snapshot);
    ~CGraphicsDraw();

    long screenWidth() const { return m_screenWidth; }
    long screenHeight() const { return m_screenHeight; }
    bool smoothScreen() const { return m_smoothScreen; }
    double scale() const { return m_scale; }
    unsigned char brightness() const { return m_brightness; }
    void setBrightness(unsigned char brightness) { m_brightness = brightness; }
    unsigned long frameRate() const { return m_frameRate; }

    void init(sf::RenderWindow& window, const CGraphicsConfig& vSync);
    void resizeScreen(int width, int height); 
    void setShader(sf::RenderStates* shader);

    std::unique_ptr<GraphicsUpdateMessage> update();
    void updateInternal();
    bool isGameWindowOpen() const;
    void syncStackCppFromRuby();
    void clearRubyStack();
    void bind(CDrawable_Element& element);
    void stop();

    void updateSelf(VALUE self) {
        m_rubyStack = std::make_unique<CRubyGraphicsStack>(self);
    }

private:
    void initRender();
    
    void drawBrightness();

    sf::RenderTarget& updateDrawPreProc(sf::View& defview);
    void updateDrawPostProc();

    CGraphicsStack_Element m_stack;
    std::unique_ptr<CRubyGraphicsStack> m_rubyStack;
    CRubyGlobalBitmaps m_globalBitmaps;

    sf::RenderStates* m_renderState = nullptr;
    sf::RenderWindow* m_gameWindow = nullptr;
    std::unique_ptr<sf::RenderTexture> m_renderTexture = nullptr;
    CGraphicsSnapshot& m_snapshot;

    unsigned long m_frameRate = 60;
    unsigned char m_brightness = 255;
    long m_screenWidth = 640;
    long m_screenHeight = 480;
    double m_scale = 1;
    bool m_smoothScreen = false;
};

#endif
