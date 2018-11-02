#include "LiteRGSS.h"

#define L_INPUT_NUM_INGAME_KEY 17
#define L_INPUT_NUM_MOUSE_KEY 5
#define L_TRIGGER_TIME_METHOD 1
#define L_TRIGGER_COUNT_METHOD 2
#define L_TRIGGER_METHOD L_TRIGGER_COUNT_METHOD

VALUE rb_mInput = Qnil;
VALUE rb_mMouse = Qnil;
VALUE rb_mMouseKey = Qnil;
VALUE rb_mInputKey = Qnil;
VALUE rb_mMouseKeyID = Qnil;
VALUE rb_mInputKeyID = Qnil;
VALUE rb_mInputMainJoy = Qnil;
VALUE rb_mInputMainAxisX = Qnil;
VALUE rb_mInputMainAxisY = Qnil;
VALUE rb_mInputInvertAxisX = Qfalse;
VALUE rb_mInputInvertAxisY = Qfalse;
ID rb_mInputEach = Qnil;
/* Clocks used to detect trigger, and repeat */
#if L_TRIGGER_METHOD == L_TRIGGER_TIME_METHOD
sf::Clock L_Input_Clock[L_INPUT_NUM_INGAME_KEY];
sf::Clock L_Mouse_Clock[L_INPUT_NUM_MOUSE_KEY];
#define L_TRIGGER_RESET_Mouse(i) L_Mouse_Clock[i].restart()
#define L_TRIGGER_RESET_Input(i) L_Input_Clock[i].restart()
#define L_Trigger_Mouse(pos) L_Mouse_Clock[pos].getElapsedTime().asMicroseconds() < L_Input_threshold
#define L_Trigger_Input(pos) L_Input_Clock[pos].getElapsedTime().asMicroseconds() < L_Input_threshold
#define L_Repeat_Input(pos) long time = L_Input_Clock[pos].getElapsedTime().asMicroseconds(); \
	if (time >= (L_Input_threshold * 20)) \
		if((time % (L_Input_threshold * 8)) < L_Input_threshold) \
			return Qtrue;
#elif L_TRIGGER_METHOD == L_TRIGGER_COUNT_METHOD
unsigned long L_Input_Count[L_INPUT_NUM_INGAME_KEY];
unsigned long L_Mouse_Count[L_INPUT_NUM_MOUSE_KEY];
#define L_TRIGGER_RESET_Mouse(i) L_Mouse_Count[i] = frame_count
#define L_TRIGGER_RESET_Input(i) L_Input_Count[i] = frame_count
#define L_Trigger_Mouse(pos) L_Mouse_Count[pos] == (frame_count - 1)
#define L_Trigger_Input(pos) L_Input_Count[pos] == (frame_count - 1)
#define L_Repeat_Input(pos) unsigned long count = (frame_count - L_Input_Count[pos]); \
	if(count > (frame_rate / 2)) \
	{ \
		count -= (frame_rate / 2); \
		if ((count % (frame_rate / 6)) == 0) \
			return Qtrue; \
	}
#endif
bool L_Input_Press[L_INPUT_NUM_INGAME_KEY];
bool L_Mouse_Press[L_INPUT_NUM_MOUSE_KEY];
long L_Input_threshold = 0;
long L_Mouse_Wheel_Delta = 0;
double L_Mouse_Pos_X = 0;
double L_Mouse_Pos_Y = 0;
std::string L_EnteredText = std::string();

unsigned int L_Input_Get_Key_Position(VALUE hash_ids, VALUE hash_key);

