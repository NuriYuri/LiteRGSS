#include "LiteRGSS.h"
#include "CBitmap_Element.h"
#include "CRect_Element.h"

VALUE rb_cWindow = Qnil;


#define WINDOW_PROTECT if(RDATA(self)->data == nullptr) \
{\
    rb_raise(rb_eRGSSError, "Disposed Window."); \
}

#define GET_WINDOW CWindow_Element* window; \
    Data_Get_Struct(self, CWindow_Element, window); \
    WINDOW_PROTECT

void rb_Window_Free(void* data)
{
	CWindow_Element* window = reinterpret_cast<CWindow_Element*>(data);
	if (window != nullptr)
	{
		if (NIL_P(window->rViewport)) // I can drop a sprite from the viewport it's stored in its table
			window->setOriginStack(nullptr);
		CRect_Element* rect = window->getLinkedRect();
		if (rect != nullptr)
			rect->setElement(nullptr);
		delete window;
	}
}

void rb_Window_Mark(CWindow_Element* window)
{
	if (window == nullptr)
		return;
	rb_gc_mark(window->rViewport);
	rb_gc_mark(window->rBitmap);
	rb_gc_mark(window->rX);
	rb_gc_mark(window->rY);
	rb_gc_mark(window->rZ);
	rb_gc_mark(window->rOX);
	rb_gc_mark(window->rOY);
	rb_gc_mark(window->rWidth);
	rb_gc_mark(window->rHeight);
	rb_gc_mark(window->rCursorRect);
	rb_gc_mark(window->rBackOpacity);
	rb_gc_mark(window->rContentOpacity);
	rb_gc_mark(window->rOpacity);
	rb_gc_mark(window->rWindowBuilder);
	rb_gc_mark(window->rPauseSkin);
	rb_gc_mark(window->rCursorSkin);
	rb_gc_mark(window->rPause);
	rb_gc_mark(window->rPauseX);
	rb_gc_mark(window->rPauseY);
}

VALUE rb_Window_Alloc(VALUE klass)
{
	CWindow_Element* window = new CWindow_Element();
	window->setLinkedRect(nullptr);
	return Data_Wrap_Struct(klass, rb_Window_Mark, rb_Window_Free, window);
}

void Init_Window()
{
	rb_cWindow = rb_define_class_under(rb_mLiteRGSS, "Window", rb_cObject);
	rb_define_alloc_func(rb_cWindow, rb_Window_Alloc);

	rb_define_method(rb_cWindow, "initialize", _rbf rb_Window_Initialize, -1);
	rb_define_method(rb_cWindow, "dispose", _rbf rb_Window_Dispose, 0);
	rb_define_method(rb_cWindow, "disposed?", _rbf rb_Window_Disposed, 0);
	rb_define_method(rb_cWindow, "windowskin=", _rbf rb_Window_setWindowSkin, 1);
	rb_define_method(rb_cWindow, "windowskin", _rbf rb_Window_getWindowSkin, 0);
	rb_define_method(rb_cWindow, "width=", _rbf rb_Window_setWidth, 1);
	rb_define_method(rb_cWindow, "width", _rbf rb_Window_getWidth, 0);
	rb_define_method(rb_cWindow, "height=", _rbf rb_Window_setHeight, 1);
	rb_define_method(rb_cWindow, "height", _rbf rb_Window_getHeight, 0);
	rb_define_method(rb_cWindow, "set_size", _rbf rb_Window_setSize, 2);
	rb_define_method(rb_cWindow, "window_builder=", _rbf rb_Window_setWindowBuilder, 1);
	rb_define_method(rb_cWindow, "window_builder", _rbf rb_Window_getWindowBuilder, 0);
	rb_define_method(rb_cWindow, "x=", _rbf rb_Window_setX, 1);
	rb_define_method(rb_cWindow, "x", _rbf rb_Window_getX, 0);
	rb_define_method(rb_cWindow, "y=", _rbf rb_Window_setY, 1);
	rb_define_method(rb_cWindow, "y", _rbf rb_Window_getY, 0);
	rb_define_method(rb_cWindow, "set_position", _rbf rb_Window_setPosition, 2);
	rb_define_method(rb_cWindow, "z=", _rbf rb_Window_setZ, 1);
	rb_define_method(rb_cWindow, "z", _rbf rb_Window_getZ, 0);
	rb_define_method(rb_cWindow, "ox=", _rbf rb_Window_setOX, 1);
	rb_define_method(rb_cWindow, "ox", _rbf rb_Window_getOX, 0);
	rb_define_method(rb_cWindow, "oy=", _rbf rb_Window_setOY, 1);
	rb_define_method(rb_cWindow, "oy", _rbf rb_Window_getOY, 0);
	rb_define_method(rb_cWindow, "set_origin", _rbf rb_Window_setOrigin, 2);
	rb_define_method(rb_cWindow, "__index__", _rbf rb_Window_getIndex, 0);

	rb_define_method(rb_cWindow, "clone", _rbf rb_Window_Copy, 0);
	rb_define_method(rb_cWindow, "dup", _rbf rb_Window_Copy, 0);
}

