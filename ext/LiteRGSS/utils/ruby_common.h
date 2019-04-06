#ifndef L_UTILS_RUBY_COMMON_HEADER
#define L_UTILS_RUBY_COMMON_HEADER

#include "ruby.h"
#define _rbf (VALUE (*)(...))

extern VALUE rb_mLiteRGSS;
extern VALUE rb_cDisposable;
extern VALUE rb_cDrawable;
extern VALUE rb_mGraphics;
extern VALUE rb_mConfig;
extern VALUE rb_cBitmap;
extern VALUE rb_cImage;
extern VALUE rb_cSprite;
extern VALUE rb_cRect;
extern VALUE rb_cViewport;
extern VALUE rb_cColor;
extern VALUE rb_cTone;
extern VALUE rb_mFonts;
extern VALUE rb_cText;
extern VALUE rb_mInput;
extern VALUE rb_mMouse;
extern VALUE rb_cTable;
extern VALUE rb_cTable32;
extern VALUE rb_mYuki;
extern VALUE rb_cYukiGifReader;
extern VALUE rb_cBlendMode;
extern VALUE rb_cShader;
extern VALUE rb_cShaderSprite;
extern VALUE rb_cShape;
extern VALUE rb_cWindow;
extern VALUE rb_cSpriteMap;

extern VALUE rb_eRGSSError;
extern VALUE rb_eStoppedGraphics;
extern VALUE rb_eClosedWindow;

extern ID rb_iElementTable;

#endif