void Init_Input()
{
    rb_mInput = rb_define_module_under(rb_mLiteRGSS, "Input");
    rb_mMouse = rb_define_module_under(rb_mLiteRGSS, "Mouse");
    VALUE rb_mKeyboard = rb_define_module_under(rb_mInput, "Keyboard");
    /* function definition */
	rb_define_module_function(rb_mKeyboard, "press?", _rbf rb_KeyBoard_Press, 1);

    rb_define_module_function(rb_mInput, "press?", _rbf rb_Input_Press, 1);
    rb_define_module_function(rb_mInput, "trigger?", _rbf rb_Input_Trigger, 1);
    rb_define_module_function(rb_mInput, "repeat?", _rbf rb_Input_Repeat, 1);
    rb_define_module_function(rb_mInput, "released?", _rbf rb_Input_Released, 1);
    rb_define_module_function(rb_mInput, "dir4", _rbf rb_Input_dir4, 0);
    rb_define_module_function(rb_mInput, "dir8", _rbf rb_Input_dir8, 0);
    rb_define_module_function(rb_mInput, "main_joy", _rbf rb_Input_getMainJoypad, 0);
    rb_define_module_function(rb_mInput, "main_joy=", _rbf rb_Input_setMainJoypad, 1);
    rb_define_module_function(rb_mInput, "x_axis", _rbf rb_Input_getMainXAxis, 0);
    rb_define_module_function(rb_mInput, "x_axis=", _rbf rb_Input_setMainXAxis, 1);
    rb_define_module_function(rb_mInput, "y_axis", _rbf rb_Input_getMainYAxis, 0);
    rb_define_module_function(rb_mInput, "y_axis=", _rbf rb_Input_setMainYAxis, 1);
	rb_define_module_function(rb_mInput, "x_axis_inverted", _rbf rb_Input_getInvertAxisX, 0);
	rb_define_module_function(rb_mInput, "x_axis_inverted=", _rbf rb_Input_setInvertAxisX, 1);
	rb_define_module_function(rb_mInput, "y_axis_inverted", _rbf rb_Input_getInvertAxisY, 0);
	rb_define_module_function(rb_mInput, "y_axis_inverted=", _rbf rb_Input_setInvertAxisY, 1);
	rb_define_module_function(rb_mInput, "get_text", _rbf rb_Input_getText, 0);
	rb_define_module_function(rb_mInput, "joy_connected?", _rbf rb_Input_JoyConnected, 1);
	rb_define_module_function(rb_mInput, "joy_button_count", _rbf rb_Input_JoyGetButtonCount, 1);
	rb_define_module_function(rb_mInput, "joy_has_axis?", _rbf rb_Input_JoyHasAxis, 2);
	rb_define_module_function(rb_mInput, "joy_button_press?", _rbf rb_Input_JoyIsButtonPressed, 2);
	rb_define_module_function(rb_mInput, "joy_axis_position", _rbf rb_Input_JoyGetAxisPosition, 2);
	rb_define_module_function(rb_mInput, "joy_name", _rbf rb_Input_JoyGetName, 1);

    rb_define_module_function(rb_mMouse, "press?", _rbf rb_Mouse_Press, 1);
    rb_define_module_function(rb_mMouse, "trigger?", _rbf rb_Mouse_Trigger, 1);
    rb_define_module_function(rb_mMouse, "released?", _rbf rb_Mouse_Released, 1);
    rb_define_module_function(rb_mMouse, "x", _rbf rb_Mouse_x, 0);
    rb_define_module_function(rb_mMouse, "y", _rbf rb_Mouse_y, 0);
    rb_define_module_function(rb_mMouse, "wheel", _rbf rb_Mouse_Wheel, 0);
    rb_define_module_function(rb_mMouse, "wheel=", _rbf rb_Mouse_Wheel_set, 1);

    /* usefull id */
    rb_mInputEach = rb_intern("each_key");
    /* Constants definition */
    VALUE tmp, key;
    rb_mMouseKey = rb_hash_new();
    rb_mMouseKeyID = rb_hash_new();
    rb_define_const(rb_mMouse, "Keys", rb_mMouseKey);
    rb_gc_register_address(&rb_mMouseKey); // Protect the Hash from being GC'd
    rb_gc_register_address(&rb_mMouseKeyID);
    RHASH_SET_IFNONE(rb_mMouseKey, tmp = LONG2NUM(sf::Mouse::Button::Left));
    RHASH_SET_IFNONE(rb_mMouseKeyID, LONG2FIX(0));
    rb_hash_aset(rb_mMouseKey, key = rb_id2sym(rb_intern("LEFT")), tmp);
    rb_hash_aset(rb_mMouseKeyID, key, LONG2FIX(0));
    rb_hash_aset(rb_mMouseKey, key = rb_id2sym(rb_intern("left")), tmp);
    rb_hash_aset(rb_mMouseKeyID, key, LONG2FIX(0));
    rb_hash_aset(rb_mMouseKey, key = rb_id2sym(rb_intern("RIGHT")), tmp = LONG2NUM(sf::Mouse::Button::Right));
    rb_hash_aset(rb_mMouseKeyID, key, LONG2FIX(1));
    rb_hash_aset(rb_mMouseKey, key = rb_id2sym(rb_intern("right")), tmp);
    rb_hash_aset(rb_mMouseKeyID, key, LONG2FIX(1));
    rb_hash_aset(rb_mMouseKey, key = rb_id2sym(rb_intern("MIDDLE")), tmp = LONG2NUM(sf::Mouse::Button::Middle));
    rb_hash_aset(rb_mMouseKeyID, key, LONG2FIX(2));
    rb_hash_aset(rb_mMouseKey, key = rb_id2sym(rb_intern("middle")), tmp);
    rb_hash_aset(rb_mMouseKeyID, key, LONG2FIX(2));
    rb_hash_aset(rb_mMouseKey, key = rb_id2sym(rb_intern("X1")), tmp = LONG2NUM(sf::Mouse::Button::XButton1));
    rb_hash_aset(rb_mMouseKeyID, key, LONG2FIX(3));
    rb_hash_aset(rb_mMouseKey, key = rb_id2sym(rb_intern("x1")), tmp);
    rb_hash_aset(rb_mMouseKeyID, key, LONG2FIX(3));
    rb_hash_aset(rb_mMouseKey, key = rb_id2sym(rb_intern("X2")), tmp = LONG2NUM(sf::Mouse::Button::XButton2));
    rb_hash_aset(rb_mMouseKeyID, key, LONG2FIX(4));
    rb_hash_aset(rb_mMouseKey, key = rb_id2sym(rb_intern("x2")), tmp);
    rb_hash_aset(rb_mMouseKeyID, key, LONG2FIX(4));

    rb_mInputKey = rb_hash_new();
    rb_mInputKeyID = rb_hash_new();
    rb_define_const(rb_mInput, "Keys", rb_mInputKey);
    rb_gc_register_address(&rb_mInputKey); // Protect the Hash from being GC'd
    rb_gc_register_address(&rb_mInputKeyID);
    RHASH_SET_IFNONE(rb_mInputKey, tmp = rb_ary_new());
    RHASH_SET_IFNONE(rb_mInputKeyID, LONG2FIX(0));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("A")), tmp);
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(0));
    rb_ary_push(tmp, LONG2NUM(sf::Keyboard::C));
    rb_ary_push(tmp, LONG2NUM(sf::Keyboard::Return));
    rb_ary_push(tmp, LONG2NUM(sf::Keyboard::Space));
	rb_ary_push(tmp, LONG2NUM(-1)); // A on XBox360 (first joy)
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("B")), tmp = rb_ary_new());
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(1));
    rb_ary_push(tmp, LONG2NUM(sf::Keyboard::X));
    rb_ary_push(tmp, LONG2NUM(sf::Keyboard::Escape));
    rb_ary_push(tmp, LONG2NUM(sf::Keyboard::LShift));
    rb_ary_push(tmp, LONG2NUM(sf::Keyboard::RShift));
    rb_ary_push(tmp, LONG2NUM(sf::Keyboard::BackSpace));
	rb_ary_push(tmp, LONG2NUM(-2)); // B on XBox360 (first joy)
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("X")), tmp = rb_ary_new());
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(2));
    rb_ary_push(tmp, LONG2NUM(sf::Keyboard::V));
    rb_ary_push(tmp, LONG2NUM(sf::Keyboard::LAlt));
	rb_ary_push(tmp, LONG2NUM(-3)); // X on XBox360 (first joy)
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("Y")), tmp = rb_ary_new());
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(3));
    rb_ary_push(tmp, LONG2NUM(sf::Keyboard::W));
    rb_ary_push(tmp, LONG2NUM(sf::Keyboard::RAlt));
	rb_ary_push(tmp, LONG2NUM(-4)); // Y on XBox360 (first joy)
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("L")), tmp = rb_ary_new());
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(4));
    rb_ary_push(tmp, LONG2NUM(sf::Keyboard::A));
	rb_ary_push(tmp, LONG2NUM(-5)); // L on XBox360 (first joy)
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("R")), tmp = rb_ary_new());
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(5));
    rb_ary_push(tmp, LONG2NUM(sf::Keyboard::E));
	rb_ary_push(tmp, LONG2NUM(-6)); // R on XBox360 (first joy)
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("L2")), tmp = rb_ary_new());
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(6));
    rb_ary_push(tmp, LONG2NUM(sf::Keyboard::Num1));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("R2")), tmp = rb_ary_new());
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(7));
    rb_ary_push(tmp, LONG2NUM(sf::Keyboard::Num3));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("L3")), tmp = rb_ary_new());
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(8));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("R3")), tmp = rb_ary_new());
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(9));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("START")), tmp = rb_ary_new());
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(10));
    rb_ary_push(tmp, LONG2NUM(sf::Keyboard::B));
	rb_ary_push(tmp, LONG2NUM(-8)); // Start on XBox360 (first joy)
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("start")), tmp);
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(10));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("SELECT")), tmp = rb_ary_new());
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(11));
    rb_ary_push(tmp, LONG2NUM(sf::Keyboard::N));
	rb_ary_push(tmp, LONG2NUM(-7)); // Back on XBox360 (first joy)
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("select")), tmp);
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(11));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("HOME")), tmp = rb_ary_new());
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(12));
    rb_ary_push(tmp, LONG2NUM(sf::Keyboard::LControl));
    rb_ary_push(tmp, LONG2NUM(sf::Keyboard::RControl));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("home")), tmp);
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(12));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("UP")), tmp = rb_ary_new());
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(13));
    rb_ary_push(tmp, LONG2NUM(sf::Keyboard::Up));
    rb_ary_push(tmp, LONG2NUM(sf::Keyboard::Z));
    rb_ary_push(tmp, LONG2NUM(sf::Keyboard::Numpad8));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("up")), tmp);
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(13));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("DOWN")), tmp = rb_ary_new());
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(14));
    rb_ary_push(tmp, LONG2NUM(sf::Keyboard::Down));
    rb_ary_push(tmp, LONG2NUM(sf::Keyboard::S));
    rb_ary_push(tmp, LONG2NUM(sf::Keyboard::Numpad2));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("down")), tmp);
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(14));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("LEFT")), tmp = rb_ary_new());
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(15));
    rb_ary_push(tmp, LONG2NUM(sf::Keyboard::Left));
    rb_ary_push(tmp, LONG2NUM(sf::Keyboard::Q));
    rb_ary_push(tmp, LONG2NUM(sf::Keyboard::Numpad4));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("left")), tmp);
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(15));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("RIGHT")), tmp = rb_ary_new());
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(16));
    rb_ary_push(tmp, LONG2NUM(sf::Keyboard::Right));
    rb_ary_push(tmp, LONG2NUM(sf::Keyboard::D));
    rb_ary_push(tmp, LONG2NUM(sf::Keyboard::Numpad6));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("right")), tmp);
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(16));

    rb_mInputMainJoy = LONG2FIX(0);
    rb_define_const(rb_mInput, "JoyAxisX", tmp = LONG2NUM(sf::Joystick::Axis::X));
    rb_mInputMainAxisX = tmp;
    rb_define_const(rb_mInput, "JoyAxisY", tmp = LONG2NUM(sf::Joystick::Axis::Y));
    rb_mInputMainAxisY = tmp;
    rb_define_const(rb_mInput, "JoyAxisZ", LONG2NUM(sf::Joystick::Axis::Z));
    rb_define_const(rb_mInput, "JoyAxisR", LONG2NUM(sf::Joystick::Axis::R));
    rb_define_const(rb_mInput, "JoyAxisU", LONG2NUM(sf::Joystick::Axis::U));
    rb_define_const(rb_mInput, "JoyAxisV", LONG2NUM(sf::Joystick::Axis::V));
    rb_define_const(rb_mInput, "JoyAxisPovX", LONG2NUM(sf::Joystick::Axis::PovX));
    rb_define_const(rb_mInput, "JoyAxisPovY", LONG2NUM(sf::Joystick::Axis::PovY));

    rb_define_const(rb_mKeyboard, "A", LONG2NUM(sf::Keyboard::A));
    rb_define_const(rb_mKeyboard, "B", LONG2NUM(sf::Keyboard::B));
    rb_define_const(rb_mKeyboard, "C", LONG2NUM(sf::Keyboard::C));
    rb_define_const(rb_mKeyboard, "D", LONG2NUM(sf::Keyboard::D));
    rb_define_const(rb_mKeyboard, "E", LONG2NUM(sf::Keyboard::E));
    rb_define_const(rb_mKeyboard, "F", LONG2NUM(sf::Keyboard::F));
    rb_define_const(rb_mKeyboard, "G", LONG2NUM(sf::Keyboard::G));
    rb_define_const(rb_mKeyboard, "H", LONG2NUM(sf::Keyboard::H));
    rb_define_const(rb_mKeyboard, "I", LONG2NUM(sf::Keyboard::I));
    rb_define_const(rb_mKeyboard, "J", LONG2NUM(sf::Keyboard::J));
    rb_define_const(rb_mKeyboard, "K", LONG2NUM(sf::Keyboard::K));
    rb_define_const(rb_mKeyboard, "L", LONG2NUM(sf::Keyboard::L));
    rb_define_const(rb_mKeyboard, "M", LONG2NUM(sf::Keyboard::M));
    rb_define_const(rb_mKeyboard, "N", LONG2NUM(sf::Keyboard::N));
    rb_define_const(rb_mKeyboard, "O", LONG2NUM(sf::Keyboard::O));
    rb_define_const(rb_mKeyboard, "P", LONG2NUM(sf::Keyboard::P));
    rb_define_const(rb_mKeyboard, "Q", LONG2NUM(sf::Keyboard::Q));
    rb_define_const(rb_mKeyboard, "R", LONG2NUM(sf::Keyboard::R));
    rb_define_const(rb_mKeyboard, "S", LONG2NUM(sf::Keyboard::S));
    rb_define_const(rb_mKeyboard, "T", LONG2NUM(sf::Keyboard::T));
    rb_define_const(rb_mKeyboard, "U", LONG2NUM(sf::Keyboard::U));
    rb_define_const(rb_mKeyboard, "V", LONG2NUM(sf::Keyboard::V));
    rb_define_const(rb_mKeyboard, "W", LONG2NUM(sf::Keyboard::W));
    rb_define_const(rb_mKeyboard, "X", LONG2NUM(sf::Keyboard::X));
    rb_define_const(rb_mKeyboard, "Y", LONG2NUM(sf::Keyboard::Y));
    rb_define_const(rb_mKeyboard, "Z", LONG2NUM(sf::Keyboard::Z));
    rb_define_const(rb_mKeyboard, "Num0", LONG2NUM(sf::Keyboard::Num0));
    rb_define_const(rb_mKeyboard, "Num1", LONG2NUM(sf::Keyboard::Num1));
    rb_define_const(rb_mKeyboard, "Num2", LONG2NUM(sf::Keyboard::Num2));
    rb_define_const(rb_mKeyboard, "Num3", LONG2NUM(sf::Keyboard::Num3));
    rb_define_const(rb_mKeyboard, "Num4", LONG2NUM(sf::Keyboard::Num4));
    rb_define_const(rb_mKeyboard, "Num5", LONG2NUM(sf::Keyboard::Num5));
    rb_define_const(rb_mKeyboard, "Num6", LONG2NUM(sf::Keyboard::Num6));
    rb_define_const(rb_mKeyboard, "Num7", LONG2NUM(sf::Keyboard::Num7));
    rb_define_const(rb_mKeyboard, "Num8", LONG2NUM(sf::Keyboard::Num8));
    rb_define_const(rb_mKeyboard, "Num9", LONG2NUM(sf::Keyboard::Num9));
    rb_define_const(rb_mKeyboard, "Escape", LONG2NUM(sf::Keyboard::Escape));
    rb_define_const(rb_mKeyboard, "LControl", LONG2NUM(sf::Keyboard::LControl));
    rb_define_const(rb_mKeyboard, "LShift", LONG2NUM(sf::Keyboard::LShift));
    rb_define_const(rb_mKeyboard, "LAlt", LONG2NUM(sf::Keyboard::LAlt));
    rb_define_const(rb_mKeyboard, "LSystem", LONG2NUM(sf::Keyboard::LSystem));
    rb_define_const(rb_mKeyboard, "RControl", LONG2NUM(sf::Keyboard::RControl));
    rb_define_const(rb_mKeyboard, "RShift", LONG2NUM(sf::Keyboard::RShift));
    rb_define_const(rb_mKeyboard, "RAlt", LONG2NUM(sf::Keyboard::RAlt));
    rb_define_const(rb_mKeyboard, "RSystem", LONG2NUM(sf::Keyboard::RSystem));
    rb_define_const(rb_mKeyboard, "Menu", LONG2NUM(sf::Keyboard::Menu));
    rb_define_const(rb_mKeyboard, "LBracket", LONG2NUM(sf::Keyboard::LBracket));
    rb_define_const(rb_mKeyboard, "RBracket", LONG2NUM(sf::Keyboard::RBracket));
    rb_define_const(rb_mKeyboard, "Semicolon", LONG2NUM(sf::Keyboard::Semicolon));
    rb_define_const(rb_mKeyboard, "Comma", LONG2NUM(sf::Keyboard::Comma));
    rb_define_const(rb_mKeyboard, "Period", LONG2NUM(sf::Keyboard::Period));
    rb_define_const(rb_mKeyboard, "Quote", LONG2NUM(sf::Keyboard::Quote));
    rb_define_const(rb_mKeyboard, "Slash", LONG2NUM(sf::Keyboard::Slash));
    rb_define_const(rb_mKeyboard, "Backslash", LONG2NUM(sf::Keyboard::Backslash));
    rb_define_const(rb_mKeyboard, "Tilde", LONG2NUM(sf::Keyboard::Tilde));
    rb_define_const(rb_mKeyboard, "Equal", LONG2NUM(sf::Keyboard::Equal));
    rb_define_const(rb_mKeyboard, "Hyphen", LONG2NUM(sf::Keyboard::Hyphen));
    rb_define_const(rb_mKeyboard, "Space", LONG2NUM(sf::Keyboard::Space));
    rb_define_const(rb_mKeyboard, "Enter", LONG2NUM(sf::Keyboard::Enter));
    rb_define_const(rb_mKeyboard, "Backspace", LONG2NUM(sf::Keyboard::Backspace));
    rb_define_const(rb_mKeyboard, "Tab", LONG2NUM(sf::Keyboard::Tab));
    rb_define_const(rb_mKeyboard, "PageUp", LONG2NUM(sf::Keyboard::PageUp));
    rb_define_const(rb_mKeyboard, "PageDown", LONG2NUM(sf::Keyboard::PageDown));
    rb_define_const(rb_mKeyboard, "End", LONG2NUM(sf::Keyboard::End));
    rb_define_const(rb_mKeyboard, "Home", LONG2NUM(sf::Keyboard::Home));
    rb_define_const(rb_mKeyboard, "Insert", LONG2NUM(sf::Keyboard::Insert));
    rb_define_const(rb_mKeyboard, "Delete", LONG2NUM(sf::Keyboard::Delete));
    rb_define_const(rb_mKeyboard, "Add", LONG2NUM(sf::Keyboard::Add));
    rb_define_const(rb_mKeyboard, "Subtract", LONG2NUM(sf::Keyboard::Subtract));
    rb_define_const(rb_mKeyboard, "Multiply", LONG2NUM(sf::Keyboard::Multiply));
    rb_define_const(rb_mKeyboard, "Divide", LONG2NUM(sf::Keyboard::Divide));
    rb_define_const(rb_mKeyboard, "Left", LONG2NUM(sf::Keyboard::Left));
    rb_define_const(rb_mKeyboard, "Right", LONG2NUM(sf::Keyboard::Right));
    rb_define_const(rb_mKeyboard, "Up", LONG2NUM(sf::Keyboard::Up));
    rb_define_const(rb_mKeyboard, "Down", LONG2NUM(sf::Keyboard::Down));
    rb_define_const(rb_mKeyboard, "Numpad0", LONG2NUM(sf::Keyboard::Numpad0));
    rb_define_const(rb_mKeyboard, "Numpad1", LONG2NUM(sf::Keyboard::Numpad1));
    rb_define_const(rb_mKeyboard, "Numpad2", LONG2NUM(sf::Keyboard::Numpad2));
    rb_define_const(rb_mKeyboard, "Numpad3", LONG2NUM(sf::Keyboard::Numpad3));
    rb_define_const(rb_mKeyboard, "Numpad4", LONG2NUM(sf::Keyboard::Numpad4));
    rb_define_const(rb_mKeyboard, "Numpad5", LONG2NUM(sf::Keyboard::Numpad5));
    rb_define_const(rb_mKeyboard, "Numpad6", LONG2NUM(sf::Keyboard::Numpad6));
    rb_define_const(rb_mKeyboard, "Numpad7", LONG2NUM(sf::Keyboard::Numpad7));
    rb_define_const(rb_mKeyboard, "Numpad8", LONG2NUM(sf::Keyboard::Numpad8));
    rb_define_const(rb_mKeyboard, "Numpad9", LONG2NUM(sf::Keyboard::Numpad9));
    rb_define_const(rb_mKeyboard, "F1", LONG2NUM(sf::Keyboard::F1));
    rb_define_const(rb_mKeyboard, "F2", LONG2NUM(sf::Keyboard::F2));
    rb_define_const(rb_mKeyboard, "F3", LONG2NUM(sf::Keyboard::F3));
    rb_define_const(rb_mKeyboard, "F4", LONG2NUM(sf::Keyboard::F4));
    rb_define_const(rb_mKeyboard, "F5", LONG2NUM(sf::Keyboard::F5));
    rb_define_const(rb_mKeyboard, "F6", LONG2NUM(sf::Keyboard::F6));
    rb_define_const(rb_mKeyboard, "F7", LONG2NUM(sf::Keyboard::F7));
    rb_define_const(rb_mKeyboard, "F8", LONG2NUM(sf::Keyboard::F8));
    rb_define_const(rb_mKeyboard, "F9", LONG2NUM(sf::Keyboard::F9));
    rb_define_const(rb_mKeyboard, "F10", LONG2NUM(sf::Keyboard::F10));
    rb_define_const(rb_mKeyboard, "F11", LONG2NUM(sf::Keyboard::F11));
    rb_define_const(rb_mKeyboard, "F12", LONG2NUM(sf::Keyboard::F12));
    rb_define_const(rb_mKeyboard, "F13", LONG2NUM(sf::Keyboard::F13));
    rb_define_const(rb_mKeyboard, "F14", LONG2NUM(sf::Keyboard::F14));
    rb_define_const(rb_mKeyboard, "F15", LONG2NUM(sf::Keyboard::F15));
    rb_define_const(rb_mKeyboard, "Pause", LONG2NUM(sf::Keyboard::Pause));
}