VALUE rb_Window_Initialize(int argc, VALUE* argv, VALUE self)
{
	GET_WINDOW;
	VALUE viewport;
	VALUE table;
	
	rb_scan_args(argc, argv, "01", &viewport);

	/* If a viewport was specified */
	if (argc == 1 && rb_obj_is_kind_of(argv[0], rb_cViewport) == Qtrue)
	{
		CViewport_Element* viewport;
		Data_Get_Struct(argv[0], CViewport_Element, viewport);
		viewport->bind(window);
		table = rb_ivar_get(argv[0], rb_iElementTable);
		window->rViewport = argv[0];
	}
	/* Otherwise */
	else
	{
		global_Graphics_Bind(window);
		table = rb_ivar_get(rb_mGraphics, rb_iElementTable);
		window->rViewport = Qnil;
	}

	return self;
}


VALUE rb_Window_Dispose(VALUE self)
{
	GET_WINDOW;
	RDATA(self)->data = nullptr;
	/* Suppression de la fenêtre de ses stacks */
	VALUE viewport = window->rViewport;
	VALUE table;
	if (NIL_P(viewport))
		table = rb_ivar_get(rb_mGraphics, rb_iElementTable);
	else
		table = rb_ivar_get(viewport, rb_iElementTable);
	rb_ary_delete(table, self);
	window->setOriginStack(nullptr);
	rb_Window_Free(window);
	return self;
}

VALUE rb_Window_Disposed(VALUE self)
{
	Check_Type(self, T_DATA);
	return (RDATA(self)->data == nullptr ? Qtrue : Qfalse);
}


VALUE rb_Window_setWindowSkin(VALUE self, VALUE val)
{
	GET_WINDOW;
	if (val != Qnil)
	{
		window->setTexture(rb_Bitmap_getTexture(val));
		window->rBitmap = val;
		window->updateVertices();
	}
	else
	{
		window->setTexture(nullptr);
		window->rBitmap = Qnil;
	}
	
	return self;
}

VALUE rb_Window_getWindowSkin(VALUE self)
{
	GET_WINDOW;
	return window->rBitmap;
}

VALUE rb_Window_setWidth(VALUE self, VALUE val)
{
	GET_WINDOW;
	NUM2ULONG(val);
	window->rWidth = val;
	window->updateVertices();
	return self;
}

VALUE rb_Window_getWidth(VALUE self)
{
	GET_WINDOW;
	return window->rWidth;
}

VALUE rb_Window_setHeight(VALUE self, VALUE val)
{
	GET_WINDOW;
	NUM2ULONG(val);
	window->rHeight = val;
	window->updateVertices();
	return self;
}

