#include "LiteRGSS.h"

VALUE rb_mYuki = Qnil;

void Init_Yuki()
{
	rb_mYuki = rb_define_module("Yuki");
	rb_define_module_function(rb_mYuki, "get_clipboard", _rbf rb_Yuki_getClipboard, 0);
	rb_define_module_function(rb_mYuki, "set_clipboard", _rbf rb_Yuki_setClipboard, 1);
}

/* Clipboard */
// Waiting the clipboard lib to be released
#if defined(SFML_SYSTEM_WINDOWS)
#include <windows.h>
VALUE rb_Yuki_getClipboard(VALUE self)
{
	if (!IsClipboardFormatAvailable(CF_UNICODETEXT))
	{
		std::cerr << "Failed to get the clipboard data in Unicode format." << std::endl;
		return Qnil;
	}

	if (!OpenClipboard(NULL))
	{
		std::cerr << "Failed to open the Win32 clipboard." << std::endl;
		return Qnil;
	}

	HANDLE clipboard_handle = GetClipboardData(CF_UNICODETEXT);

	if (!clipboard_handle)
	{
		std::cerr << "Failed to get Win32 handle for clipboard content." << std::endl;
		CloseClipboard();
		return Qnil;
	}

	wchar_t* str = static_cast<wchar_t*>(GlobalLock(clipboard_handle));
	VALUE ret_val = rb_str_new(
		reinterpret_cast<char*>(str),
		lstrlenW(str) * sizeof(WCHAR));

	GlobalUnlock(clipboard_handle);

	CloseClipboard();

	rb_funcall(ret_val, rb_intern("force_encoding"), 1, rb_str_new_cstr("UTF-16LE"));
	
	return rb_funcall(ret_val, rb_intern("encode"), 1, rb_str_new_cstr("UTF-8"));
}

VALUE rb_Yuki_setClipboard(VALUE self, VALUE val)
{
	rb_check_type(val, T_STRING);
	if (!OpenClipboard(NULL))
	{
		std::cerr << "Failed to open the Win32 clipboard." << std::endl;
		return self;
	}

	if (!EmptyClipboard())
	{
		std::cerr << "Failed to empty the Win32 clipboard." << std::endl;
		return self;
	}

	// Create a Win32-compatible string
	VALUE val2 = rb_funcall(val, rb_intern("encode"), 1, rb_str_new_cstr("UTF-16LE"));
	size_t string_size = RSTRING_LEN(val2);
	HANDLE string_handle = GlobalAlloc(GMEM_MOVEABLE, string_size + 2);

	if (string_handle)
	{
		memcpy(GlobalLock(string_handle), RSTRING_PTR(val2), string_size);
		GlobalUnlock(string_handle);
		SetClipboardData(CF_UNICODETEXT, string_handle);
	}

	CloseClipboard();
	return self;
}
#elif defined(SFML_SYSTEM_LINUX) || defined(SFML_SYSTEM_FREEBSD)
#include <iostream>
#include <string>
#include <X11/Xlib.h>
void initClipboard();
void* hostSelection(void*);

sf::String string;

pthread_mutex_t mutex;
pthread_t host_thread;

bool is_fail = false;
bool is_init = false;
bool is_host = false;

Display* display = NULL;
Window window = 0;

Atom selection = 0;
Atom atom_targ = 0;
Atom atom_text = 0;
Atom utf8_text = 0;
int xa_string = 31;
int xa_atom = 4;

////////////////////////////////////////////////////////////
void initClipboard()
{
	is_init = true;

	display = XOpenDisplay(NULL);
	int screen = DefaultScreen(display);
	window = XCreateSimpleWindow(display, RootWindow(display, screen),
		0, 0, 1, 1, 0, BlackPixel(display, screen), WhitePixel(display, screen));

	selection = XInternAtom(display, "CLIPBOARD", false);
	atom_targ = XInternAtom(display, "TARGETS", false);
	atom_text = XInternAtom(display, "TEXT", false);
	utf8_text = XInternAtom(display, "UTF8_STRING", true);

	if (utf8_text == None)
	{
		std::cerr << "UTF-8 format unavailable on clipboard." << std::endl;
		utf8_text = xa_string;
	}

	if (pthread_mutex_init(&mutex, NULL))
	{
		is_fail = true;
		std::cerr << "Unable to initialize mutex. Failed to initialize clipboard." << std::endl;
		return;
	}

	if (pthread_create(&host_thread, NULL, hostSelection, NULL))
	{
		is_fail = true;
		std::cerr << "Unable to create host thread. Failed to initialize clipboard." << std::endl;
		return;
	}
}