/* Reset the clock of the Input System */
void L_Input_Reset_Clocks()
{
    unsigned int i;
    for(i = 0; i < L_INPUT_NUM_INGAME_KEY; i++)
    {
		L_TRIGGER_RESET_Input(i);
        L_Input_Press[i] = false;
    }
    for(i = 0;i < L_INPUT_NUM_MOUSE_KEY; i++)
    {
		L_TRIGGER_RESET_Mouse(i);
        L_Mouse_Press[i] = false;
    }
}

unsigned int L_Input_Get_Key_Position(VALUE hash_ids, VALUE hash_key)
{
    return rb_num2ulong(rb_hash_aref(hash_ids, hash_key));
}

/* Blocks that update the press key state */
VALUE rb_Input_KeyStateDown_Block(VALUE hash_key, VALUE id_key, int argc, VALUE* argv)
{
    VALUE array = rb_hash_aref(rb_mInputKey, hash_key);
    if(rb_ary_includes(array, id_key) == Qtrue)
    {
        unsigned int pos = L_Input_Get_Key_Position(rb_mInputKeyID, hash_key);
        if(!L_Input_Press[pos])
        {
            L_Input_Press[pos] = true;
			L_TRIGGER_RESET_Input(pos);
        }
    }
    return Qnil;
}

