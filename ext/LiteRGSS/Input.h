#ifndef L_INPUT_HEADER
#define L_INPUT_HEADER

void L_Input_Reset_Clocks();
void L_Input_Update_Key(sf::Keyboard::Key code, bool state);
void L_Input_Update_Joy(unsigned int joy_id, unsigned int key, bool state);
void L_Input_Update_JoyPos(unsigned int joy_id, long axis, float position);
void L_Input_Setusec_threshold(long usec);
void L_Input_Mouse_Pos_Update(long x, long y);
void L_Input_Mouse_Wheel_Update(long delta);
void L_Input_Mouse_Button_Update(long button, bool state);

VALUE rb_Input_Press(VALUE self, VALUE key_sym);
VALUE rb_Input_Trigger(VALUE self, VALUE key_sym);
VALUE rb_Input_Repeat(VALUE self, VALUE key_sym);
VALUE rb_Input_Released(VALUE self, VALUE key_sym);
VALUE rb_Input_dir4(VALUE self);
VALUE rb_Input_dir8(VALUE self);

VALUE rb_Mouse_Press(VALUE self, VALUE key_sym);
VALUE rb_Mouse_Trigger(VALUE self, VALUE key_sym);
VALUE rb_Mouse_Released(VALUE self, VALUE key_sym);
VALUE rb_Mouse_x(VALUE self);
VALUE rb_Mouse_y(VALUE self);
VALUE rb_Mouse_Wheel(VALUE self);
VALUE rb_Mouse_Wheel_set(VALUE self, VALUE val);

#endif