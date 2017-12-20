#include "ruby.h"
#include "ruby/ruby.h"
#include "LiteRGSS.h"
#include <SFML/Graphics.hpp>
#include "Input.h"

#define L_INPUT_NUM_INGAME_KEY 17
#define L_INPUT_NUM_MOUSE_KEY 5

VALUE rb_mInput = Qnil;
VALUE rb_mMouse = Qnil;
VALUE rb_mMouseKey = Qnil;
VALUE rb_mInputKey = Qnil;
VALUE rb_mMouseKeyID = Qnil;
VALUE rb_mInputKeyID = Qnil;
VALUE rb_mInputMainJoy = Qnil;
VALUE rb_mInputMainAxisX = Qnil;
VALUE rb_mInputMainAxisY = Qnil;
ID rb_mInputEach = Qnil;
/* Clocks used to detect trigger, and repeat */
sf::Clock L_Input_Clock[L_INPUT_NUM_INGAME_KEY];
sf::Clock L_Mouse_Clock[L_INPUT_NUM_MOUSE_KEY];
bool L_Input_Press[L_INPUT_NUM_INGAME_KEY];
bool L_Mouse_Press[L_INPUT_NUM_MOUSE_KEY];
long L_Input_threshold = 0;
long L_Mouse_Wheel_Delta = 0;
long L_Mouse_Pos_X = 0;
long L_Mouse_Pos_Y = 0;

unsigned int L_Input_Get_Key_Position(VALUE hash_ids, VALUE hash_key);