VALUE rb_Input_KeyStateUp_Block(VALUE hash_key, VALUE id_key, int argc, VALUE* argv)
{
    VALUE array = rb_hash_aref(rb_mInputKey, hash_key);
    if(rb_ary_includes(array, id_key) == Qtrue)
    {
        unsigned int pos = L_Input_Get_Key_Position(rb_mInputKeyID, hash_key);
        if(L_Input_Press[pos])
        {
            L_Input_Press[pos] = false;
			L_TRIGGER_RESET_Input(pos);
        }
    }
    return Qnil;
}

/* Update the key state when an event has been triggered */
void L_Input_Update_Key(sf::Keyboard::Key code, bool state)
{
    rb_block_call(rb_mInputKey, rb_mInputEach, 0, NULL, 
        (rb_block_call_func_t)( state ? rb_Input_KeyStateDown_Block : rb_Input_KeyStateUp_Block),
        LONG2NUM(code));
}

void L_Input_Mouse_Pos_Update(int x, int y)
{
	if (x < 0)
		x = -256;
    L_Mouse_Pos_X = static_cast<double>(x);
    L_Mouse_Pos_Y = static_cast<double>(y);
}

void L_Input_Mouse_Wheel_Update(long delta)
{
    L_Mouse_Wheel_Delta += delta;
}

