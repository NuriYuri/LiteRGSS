#ifndef L_TONE_H
#define L_TONE_H

VALUE rb_Tone_Initialize(int argc, VALUE* argv, VALUE self); // Also .set
VALUE rb_Tone_InitializeCopy(VALUE self, VALUE original);
VALUE rb_Tone_getRed(VALUE self);
VALUE rb_Tone_setRed(VALUE self, VALUE val);
VALUE rb_Tone_getGreen(VALUE self);
VALUE rb_Tone_setGreen(VALUE self, VALUE val);
VALUE rb_Tone_getBlue(VALUE self);
VALUE rb_Tone_setBlue(VALUE self, VALUE val);
VALUE rb_Tone_getGray(VALUE self);
VALUE rb_Tone_setGray(VALUE self, VALUE val);

#endif