void Init_Input()
{
    rb_mInput = rb_define_module_under(rb_mLiteRGSS, "Input");
    rb_mMouse = rb_define_module_under(rb_mLiteRGSS, "Mouse");
    VALUE rb_mKeyboard = rb_define_module_under(rb_mInput, "Keyboard");
    /* function definition */
    rb_define_module_function(rb_mInput, "press?", _rbf rb_Input_Press, 1);
    rb_define_module_function(rb_mInput, "trigger?", _rbf rb_Input_Trigger, 1);
    rb_define_module_function(rb_mInput, "repeat?", _rbf rb_Input_Repeat, 1);
    rb_define_module_function(rb_mInput, "released?", _rbf rb_Input_Released, 1);
    rb_define_module_function(rb_mInput, "dir4", _rbf rb_Input_dir4, 0);
    rb_define_module_function(rb_mInput, "dir8", _rbf rb_Input_dir8, 0);

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
    RHASH_SET_IFNONE(rb_mMouseKey, tmp = rb_int2inum(sf::Mouse::Button::Left));
    RHASH_SET_IFNONE(rb_mMouseKeyID, LONG2FIX(0));
    rb_hash_aset(rb_mMouseKey, key = rb_id2sym(rb_intern("LEFT")), tmp);
    rb_hash_aset(rb_mMouseKeyID, key, LONG2FIX(0));
    rb_hash_aset(rb_mMouseKey, key = rb_id2sym(rb_intern("left")), tmp);
    rb_hash_aset(rb_mMouseKeyID, key, LONG2FIX(0));
    rb_hash_aset(rb_mMouseKey, key = rb_id2sym(rb_intern("RIGHT")), tmp = rb_int2inum(sf::Mouse::Button::Right));
    rb_hash_aset(rb_mMouseKeyID, key, LONG2FIX(1));
    rb_hash_aset(rb_mMouseKey, key = rb_id2sym(rb_intern("right")), tmp);
    rb_hash_aset(rb_mMouseKeyID, key, LONG2FIX(1));
    rb_hash_aset(rb_mMouseKey, key = rb_id2sym(rb_intern("MIDDLE")), tmp = rb_int2inum(sf::Mouse::Button::Middle));
    rb_hash_aset(rb_mMouseKeyID, key, LONG2FIX(2));
    rb_hash_aset(rb_mMouseKey, key = rb_id2sym(rb_intern("middle")), tmp);
    rb_hash_aset(rb_mMouseKeyID, key, LONG2FIX(2));
    rb_hash_aset(rb_mMouseKey, key = rb_id2sym(rb_intern("X1")), tmp = rb_int2inum(sf::Mouse::Button::XButton1));
    rb_hash_aset(rb_mMouseKeyID, key, LONG2FIX(3));
    rb_hash_aset(rb_mMouseKey, key = rb_id2sym(rb_intern("x1")), tmp);
    rb_hash_aset(rb_mMouseKeyID, key, LONG2FIX(3));
    rb_hash_aset(rb_mMouseKey, key = rb_id2sym(rb_intern("X2")), tmp = rb_int2inum(sf::Mouse::Button::XButton2));
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
    rb_ary_push(tmp, rb_int2inum(sf::Keyboard::C));
    rb_ary_push(tmp, rb_int2inum(sf::Keyboard::Return));
    rb_ary_push(tmp, rb_int2inum(sf::Keyboard::Space));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("B")), tmp = rb_ary_new());
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(1));
    rb_ary_push(tmp, rb_int2inum(sf::Keyboard::X));
    rb_ary_push(tmp, rb_int2inum(sf::Keyboard::Escape));
    rb_ary_push(tmp, rb_int2inum(sf::Keyboard::LShift));
    rb_ary_push(tmp, rb_int2inum(sf::Keyboard::RShift));
    rb_ary_push(tmp, rb_int2inum(sf::Keyboard::BackSpace));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("X")), tmp = rb_ary_new());
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(2));
    rb_ary_push(tmp, rb_int2inum(sf::Keyboard::V));
    rb_ary_push(tmp, rb_int2inum(sf::Keyboard::LAlt));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("Y")), tmp = rb_ary_new());
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(3));
    rb_ary_push(tmp, rb_int2inum(sf::Keyboard::W));
    rb_ary_push(tmp, rb_int2inum(sf::Keyboard::RAlt));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("L")), tmp = rb_ary_new());
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(4));
    rb_ary_push(tmp, rb_int2inum(sf::Keyboard::A));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("R")), tmp = rb_ary_new());
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(5));
    rb_ary_push(tmp, rb_int2inum(sf::Keyboard::E));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("L2")), tmp = rb_ary_new());
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(6));
    rb_ary_push(tmp, rb_int2inum(sf::Keyboard::Num1));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("R2")), tmp = rb_ary_new());
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(7));
    rb_ary_push(tmp, rb_int2inum(sf::Keyboard::Num3));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("L3")), tmp = rb_ary_new());
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(8));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("R3")), tmp = rb_ary_new());
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(9));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("START")), tmp = rb_ary_new());
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(10));
    rb_ary_push(tmp, rb_int2inum(sf::Keyboard::B));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("start")), tmp);
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(10));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("SELECT")), tmp = rb_ary_new());
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(11));
    rb_ary_push(tmp, rb_int2inum(sf::Keyboard::N));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("select")), tmp);
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(11));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("HOME")), tmp = rb_ary_new());
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(12));
    rb_ary_push(tmp, rb_int2inum(sf::Keyboard::LControl));
    rb_ary_push(tmp, rb_int2inum(sf::Keyboard::RControl));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("home")), tmp);
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(12));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("UP")), tmp = rb_ary_new());
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(13));
    rb_ary_push(tmp, rb_int2inum(sf::Keyboard::Up));
    rb_ary_push(tmp, rb_int2inum(sf::Keyboard::Z));
    rb_ary_push(tmp, rb_int2inum(sf::Keyboard::Numpad8));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("up")), tmp);
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(13));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("DOWN")), tmp = rb_ary_new());
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(14));
    rb_ary_push(tmp, rb_int2inum(sf::Keyboard::Down));
    rb_ary_push(tmp, rb_int2inum(sf::Keyboard::S));
    rb_ary_push(tmp, rb_int2inum(sf::Keyboard::Numpad2));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("down")), tmp);
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(14));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("LEFT")), tmp = rb_ary_new());
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(15));
    rb_ary_push(tmp, rb_int2inum(sf::Keyboard::Left));
    rb_ary_push(tmp, rb_int2inum(sf::Keyboard::Q));
    rb_ary_push(tmp, rb_int2inum(sf::Keyboard::Numpad4));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("left")), tmp);
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(15));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("RIGHT")), tmp = rb_ary_new());
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(16));
    rb_ary_push(tmp, rb_int2inum(sf::Keyboard::Right));
    rb_ary_push(tmp, rb_int2inum(sf::Keyboard::D));
    rb_ary_push(tmp, rb_int2inum(sf::Keyboard::Numpad6));
    rb_hash_aset(rb_mInputKey, key = rb_id2sym(rb_intern("right")), tmp);
    rb_hash_aset(rb_mInputKeyID, key, LONG2FIX(16));

    rb_mInputMainJoy = LONG2FIX(0);
    rb_define_const(rb_mInput, "JoyAxisX", tmp = rb_int2inum(sf::Joystick::Axis::X));
    rb_mInputMainAxisX = tmp;
    rb_define_const(rb_mInput, "JoyAxisY", rb_int2inum(sf::Joystick::Axis::Y));
    rb_mInputMainAxisY = tmp;
    rb_define_const(rb_mInput, "JoyAxisZ", rb_int2inum(sf::Joystick::Axis::Z));
    rb_define_const(rb_mInput, "JoyAxisU", rb_int2inum(sf::Joystick::Axis::U));
    rb_define_const(rb_mInput, "JoyAxisV", rb_int2inum(sf::Joystick::Axis::V));
    rb_define_const(rb_mInput, "JoyAxisPovX", rb_int2inum(sf::Joystick::Axis::PovX));
    rb_define_const(rb_mInput, "JoyAxisPovY", rb_int2inum(sf::Joystick::Axis::PovY));

    rb_define_const(rb_mKeyboard, "A", rb_int2inum(sf::Keyboard::A));
    rb_define_const(rb_mKeyboard, "B", rb_int2inum(sf::Keyboard::B));
    rb_define_const(rb_mKeyboard, "C", rb_int2inum(sf::Keyboard::C));
    rb_define_const(rb_mKeyboard, "D", rb_int2inum(sf::Keyboard::D));
    rb_define_const(rb_mKeyboard, "E", rb_int2inum(sf::Keyboard::E));
    rb_define_const(rb_mKeyboard, "F", rb_int2inum(sf::Keyboard::F));
    rb_define_const(rb_mKeyboard, "G", rb_int2inum(sf::Keyboard::G));
    rb_define_const(rb_mKeyboard, "H", rb_int2inum(sf::Keyboard::H));
    rb_define_const(rb_mKeyboard, "I", rb_int2inum(sf::Keyboard::I));
    rb_define_const(rb_mKeyboard, "J", rb_int2inum(sf::Keyboard::J));
    rb_define_const(rb_mKeyboard, "K", rb_int2inum(sf::Keyboard::K));
    rb_define_const(rb_mKeyboard, "L", rb_int2inum(sf::Keyboard::L));
    rb_define_const(rb_mKeyboard, "M", rb_int2inum(sf::Keyboard::M));
    rb_define_const(rb_mKeyboard, "N", rb_int2inum(sf::Keyboard::N));
    rb_define_const(rb_mKeyboard, "O", rb_int2inum(sf::Keyboard::O));
    rb_define_const(rb_mKeyboard, "P", rb_int2inum(sf::Keyboard::P));
    rb_define_const(rb_mKeyboard, "Q", rb_int2inum(sf::Keyboard::Q));
    rb_define_const(rb_mKeyboard, "R", rb_int2inum(sf::Keyboard::R));
    rb_define_const(rb_mKeyboard, "S", rb_int2inum(sf::Keyboard::S));
    rb_define_const(rb_mKeyboard, "T", rb_int2inum(sf::Keyboard::T));
    rb_define_const(rb_mKeyboard, "U", rb_int2inum(sf::Keyboard::U));
    rb_define_const(rb_mKeyboard, "V", rb_int2inum(sf::Keyboard::V));
    rb_define_const(rb_mKeyboard, "W", rb_int2inum(sf::Keyboard::W));
    rb_define_const(rb_mKeyboard, "X", rb_int2inum(sf::Keyboard::X));
    rb_define_const(rb_mKeyboard, "Y", rb_int2inum(sf::Keyboard::Y));
    rb_define_const(rb_mKeyboard, "Z", rb_int2inum(sf::Keyboard::Z));
    rb_define_const(rb_mKeyboard, "Num0", rb_int2inum(sf::Keyboard::Num0));
    rb_define_const(rb_mKeyboard, "Num1", rb_int2inum(sf::Keyboard::Num1));
    rb_define_const(rb_mKeyboard, "Num2", rb_int2inum(sf::Keyboard::Num2));
    rb_define_const(rb_mKeyboard, "Num3", rb_int2inum(sf::Keyboard::Num3));
    rb_define_const(rb_mKeyboard, "Num4", rb_int2inum(sf::Keyboard::Num4));
    rb_define_const(rb_mKeyboard, "Num5", rb_int2inum(sf::Keyboard::Num5));
    rb_define_const(rb_mKeyboard, "Num6", rb_int2inum(sf::Keyboard::Num6));
    rb_define_const(rb_mKeyboard, "Num7", rb_int2inum(sf::Keyboard::Num7));
    rb_define_const(rb_mKeyboard, "Num8", rb_int2inum(sf::Keyboard::Num8));
    rb_define_const(rb_mKeyboard, "Num9", rb_int2inum(sf::Keyboard::Num9));
    rb_define_const(rb_mKeyboard, "Escape", rb_int2inum(sf::Keyboard::Escape));
    rb_define_const(rb_mKeyboard, "LControl", rb_int2inum(sf::Keyboard::LControl));
    rb_define_const(rb_mKeyboard, "LShift", rb_int2inum(sf::Keyboard::LShift));
    rb_define_const(rb_mKeyboard, "LAlt", rb_int2inum(sf::Keyboard::LAlt));
    rb_define_const(rb_mKeyboard, "LSystem", rb_int2inum(sf::Keyboard::LSystem));
    rb_define_const(rb_mKeyboard, "RControl", rb_int2inum(sf::Keyboard::RControl));
    rb_define_const(rb_mKeyboard, "RShift", rb_int2inum(sf::Keyboard::RShift));
    rb_define_const(rb_mKeyboard, "RAlt", rb_int2inum(sf::Keyboard::RAlt));
    rb_define_const(rb_mKeyboard, "RSystem", rb_int2inum(sf::Keyboard::RSystem));
    rb_define_const(rb_mKeyboard, "Menu", rb_int2inum(sf::Keyboard::Menu));
    rb_define_const(rb_mKeyboard, "LBracket", rb_int2inum(sf::Keyboard::LBracket));
    rb_define_const(rb_mKeyboard, "RBracket", rb_int2inum(sf::Keyboard::RBracket));
    rb_define_const(rb_mKeyboard, "SemiColon", rb_int2inum(sf::Keyboard::SemiColon));
    rb_define_const(rb_mKeyboard, "Comma", rb_int2inum(sf::Keyboard::Comma));
    rb_define_const(rb_mKeyboard, "Period", rb_int2inum(sf::Keyboard::Period));
    rb_define_const(rb_mKeyboard, "Quote", rb_int2inum(sf::Keyboard::Quote));
    rb_define_const(rb_mKeyboard, "Slash", rb_int2inum(sf::Keyboard::Slash));
    rb_define_const(rb_mKeyboard, "BackSlash", rb_int2inum(sf::Keyboard::BackSlash));
    rb_define_const(rb_mKeyboard, "Tilde", rb_int2inum(sf::Keyboard::Tilde));
    rb_define_const(rb_mKeyboard, "Equal", rb_int2inum(sf::Keyboard::Equal));
    rb_define_const(rb_mKeyboard, "Dash", rb_int2inum(sf::Keyboard::Dash));
    rb_define_const(rb_mKeyboard, "Space", rb_int2inum(sf::Keyboard::Space));
    rb_define_const(rb_mKeyboard, "Return", rb_int2inum(sf::Keyboard::Return));
    rb_define_const(rb_mKeyboard, "BackSpace", rb_int2inum(sf::Keyboard::BackSpace));
    rb_define_const(rb_mKeyboard, "Tab", rb_int2inum(sf::Keyboard::Tab));
    rb_define_const(rb_mKeyboard, "PageUp", rb_int2inum(sf::Keyboard::PageUp));
    rb_define_const(rb_mKeyboard, "PageDown", rb_int2inum(sf::Keyboard::PageDown));
    rb_define_const(rb_mKeyboard, "End", rb_int2inum(sf::Keyboard::End));
    rb_define_const(rb_mKeyboard, "Home", rb_int2inum(sf::Keyboard::Home));
    rb_define_const(rb_mKeyboard, "Insert", rb_int2inum(sf::Keyboard::Insert));
    rb_define_const(rb_mKeyboard, "Delete", rb_int2inum(sf::Keyboard::Delete));
    rb_define_const(rb_mKeyboard, "Add", rb_int2inum(sf::Keyboard::Add));
    rb_define_const(rb_mKeyboard, "Subtract", rb_int2inum(sf::Keyboard::Subtract));
    rb_define_const(rb_mKeyboard, "Multiply", rb_int2inum(sf::Keyboard::Multiply));
    rb_define_const(rb_mKeyboard, "Divide", rb_int2inum(sf::Keyboard::Divide));
    rb_define_const(rb_mKeyboard, "Left", rb_int2inum(sf::Keyboard::Left));
    rb_define_const(rb_mKeyboard, "Right", rb_int2inum(sf::Keyboard::Right));
    rb_define_const(rb_mKeyboard, "Up", rb_int2inum(sf::Keyboard::Up));
    rb_define_const(rb_mKeyboard, "Down", rb_int2inum(sf::Keyboard::Down));
    rb_define_const(rb_mKeyboard, "Numpad0", rb_int2inum(sf::Keyboard::Numpad0));
    rb_define_const(rb_mKeyboard, "Numpad1", rb_int2inum(sf::Keyboard::Numpad1));
    rb_define_const(rb_mKeyboard, "Numpad2", rb_int2inum(sf::Keyboard::Numpad2));
    rb_define_const(rb_mKeyboard, "Numpad3", rb_int2inum(sf::Keyboard::Numpad3));
    rb_define_const(rb_mKeyboard, "Numpad4", rb_int2inum(sf::Keyboard::Numpad4));
    rb_define_const(rb_mKeyboard, "Numpad5", rb_int2inum(sf::Keyboard::Numpad5));
    rb_define_const(rb_mKeyboard, "Numpad6", rb_int2inum(sf::Keyboard::Numpad6));
    rb_define_const(rb_mKeyboard, "Numpad7", rb_int2inum(sf::Keyboard::Numpad7));
    rb_define_const(rb_mKeyboard, "Numpad8", rb_int2inum(sf::Keyboard::Numpad8));
    rb_define_const(rb_mKeyboard, "Numpad9", rb_int2inum(sf::Keyboard::Numpad9));
    rb_define_const(rb_mKeyboard, "F1", rb_int2inum(sf::Keyboard::F1));
    rb_define_const(rb_mKeyboard, "F2", rb_int2inum(sf::Keyboard::F2));
    rb_define_const(rb_mKeyboard, "F3", rb_int2inum(sf::Keyboard::F3));
    rb_define_const(rb_mKeyboard, "F4", rb_int2inum(sf::Keyboard::F4));
    rb_define_const(rb_mKeyboard, "F5", rb_int2inum(sf::Keyboard::F5));
    rb_define_const(rb_mKeyboard, "F6", rb_int2inum(sf::Keyboard::F6));
    rb_define_const(rb_mKeyboard, "F7", rb_int2inum(sf::Keyboard::F7));
    rb_define_const(rb_mKeyboard, "F8", rb_int2inum(sf::Keyboard::F8));
    rb_define_const(rb_mKeyboard, "F9", rb_int2inum(sf::Keyboard::F9));
    rb_define_const(rb_mKeyboard, "F10", rb_int2inum(sf::Keyboard::F10));
    rb_define_const(rb_mKeyboard, "F11", rb_int2inum(sf::Keyboard::F11));
    rb_define_const(rb_mKeyboard, "F12", rb_int2inum(sf::Keyboard::F12));
    rb_define_const(rb_mKeyboard, "F13", rb_int2inum(sf::Keyboard::F13));
    rb_define_const(rb_mKeyboard, "F14", rb_int2inum(sf::Keyboard::F14));
    rb_define_const(rb_mKeyboard, "F15", rb_int2inum(sf::Keyboard::F15));
    rb_define_const(rb_mKeyboard, "Pause", rb_int2inum(sf::Keyboard::Pause));
}