void L_Input_Mouse_Button_Update(long button, bool state)
{
    if(button >= 0 && button < L_INPUT_NUM_MOUSE_KEY)
    {
        if(L_Mouse_Press[button] != state)
        {
            L_Mouse_Press[button] = state;
			L_TRIGGER_RESET_Mouse(button);
        }
    }
}

void L_Input_Update_Joy(unsigned int joy_id, unsigned int key, bool state)
{
	if (!sf::Joystick::isConnected(joy_id))
		return;
    rb_block_call(rb_mInputKey, rb_mInputEach, 0, NULL, 
        (rb_block_call_func_t)( state ? rb_Input_KeyStateDown_Block : rb_Input_KeyStateUp_Block),
        LONG2NUM(-(32 * joy_id) - key - 1));
}

void L_Input_Update_JoyPos_setKeyState(unsigned int key, bool state)
{
	L_Input_Press[key] = state;
	L_TRIGGER_RESET_Input(key);
}

#define JOY_MIN_DEADZONE -25.0f
#define JOY_MAX_DEADZONE 25.0f

void L_Input_Update_JoyXPos(float position)
{
	if (rb_mInputInvertAxisX != Qfalse)
		position = -position;
	if (position < JOY_MIN_DEADZONE)
	{
		if (!L_Input_Press[15])
		{
			L_Input_Update_JoyPos_setKeyState(15, true);
			L_Input_Update_JoyPos_setKeyState(16, false);
		}
	}
	else if (position > JOY_MAX_DEADZONE)
	{
		if (!L_Input_Press[16])
		{
			L_Input_Update_JoyPos_setKeyState(15, false);
			L_Input_Update_JoyPos_setKeyState(16, true);
		}
	}
	else
	{
		if (L_Input_Press[16])
			L_Input_Update_JoyPos_setKeyState(16, false);
		if (L_Input_Press[15])
			L_Input_Update_JoyPos_setKeyState(15, false);
	}
}

