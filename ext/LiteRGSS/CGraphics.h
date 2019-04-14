#ifndef CGraphics_H
#define CGraphics_H
#include <memory>
#include "ruby.h"
#include "CGraphicsConfig.h"
#include "CGraphicsDraw.h"
#include "CGraphicsSnapshot.h"
#include "CDrawableStack.h"

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

    void updateSelf(VALUE self);
    void init();
    void stop();
    bool isGameWindowOpen() const;
    void protect();
    void update(VALUE self, bool input = true);
    void updateOnlyInput(VALUE self);

    VALUE takeSnapshot();
    void freeze(VALUE self);
    void transition(VALUE self, int argc, VALUE* argv);
    void resizeScreen(int width, int height); 
    void setShader(sf::RenderStates* shader);
    void syncStackCppFromRuby();
    void add(CDrawable_Element& element);

private:
    void warnIfGraphicsUpdate() const;
    CGraphics();

    void manageErrorMessage(VALUE self, const GraphicsUpdateMessage& message);
    sf::RenderTarget& updateDrawPreProc(sf::View& defview);
    void updateDrawPostProc();
    void updateProcessEvent(GraphicsUpdateMessage& message);
   
    bool InsideGraphicsUpdate = false;
    unsigned long frame_count = 0;

    std::unique_ptr<sf::RenderWindow> game_window = nullptr;

    CGraphicsConfigLoader m_configLoader;
    CGraphicsSnapshot m_snapshot;
    CGraphicsDraw m_draw;
};

#endif