/* Reset the clock of the Input System */
void L_Input_Reset_Clocks()
{
    unsigned int i;
    for(i = 0; i < L_INPUT_NUM_INGAME_KEY; i++)
    {
        L_Input_Clock[i].restart();
        L_Input_Press[i] = false;
    }
    for(i = 0;i < L_INPUT_NUM_MOUSE_KEY; i++)
    {
        L_Mouse_Clock[i].restart();
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
            L_Input_Clock[pos].restart();
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
            L_Input_Clock[pos].restart();
        }
    }
    return Qnil;
}

/* Update the key state when an event has been triggered */
void L_Input_Update_Key(sf::Keyboard::Key code, bool state)
{
    rb_block_call(rb_mInputKey, rb_mInputEach, 0, NULL, 
        (rb_block_call_func_t)( state ? rb_Input_KeyStateDown_Block : rb_Input_KeyStateUp_Block),
        rb_int2inum(code));
}

void L_Input_Mouse_Pos_Update(long x, long y)
{
    L_Mouse_Pos_X = x;
    L_Mouse_Pos_Y = y;
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
            L_Mouse_Clock[button].restart();
        }
    }
}

void L_Input_Update_Joy(unsigned int joy_id, unsigned int key, bool state)
{
    rb_block_call(rb_mInputKey, rb_mInputEach, 0, NULL, 
        (rb_block_call_func_t)( state ? rb_Input_KeyStateDown_Block : rb_Input_KeyStateUp_Block),
        rb_int2inum(-(32 * joy_id) - key - 1));
}