void L_Input_Update_JoyYPos(float position)
{
	if (rb_mInputInvertAxisY != Qfalse)
		position = -position;
	if (position < JOY_MIN_DEADZONE)
	{
		if (!L_Input_Press[13])
		{
			L_Input_Update_JoyPos_setKeyState(13, true);
			L_Input_Update_JoyPos_setKeyState(14, false);
		}
	}
	else if (position > JOY_MAX_DEADZONE)
	{
		if (!L_Input_Press[14])
		{
			L_Input_Update_JoyPos_setKeyState(13, false);
			L_Input_Update_JoyPos_setKeyState(14, true);
		}
	}
	else
	{
		if (L_Input_Press[13])
			L_Input_Update_JoyPos_setKeyState(13, false);
		if (L_Input_Press[14])
			L_Input_Update_JoyPos_setKeyState(14, false);
	}
}

void L_Input_Update_JoyPos(unsigned int joy_id, long axis, float position)
{
    VALUE joypad = LONG2FIX(joy_id);
    if(joypad != rb_mInputMainJoy)
        return;
    VALUE raxis = LONG2FIX(axis);
	//printf("\r%d %d %f %d %d      ", joy_id, raxis, position, raxis == rb_mInputMainAxisX, raxis == rb_mInputMainAxisY);
	if (raxis == rb_mInputMainAxisX)
		L_Input_Update_JoyXPos(position);
	else if (raxis == rb_mInputMainAxisY)
		L_Input_Update_JoyYPos(position);
}

