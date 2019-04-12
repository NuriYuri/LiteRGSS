#include "ruby_common.h"
#include "common.h"
#include "CGraphicsConfig.h"

CGraphicsVideoConfig CGraphicsConfigLoader::loadVideoFromConfigs() const {
    sf::VideoMode vmode(640, 480, 32);

    ID swidth = rb_intern("ScreenWidth");
    ID sheight = rb_intern("ScreenHeight");
    ID sscale = rb_intern("ScreenScale");
    long max_width, max_height = 0xFFFFFF;
    double scale = 1;
    std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
    /* If there's a fullscreen mode */
    if(modes.size() > 0) {
        max_width = modes[0].width;
        max_height = modes[0].height;
        vmode.bitsPerPixel = modes[0].bitsPerPixel;
    }
    /* Adjust Width */
    if(rb_const_defined(rb_mConfig, swidth))
        vmode.width = normalize_long(rb_num2long(rb_const_get(rb_mConfig, swidth)), 160, max_width);
    /* Adjust Height */
    if(rb_const_defined(rb_mConfig, sheight))
        vmode.height = normalize_long(rb_num2long(rb_const_get(rb_mConfig, sheight)), 144, max_height);
    /* Adjust Scale */
    if(rb_const_defined(rb_mConfig, sscale))
        scale = normalize_double(NUM2DBL(rb_const_get(rb_mConfig, sscale)), 0.1, 10);
    auto lastWidth = vmode.width;
    auto lastHeight = vmode.height;
    vmode.width *= scale;
    vmode.height *= scale;
    return {std::move(vmode), lastWidth, lastHeight, scale};
}

bool CGraphicsConfigLoader::loadSmoothScreenFromConfigs() const {
    ID fsc = rb_intern("SmoothScreen");
    return rb_const_defined(rb_mConfig, fsc) && RTEST(rb_const_get(rb_mConfig, fsc));
}

sf::String CGraphicsConfigLoader::loadTitleFromConfigs() const {
    ID title = rb_intern("Title");
    if(rb_const_defined(rb_mConfig, title)) {
        VALUE str_title = rb_const_get(rb_mConfig, title);
        rb_check_type(str_title, T_STRING);
        std::string str(RSTRING_PTR(str_title));
        return sf::String::fromUtf8(str.begin(), str.end());
    }
    return sf::String("LiteRGSS");
}

unsigned int CGraphicsConfigLoader::loadFrameRateFromConfigs() const {
    ID framerate = rb_intern("FrameRate");
    if(rb_const_defined(rb_mConfig, framerate)) {
        return normalize_long(rb_num2long(rb_const_get(rb_mConfig, framerate)), 1, 120);
    }
    return 60;
}

bool CGraphicsConfigLoader::loadVSYNCFromConfigs() const {
    ID vsync = rb_intern("Vsync");
    if(rb_const_defined(rb_mConfig, vsync)) {
        return RTEST(rb_const_get(rb_mConfig, vsync));
    }
    return true;
}

bool CGraphicsConfigLoader::loadFullScreenFromConfigs() const {
    ID fsc = rb_intern("FullScreen");
    if(rb_const_defined(rb_mConfig, fsc)) {
        return RTEST(rb_const_get(rb_mConfig, fsc));
    }
    return false;
}

CGraphicsConfig CGraphicsConfigLoader::load() const {
    auto vmode = loadVideoFromConfigs();
    auto smoothScreen = loadSmoothScreenFromConfigs();
    auto title = loadTitleFromConfigs();
    auto frameRate = loadFrameRateFromConfigs();
    auto vSync = loadVSYNCFromConfigs();
    auto fullscreen = loadFullScreenFromConfigs();
    return {
        std::move(vmode),
        smoothScreen,
        std::move(title),
        frameRate,
        vSync,
        fullscreen
    };
}
