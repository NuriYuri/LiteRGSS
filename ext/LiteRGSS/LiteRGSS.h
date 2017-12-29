#ifndef LITERGSS_HEADER_FILE
#define LITERGSS_HEADER_FILE
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "ruby.h" // Windows Patch
#include <SFML/Graphics.hpp>
#include "Graphics.h"
#include "Color.h"
#include "Tone.h"
#include "Rect.h"
#include "Bitmap.h"
#include "Viewport.h"
#include "Sprite.h"
#include "Fonts.h"
#include "Text.h"
#include "Input.h"
#include "Table.h"

#define _rbf (VALUE (*)(...))

extern long ScreenWidth;
extern long ScreenHeight;
extern VALUE rb_mLiteRGSS;
extern VALUE rb_mGraphics;
extern VALUE rb_mConfig;
extern VALUE rb_cBitmap;
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

extern VALUE rb_eRGSSError;
extern VALUE rb_eStoppedGraphics;
extern VALUE rb_eClosedWindow;

extern ID rb_iElementTable;

void Init_Graphics();

void Init_Bitmap();
void Init_Sprite();
void Init_Rect();
void Init_Viewport();
void Init_Color();
void Init_Tone();
void Init_Fonts();
void Init_Text();
void Init_Input();
void Init_Table();

inline long normalize_long(long value, long min, long max) {
    if(value < min)
        return min;
    if(value > max)
        return max;
    return value;
}
inline double normalize_double(double value, double min, double max) {
    if(value < min)
        return min;
    if(value > max)
        return max;
    return value;
}

inline void rect_copy(sf::IntRect* dest, const sf::IntRect* src) {
    dest->left = src->left;
    dest->top = src->top;
    dest->width = src->width;
    dest->height = src->height;
}

inline void tone_copy(sf::Glsl::Vec4* dest, const sf::Glsl::Vec4* src)
{
    dest->x = src->x;
    dest->y = src->y;
    dest->z = src->z;
    dest->w = src->w;
}
#endif