void L_Input_Reset_JoyPos(unsigned int joy_id)
{
	L_Input_Update_JoyPos(joy_id, LONG2FIX(rb_mInputMainAxisX), 0.0f);
	L_Input_Update_JoyPos(joy_id, LONG2FIX(rb_mInputMainAxisY), 0.0f);
}

void L_Input_Setusec_threshold(long usec)
{
    if(usec > 1)
        L_Input_threshold = usec - 100;
    else
        L_Input_threshold = 1;
}


VALUE rb_Input_Press(VALUE self, VALUE key_sym)
{
    unsigned int pos = L_Input_Get_Key_Position(rb_mInputKeyID, key_sym);
    return L_Input_Press[pos] ? Qtrue : Qfalse;
}

VALUE rb_Input_Trigger(VALUE self, VALUE key_sym)
{
    unsigned int pos = L_Input_Get_Key_Position(rb_mInputKeyID, key_sym);
    bool pressed = L_Input_Press[pos];
    if(!pressed)
        return Qfalse;
    if(L_Trigger_Input(pos))
        return Qtrue;
    return Qfalse;
}

VALUE rb_Input_Repeat(VALUE self, VALUE key_sym)
{
    unsigned int pos = L_Input_Get_Key_Position(rb_mInputKeyID, key_sym);
    bool pressed = L_Input_Press[pos];
    if(!pressed)
        return Qfalse;
    /* Trigger */
    if(L_Trigger_Input(pos))
        return Qtrue;
	/* Repeat */
	L_Repeat_Input(pos)
    return Qfalse;
}

VALUE rb_Input_Released(VALUE self, VALUE key_sym)
{
    unsigned int pos = L_Input_Get_Key_Position(rb_mInputKeyID, key_sym);
    bool pressed = L_Input_Press[pos];
    if(pressed)
        return Qfalse;
    if(L_Trigger_Input(pos))
        return Qtrue;
    return Qfalse;
}


VALUE rb_Input_dir4(VALUE self)
{
    if(L_Input_Press[13])
        return LONG2FIX(8);
    if(L_Input_Press[14])
        return LONG2FIX(2);
    if(L_Input_Press[15])
        return LONG2FIX(4);
    if(L_Input_Press[16])
        return LONG2FIX(6);
    return LONG2FIX(0);
}

VALUE rb_Input_dir8(VALUE self)
{
    // Up Left / Up Right / Up
    if(L_Input_Press[13])
    {
        if(L_Input_Press[15])
            return LONG2FIX(7);
        else if(L_Input_Press[16])
            return LONG2FIX(9);
        return LONG2FIX(8);
    }
    // Down Left / Down Right / Down
    if(L_Input_Press[14])
    {
        if(L_Input_Press[15])
            return LONG2FIX(1);
        else if(L_Input_Press[16])
            return LONG2FIX(3);
        return LONG2FIX(2);
    }
    if(L_Input_Press[15])
        return LONG2FIX(4);
    if(L_Input_Press[16])
        return LONG2FIX(6);
    return LONG2FIX(0);
}

