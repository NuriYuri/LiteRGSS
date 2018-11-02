#ifndef L_INPUT_HEADER
#define L_INPUT_HEADER

void L_Input_Reset_Clocks();
void L_Input_Update_Key(sf::Keyboard::Key code, bool state);
void L_Input_Update_Joy(unsigned int joy_id, unsigned int key, bool state);
void L_Input_Update_JoyPos(unsigned int joy_id, long axis, float position);
void L_Input_Reset_JoyPos(unsigned int joy_id);
void L_Input_Setusec_threshold(long usec);
void L_Input_Mouse_Pos_Update(int x, int y);
void L_Input_Mouse_Wheel_Update(long delta);
void L_Input_Mouse_Button_Update(long button, bool state);

VALUE rb_Input_Press(VALUE self, VALUE key_sym);
VALUE rb_Input_Trigger(VALUE self, VALUE key_sym);
VALUE rb_Input_Repeat(VALUE self, VALUE key_sym);
VALUE rb_Input_Released(VALUE self, VALUE key_sym);
VALUE rb_Input_dir4(VALUE self);
VALUE rb_Input_dir8(VALUE self);
VALUE rb_Input_setMainJoypad(VALUE self, VALUE id);
VALUE rb_Input_setMainXAxis(VALUE self, VALUE axis);
VALUE rb_Input_setMainYAxis(VALUE self, VALUE axis);
VALUE rb_Input_getMainJoypad(VALUE self);
VALUE rb_Input_getMainXAxis(VALUE self);
VALUE rb_Input_getMainYAxis(VALUE self);
VALUE rb_Input_getText(VALUE self);
VALUE rb_Input_getInvertAxisX(VALUE self);
VALUE rb_Input_setInvertAxisX(VALUE self, VALUE val);
VALUE rb_Input_getInvertAxisY(VALUE self);
VALUE rb_Input_setInvertAxisY(VALUE self, VALUE val);
VALUE rb_Input_JoyConnected(VALUE self, VALUE id);
VALUE rb_Input_JoyGetButtonCount(VALUE self, VALUE id);
VALUE rb_Input_JoyHasAxis(VALUE self, VALUE id, VALUE axis);
VALUE rb_Input_JoyIsButtonPressed(VALUE self, VALUE id, VALUE button);
VALUE rb_Input_JoyGetAxisPosition(VALUE self, VALUE id, VALUE axis);
VALUE rb_Input_JoyGetName(VALUE self, VALUE id);

VALUE rb_Mouse_Press(VALUE self, VALUE key_sym);
VALUE rb_Mouse_Trigger(VALUE self, VALUE key_sym);
VALUE rb_Mouse_Released(VALUE self, VALUE key_sym);
VALUE rb_Mouse_x(VALUE self);
VALUE rb_Mouse_y(VALUE self);
VALUE rb_Mouse_Wheel(VALUE self);
VALUE rb_Mouse_Wheel_set(VALUE self, VALUE val);

VALUE rb_KeyBoard_Press(VALUE self, VALUE val);

extern std::string L_EnteredText;
#endif