#ifndef LITERGSS_HEADER_FILE
#define LITERGSS_HEADER_FILE
#include <iostream>

#define _rbf (VALUE (*)(...))

extern VALUE rb_mLiteRGSS;
extern VALUE rb_mGraphics;
extern VALUE rb_mConfig;
extern VALUE rb_cBitmap;
extern VALUE rb_cSprite;

extern VALUE rb_eRGSSError;

extern ID rb_iElementTable;

void Init_Graphics();

void Init_Bitmap();
void Init_Sprite();

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


#endif