VALUE rb_Window_getHeight(VALUE self)
{
	GET_WINDOW;
	return window->rHeight;
}

VALUE rb_Window_setSize(VALUE self, VALUE x, VALUE y)
{
	GET_WINDOW;
	NUM2ULONG(x);
	NUM2ULONG(y);
	window->rWidth = x;
	window->rHeight = y;
	window->updateVertices();
	return self;
}

VALUE rb_Window_setWindowBuilder(VALUE self, VALUE val)
{
	GET_WINDOW;
	Check_Type(val, T_ARRAY);
	// Lenght check
	if (rb_array_len(val) < 6)
	{
		rb_raise(rb_eArgError, "Window Builder should be 6 Integer long.");
	}
	// Type check
	for (long i = 0; i < 6; i++)
	{
		NUM2ULONG(rb_ary_entry(val, i));
	}
	// Freeze window builder
	rb_obj_freeze(val);
	window->rWindowBuilder = val;
	window->updateVertices();
	return self;
}

VALUE rb_Window_getWindowBuilder(VALUE self)
{
	GET_WINDOW;
	return window->rWindowBuilder;
}

VALUE rb_Window_setX(VALUE self, VALUE val)
{
	GET_WINDOW;
	NUM2LONG(val);
	window->rX = val;
	window->updateVertices();
	return self;
}

VALUE rb_Window_getX(VALUE self)
{
	GET_WINDOW;
	return window->rX;
}

VALUE rb_Window_setY(VALUE self, VALUE val)
{
	GET_WINDOW;
	NUM2LONG(val);
	window->rY = val;
	window->updateVertices();
	return self;
}

VALUE rb_Window_getY(VALUE self)
{
	GET_WINDOW;
	return window->rY;
}

VALUE rb_Window_setPosition(VALUE self, VALUE x, VALUE y)
{
	GET_WINDOW;
	NUM2LONG(x);
	NUM2LONG(y);
	window->rX = x;
	window->rY = y;
	window->updateVertices();
	return self;
}

VALUE rb_Window_setZ(VALUE self, VALUE val)
{
	GET_WINDOW;
	NUM2LONG(val);
	window->rZ = val;
	return self;
}

VALUE rb_Window_getZ(VALUE self)
{
	GET_WINDOW;
	return window->rZ;
}

VALUE rb_Window_setOX(VALUE self, VALUE val)
{
	GET_WINDOW;
	NUM2LONG(val);
	window->rOX = val;
	window->updateContents();
	return self;
}

VALUE rb_Window_getOX(VALUE self)
{
	GET_WINDOW;
	return window->rOX;
}

VALUE rb_Window_setOY(VALUE self, VALUE val)
{
	GET_WINDOW;
	NUM2LONG(val);
	window->rOY = val;
	window->updateContents();
	return self;
}

VALUE rb_Window_getOY(VALUE self)
{
	GET_WINDOW;
	return window->rOY;
}

VALUE rb_Window_setOrigin(VALUE self, VALUE x, VALUE y)
{
	GET_WINDOW;
	NUM2LONG(x);
	NUM2LONG(y);
	window->rOX = x;
	window->rOY = y;
	window->updateContents();
	return self;
}



VALUE rb_Window_getIndex(VALUE self)
{
	GET_WINDOW;
	return ULONG2NUM(window->getIndex());
}

VALUE rb_Window_Copy(VALUE self)
{
	rb_raise(rb_eRGSSError, "Windows cannot be cloned or duplicated.");
	return self;
}

CWindow_Element* rb_Window_get_window(VALUE self)
{
	rb_Window_test_window(self);
	GET_WINDOW;
	return window;
}

void rb_Window_test_window(VALUE self)
{
	if (rb_obj_is_kind_of(self, rb_cWindow) != Qtrue)
	{
		rb_raise(rb_eTypeError, "Expected Window got %s.", RSTRING_PTR(rb_class_name(CLASS_OF(self))));
	}
}