void L_Input_Update_JoyPos(unsigned int joy_id, long axis, float position)
{
    VALUE raxis = LONG2FIX(axis);
    if(raxis == rb_mInputMainAxisX)
    {
        if(position < -25.0f)
            if(!L_Input_Press[15])
            {
                L_Input_Press[15] = true;
                L_Input_Clock[15].restart();
            }
        else if(position > 25.0f)
            if(!L_Input_Press[16])
            {
                L_Input_Press[16] = true;
                L_Input_Clock[16].restart();
            }
        else
        {
            if(L_Input_Press[16])
            {
                L_Input_Press[16] = false;
                L_Input_Clock[16].restart();
            }
            if(L_Input_Press[15])
            {
                L_Input_Press[15] = false;
                L_Input_Clock[15].restart();
            }
        }
    }
    else if(raxis == rb_mInputMainAxisY)
    {
        if(position < -25.0f)
            if(!L_Input_Press[13])
            {
                L_Input_Press[13] = true;
                L_Input_Clock[13].restart();
            }
        else if(position > 25.0f)
            if(!L_Input_Press[14])
            {
                L_Input_Press[14] = true;
                L_Input_Clock[14].restart();
            }
        else
        {
            if(L_Input_Press[14])
            {
                L_Input_Press[14] = false;
                L_Input_Clock[14].restart();
            }
            if(L_Input_Press[13])
            {
                L_Input_Press[13] = false;
                L_Input_Clock[13].restart();
            }
        }
    }
}

