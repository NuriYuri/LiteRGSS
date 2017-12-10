#ifndef LITERGSS_HEADER_FILE
#define LITERGSS_HEADER_FILE
#include <iostream>
#include <SFML/Graphics.hpp>

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

extern VALUE rb_eRGSSError;

extern ID rb_iElementTable;

void Init_Graphics();

void Init_Bitmap();
void Init_Sprite();
void Init_Rect();
void Init_Viewport();

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

#endif