VALUE rb_Input_setMainJoypad(VALUE self, VALUE id)
{
    rb_check_type(id, T_FIXNUM);
    rb_mInputMainJoy = id;
    return id;
}

VALUE rb_Input_setMainXAxis(VALUE self, VALUE axis)
{
    rb_check_type(axis, T_FIXNUM);
    rb_mInputMainAxisX = axis;
    return axis;
}

VALUE rb_Input_setMainYAxis(VALUE self, VALUE axis)
{
    rb_check_type(axis, T_FIXNUM);
    rb_mInputMainAxisY = axis;
    return axis;
}

VALUE rb_Input_getMainJoypad(VALUE self)
{
    return rb_mInputMainJoy;
}

VALUE rb_Input_getMainXAxis(VALUE self)
{
    return rb_mInputMainAxisX;
}

VALUE rb_Input_getMainYAxis(VALUE self)
{
    return rb_mInputMainAxisY;
}

VALUE rb_Input_getText(VALUE self)
{
	if (L_EnteredText.size() > 0)
		return rb_utf8_str_new_cstr(L_EnteredText.c_str());
	return Qnil;
}

VALUE rb_Input_getInvertAxisX(VALUE self)
{
	return rb_mInputInvertAxisX;
}

VALUE rb_Input_setInvertAxisX(VALUE self, VALUE val)
{
	rb_mInputInvertAxisX = RTEST(val) ? Qtrue : Qfalse;
	return self;
}

VALUE rb_Input_getInvertAxisY(VALUE self)
{
	return rb_mInputInvertAxisY;
}

VALUE rb_Input_setInvertAxisY(VALUE self, VALUE val)
{
	rb_mInputInvertAxisY = RTEST(val) ? Qtrue : Qfalse;
	return self;
}

VALUE rb_Input_JoyConnected(VALUE self, VALUE id)
{
	return sf::Joystick::isConnected(NUM2UINT(id)) ? Qtrue : Qfalse;
}

VALUE rb_Input_JoyGetButtonCount(VALUE self, VALUE id)
{
	return UINT2NUM(sf::Joystick::getButtonCount(NUM2UINT(id)));
}

VALUE rb_Input_JoyHasAxis(VALUE self, VALUE id, VALUE axis)
{
	return sf::Joystick::hasAxis(NUM2UINT(id), static_cast<sf::Joystick::Axis>(NUM2LONG(axis))) ? Qtrue : Qfalse;
}

VALUE rb_Input_JoyIsButtonPressed(VALUE self, VALUE id, VALUE button)
{
	return sf::Joystick::isButtonPressed(NUM2UINT(id), NUM2UINT(button)) ? Qtrue : Qfalse;
}

VALUE rb_Input_JoyGetAxisPosition(VALUE self, VALUE id, VALUE axis)
{
	return LONG2NUM(static_cast<long>(sf::Joystick::getAxisPosition(NUM2UINT(id), static_cast<sf::Joystick::Axis>(NUM2LONG(axis)))));
}

VALUE rb_Input_JoyGetName(VALUE self, VALUE id)
{
	sf::String name = sf::Joystick::getIdentification(NUM2LONG(id)).name;
	return rb_utf8_str_new_cstr(reinterpret_cast<const char*>(name.toUtf8().c_str()));
}

VALUE rb_Mouse_Press(VALUE self, VALUE key_sym)
{
    unsigned int pos = L_Input_Get_Key_Position(rb_mMouseKeyID, key_sym);
    return L_Mouse_Press[pos] ? Qtrue : Qfalse;
}

VALUE rb_Mouse_Trigger(VALUE self, VALUE key_sym)
{
    unsigned int pos = L_Input_Get_Key_Position(rb_mMouseKeyID, key_sym);
    bool pressed = L_Mouse_Press[pos];
    if(!pressed)
        return Qfalse;
    if(L_Trigger_Mouse(pos))
        return Qtrue;
    return Qfalse;
}

VALUE rb_Mouse_Released(VALUE self, VALUE key_sym)
{
    unsigned int pos = L_Input_Get_Key_Position(rb_mMouseKeyID, key_sym);
    bool pressed = L_Mouse_Press[pos];
    if(pressed)
        return Qfalse;
    if(L_Trigger_Mouse(pos))
        return Qtrue;
    return Qfalse;
}

VALUE rb_Mouse_x(VALUE self)
{
    return LONG2NUM(static_cast<long>(L_Mouse_Pos_X / Graphics_Scale));
}

VALUE rb_Mouse_y(VALUE self)
{
    return LONG2NUM(static_cast<long>(L_Mouse_Pos_Y / Graphics_Scale));
}

VALUE rb_Mouse_Wheel(VALUE self)
{
    return LONG2NUM(L_Mouse_Wheel_Delta);
}

VALUE rb_Mouse_Wheel_set(VALUE self, VALUE val)
{
    L_Mouse_Wheel_Delta = NUM2LONG(val);
    return val;
}

VALUE rb_KeyBoard_Press(VALUE self, VALUE val)
{
    return sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(rb_num2long(val))) ? Qtrue : Qfalse;
}