void L_Input_Setusec_threshold(long usec)
{
    if(usec > 1)
        L_Input_threshold = usec - 1;
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
    if(L_Input_Clock[pos].getElapsedTime().asMicroseconds() < L_Input_threshold)
        return Qtrue;
    return Qfalse;
}

VALUE rb_Input_Repeat(VALUE self, VALUE key_sym)
{
    unsigned int pos = L_Input_Get_Key_Position(rb_mInputKeyID, key_sym);
    bool pressed = L_Input_Press[pos];
    if(!pressed)
        return Qfalse;
    long time = L_Input_Clock[pos].getElapsedTime().asMicroseconds();
    if(time >= (L_Input_threshold * 20))
        time %= (L_Input_threshold * 8); // Yolo val
    /* Trigger */
    if(time < L_Input_threshold)
        return Qtrue;
    return Qfalse;
}

VALUE rb_Input_Released(VALUE self, VALUE key_sym)
{
    unsigned int pos = L_Input_Get_Key_Position(rb_mInputKeyID, key_sym);
    bool pressed = L_Input_Press[pos];
    if(pressed)
        return Qfalse;
    if(L_Input_Clock[pos].getElapsedTime().asMicroseconds() < L_Input_threshold)
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
    if(L_Mouse_Clock[pos].getElapsedTime().asMicroseconds() < L_Input_threshold)
        return Qtrue;
    return Qfalse;
}

VALUE rb_Mouse_Released(VALUE self, VALUE key_sym)
{
    unsigned int pos = L_Input_Get_Key_Position(rb_mMouseKeyID, key_sym);
    bool pressed = L_Mouse_Press[pos];
    if(pressed)
        return Qfalse;
    if(L_Mouse_Clock[pos].getElapsedTime().asMicroseconds() < L_Input_threshold)
        return Qtrue;
    return Qfalse;
}

VALUE rb_Mouse_x(VALUE self)
{
    return rb_int2inum(L_Mouse_Pos_X);
}

VALUE rb_Mouse_y(VALUE self)
{
    return rb_int2inum(L_Mouse_Pos_Y);
}

VALUE rb_Mouse_Wheel(VALUE self)
{
    return rb_int2inum(L_Mouse_Wheel_Delta);
}

VALUE rb_Mouse_Wheel_set(VALUE self, VALUE val)
{
    L_Mouse_Wheel_Delta = rb_num2long(val);
    return val;
}