////////////////////////////////////////////////////////////
void* hostSelection(void*)
{
	while (true)
	{
		if (XPending(display) && is_host)
		{
			XEvent event;

			pthread_mutex_lock(&mutex);
			XNextEvent(display, &event);
			pthread_mutex_unlock(&mutex);

			switch (event.type)
			{
			case SelectionClear:
			{
				pthread_mutex_lock(&mutex);
				is_host = false;
				pthread_mutex_unlock(&mutex);

				break;
			}
			case SelectionRequest:
			{
				if (event.xselectionrequest.selection == selection)
				{
					XSelectionRequestEvent* sel_req_event = &event.xselectionrequest;
					XSelectionEvent sel_event = { 0 };

					int result = 0;
					sel_event.type = SelectionNotify,
						sel_event.display = sel_req_event->display,
						sel_event.requestor = sel_req_event->requestor,
						sel_event.selection = sel_req_event->selection,
						sel_event.time = sel_req_event->time,
						sel_event.target = sel_req_event->target,
						sel_event.property = sel_req_event->property;

					std::basic_string<unsigned char> str = string.toUtf8();

					if (sel_event.target == atom_targ)
						result = XChangeProperty(sel_event.display, sel_event.requestor,
							sel_event.property, xa_atom, 32, PropModeReplace,
							reinterpret_cast<unsigned char*>(&utf8_text), 1);
					else if (sel_event.target == xa_string || sel_event.target == atom_text)
						result = XChangeProperty(sel_event.display, sel_event.requestor,
							sel_event.property, xa_string, 8, PropModeReplace,
							reinterpret_cast<unsigned char*>(&str[0]), str.size());
					else if (sel_event.target == utf8_text)
						result = XChangeProperty(sel_event.display, sel_event.requestor,
							sel_event.property, utf8_text, 8, PropModeReplace,
							reinterpret_cast<unsigned char*>(&str[0]), str.size());
					else
						sel_event.property = None;

					if ((result & 2) == 0)
						XSendEvent(display, sel_event.requestor, 0, 0,
							reinterpret_cast<XEvent*>(&sel_event));
				}
				break;
			}
			default: break;
			}
		}
		else
			sf::sleep(sf::milliseconds(20));
	}
}

VALUE rb_Yuki_getClipboard(VALUE self)
{
	VALUE ret_val = Qnil;
	if (!is_init)
		initClipboard();

	if (is_fail || is_host)
		return string;

	// Dangerous! Wipes all previous events!
	XSync(display, true);
	XConvertSelection(display, selection, utf8_text, atom_text, window, CurrentTime);

	XEvent event;

	pthread_mutex_lock(&mutex);
	XNextEvent(display, &event);
	pthread_mutex_unlock(&mutex);

	if (event.type == SelectionNotify)
	{
		if (event.xselection.selection != selection || event.xselection.target != utf8_text)
		{
			std::cerr << "Failed to convert selection." << std::endl;
			return string;
		}

		if (event.xselection.property)
		{
			Atom target;
			int format;
			unsigned long size;
			unsigned long byte_left;
			unsigned char* data;

			XGetWindowProperty(event.xselection.display,
				event.xselection.requestor, event.xselection.property,
				0L, (~0L), false, AnyPropertyType,
				&target, &format, &size, &byte_left, &data);

			if (target == utf8_text)
			{
				ret_val = rb_str_new(data, size);
				XFree(data);
			}

			XDeleteProperty(event.xselection.display, event.xselection.requestor, event.xselection.property);
		}
	}
	return ret_val;
}

VALUE rb_Yuki_setClipboard(VALUE self, VALUE val)
{

	return self;
}
#else

VALUE rb_Yuki_getClipboard(VALUE self)
{
	return Qnil;
}

VALUE rb_Yuki_setClipboard(VALUE self, VALUE val)
{
	if (!is_init)
		initClipboard();

	if (is_fail)
		return;

	if (!is_host)
	{
		XSetSelectionOwner(display, selection, window, CurrentTime);

		if (XGetSelectionOwner(display, selection) != window)
		{
			std::cerr << "Unable to get ownership of selection." << std::endl;
			return;
		}

		pthread_mutex_lock(&mutex);
		is_host = true;
		pthread_mutex_unlock(&mutex);
	}

	pthread_mutex_lock(&mutex);
	string = text;
	pthread_mutex_unlock(&mutex);
	return self;
}
/*#elif defined(SFML_SYSTEM_MACOS)
#include <SFML/Window/OSX/ClipboardImpl.hpp>
#elif defined(SFML_SYSTEM_IOS)
#include <SFML/Window/iOS/ClipboardImpl.hpp>
#elif defined(SFML_SYSTEM_ANDROID)
#include <SFML/Window/Android/ClipboardImpl.hpp>*/
#endif