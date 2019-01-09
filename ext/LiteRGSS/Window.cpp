#include "LiteRGSS.h"
#include "CBitmap_Element.h"
#include "CRect_Element.h"

VALUE rb_cWindow = Qnil;

template<>
void rb::Mark<CWindow_Element>(CWindow_Element* window)
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
	rb_gc_mark(window->rRect);
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
	rb_gc_mark(window->rActive);
	rb_gc_mark(window->rStretch);
}

void Init_Window()
{
	rb_cWindow = rb_define_class_under(rb_mLiteRGSS, "Window", rb_cObject);
	rb_define_alloc_func(rb_cWindow, rb::AllocDrawable<CWindow_Element>);

	rb_define_method(rb_cWindow, "initialize", _rbf rb_Window_Initialize, -1);
	rb_define_method(rb_cWindow, "dispose", _rbf rb_Window_Dispose, 0);
	rb_define_method(rb_cWindow, "disposed?", _rbf rb_Window_Disposed, 0);
	rb_define_method(rb_cWindow, "update", _rbf rb_Window_update, 0);
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
	rb_define_method(rb_cWindow, "cursor_rect", _rbf rb_Window_getCursorRect, 0);
	rb_define_method(rb_cWindow, "cursor_rect=", _rbf rb_Window_setCursorRect, 1);
	rb_define_method(rb_cWindow, "cursorskin", _rbf rb_Window_getCursorSkin, 0);
	rb_define_method(rb_cWindow, "cursorskin=", _rbf rb_Window_setCursorSkin, 1);
	rb_define_method(rb_cWindow, "pauseskin", _rbf rb_Window_getPauseSkin, 0);
	rb_define_method(rb_cWindow, "pauseskin=", _rbf rb_Window_setPauseSkin, 1);
	rb_define_method(rb_cWindow, "pause", _rbf rb_Window_getPause, 0);
	rb_define_method(rb_cWindow, "pause=", _rbf rb_Window_setPause, 1);
	rb_define_method(rb_cWindow, "pause_x", _rbf rb_Window_getPauseX, 0);
	rb_define_method(rb_cWindow, "pause_x=", _rbf rb_Window_setPauseX, 1);
	rb_define_method(rb_cWindow, "pause_y", _rbf rb_Window_getPauseY, 0);
	rb_define_method(rb_cWindow, "pause_y=", _rbf rb_Window_setPauseY, 1);
	rb_define_method(rb_cWindow, "active", _rbf rb_Window_getActive, 0);
	rb_define_method(rb_cWindow, "active=", _rbf rb_Window_setActive, 1);
	rb_define_method(rb_cWindow, "stretch", _rbf rb_Window_getStretch, 0);
	rb_define_method(rb_cWindow, "stretch=", _rbf rb_Window_setStretch, 1);
	rb_define_method(rb_cWindow, "opacity", _rbf rb_Window_getOpacity, 0);
	rb_define_method(rb_cWindow, "opacity=", _rbf rb_Window_setOpacity, 1);
	rb_define_method(rb_cWindow, "back_opacity", _rbf rb_Window_getBackOpacity, 0);
	rb_define_method(rb_cWindow, "back_opacity=", _rbf rb_Window_setBackOpacity, 1);
	rb_define_method(rb_cWindow, "contents_opacity", _rbf rb_Window_getContentsOpacity, 0);
	rb_define_method(rb_cWindow, "contents_opacity=", _rbf rb_Window_setContentsOpacity, 1);
	rb_define_method(rb_cWindow, "rect", _rbf rb_Window_getRect, 0);
	rb_define_method(rb_cWindow, "viewport", _rbf rb_Window_getViewport, 0);
	rb_define_method(rb_cWindow, "visible", _rbf rb_Window_getVisible, 0);
	rb_define_method(rb_cWindow, "visible=", _rbf rb_Window_setVisible, 1);
	rb_define_method(rb_cWindow, "__index__", _rbf rb_Window_getIndex, 0);
	rb_define_method(rb_cWindow, "lock", _rbf rb_Window_lock, 0);
	rb_define_method(rb_cWindow, "unlock", _rbf rb_Window_unlock, 0);
	rb_define_method(rb_cWindow, "locked?", _rbf rb_Window_locked, 0);

	rb_define_method(rb_cWindow, "clone", _rbf rb_Window_Copy, 0);
	rb_define_method(rb_cWindow, "dup", _rbf rb_Window_Copy, 0);
}

VALUE rb_Window_Initialize(int argc, VALUE* argv, VALUE self)
{
	auto& window = rb::Get<CWindow_Element>(self);
	VALUE viewport = Qnil;
	VALUE table = Qnil;
	
	rb_scan_args(argc, argv, "01", &viewport);

	/* If a viewport was specified */
	if (argc == 1 && rb_obj_is_kind_of(argv[0], rb_cViewport) == Qtrue)
	{
		CViewport_Element* viewport;
		Data_Get_Struct(argv[0], CViewport_Element, viewport);
		viewport->bind(window);
		table = rb_ivar_get(argv[0], rb_iElementTable);
		window.rViewport = argv[0];
	}
	/* Otherwise */
	else
	{
		global_Graphics_Bind(&window);
		table = rb_ivar_get(rb_mGraphics, rb_iElementTable);
		window.rViewport = Qnil;
	}
	rb_ary_push(table, self);

	/* Sprite table creation */
	rb_ivar_set(self, rb_iElementTable, rb_ary_new());
	window.clearStack(true);

	/* Rect definition */
	VALUE args[4] = { LONG2FIX(0), LONG2FIX(0), LONG2FIX(0), LONG2FIX(0) };
	window.rRect = rb_class_new_instance(4, args, rb_cRect);
	rb_obj_freeze(window.rRect);

	return self;
}

VALUE rb_Window_getViewport(VALUE self)
{
	auto& window = rb::Get<CWindow_Element>(self);
	return window.rViewport;
}



VALUE rb_Window_DisposeFromViewport(VALUE self)
{
	auto& window = rb::Get<CWindow_Element>(self);
	window.disposeFromViewport();
	return rb::Dispose<CWindow_Element>(self);
}

VALUE rb_Window_Dispose(VALUE self)
{
	return rb::Dispose<CWindow_Element>(self);
}

VALUE rb_Window_Disposed(VALUE self)
{
	Check_Type(self, T_DATA);
	return (RDATA(self)->data == nullptr ? Qtrue : Qfalse);
}


VALUE rb_Window_setWindowSkin(VALUE self, VALUE val)
{
	auto& window = rb::Get<CWindow_Element>(self);
	if (val != Qnil)
	{
		window.setTexture(&rb_Bitmap_getTexture(val));
		window.rBitmap = val;
		window.updateVertices();
	}
	else
	{
		window.setTexture(nullptr);
		window.rBitmap = Qnil;
	}
	
	return self;
}

VALUE rb_Window_getWindowSkin(VALUE self)
{
	auto& window = rb::Get<CWindow_Element>(self);
	return window.rBitmap;
}

VALUE rb_Window_setWidth(VALUE self, VALUE val)
{
	auto& window = rb::Get<CWindow_Element>(self);
	NUM2ULONG(val);
	window.rWidth = val;
	window.updateVertices();
	window.updateView();
	return self;
}

VALUE rb_Window_getWidth(VALUE self)
{
	auto& window = rb::Get<CWindow_Element>(self);
	return window.rWidth;
}

VALUE rb_Window_setHeight(VALUE self, VALUE val)
{
	auto& window = rb::Get<CWindow_Element>(self);
	NUM2ULONG(val);
	window.rHeight = val;
	window.updateVertices();
	window.updateView();
	return self;
}

VALUE rb_Window_getHeight(VALUE self)
{
	auto& window = rb::Get<CWindow_Element>(self);
	return window.rHeight;
}

VALUE rb_Window_setSize(VALUE self, VALUE x, VALUE y)
{
	auto& window = rb::Get<CWindow_Element>(self);
	NUM2ULONG(x);
	NUM2ULONG(y);
	window.rWidth = x;
	window.rHeight = y;
	window.updateVertices();
	window.updateView();
	return self;
}

VALUE rb_Window_setWindowBuilder(VALUE self, VALUE val)
{
	auto& window = rb::Get<CWindow_Element>(self);
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
	window.rWindowBuilder = val;
	window.updateVertices();
	window.updateView();
	return self;
}

VALUE rb_Window_getWindowBuilder(VALUE self)
{
	auto& window = rb::Get<CWindow_Element>(self);
	return window.rWindowBuilder;
}

VALUE rb_Window_setX(VALUE self, VALUE val)
{
	auto& window = rb::Get<CWindow_Element>(self);
	NUM2LONG(val);
	window.rX = val;
	window.updateVertices();
	window.updateView();
	return self;
}

VALUE rb_Window_getX(VALUE self)
{
	auto& window = rb::Get<CWindow_Element>(self);
	return window.rX;
}

VALUE rb_Window_setY(VALUE self, VALUE val)
{
	auto& window = rb::Get<CWindow_Element>(self);
	NUM2LONG(val);
	window.rY = val;
	window.updateVertices();
	window.updateView();
	return self;
}

VALUE rb_Window_getY(VALUE self)
{
	auto& window = rb::Get<CWindow_Element>(self);
	return window.rY;
}

VALUE rb_Window_setPosition(VALUE self, VALUE x, VALUE y)
{
	auto& window = rb::Get<CWindow_Element>(self);
	NUM2LONG(x);
	NUM2LONG(y);
	window.rX = x;
	window.rY = y;
	window.updateVertices();
	window.updateView();
	return self;
}

VALUE rb_Window_setZ(VALUE self, VALUE val)
{
	auto& window = rb::Get<CWindow_Element>(self);
	NUM2LONG(val);
	window.rZ = val;
	return self;
}

VALUE rb_Window_getZ(VALUE self)
{
	auto& window = rb::Get<CWindow_Element>(self);
	return window.rZ;
}

VALUE rb_Window_setOX(VALUE self, VALUE val)
{
	auto& window = rb::Get<CWindow_Element>(self);
	NUM2LONG(val);
	window.rOX = val;
	window.updateContents();
	window.updateView();
	return self;
}

VALUE rb_Window_getOX(VALUE self)
{
	auto& window = rb::Get<CWindow_Element>(self);
	return window.rOX;
}

VALUE rb_Window_setOY(VALUE self, VALUE val)
{
	auto& window = rb::Get<CWindow_Element>(self);
	NUM2LONG(val);
	window.rOY = val;
	window.updateContents();
	window.updateView();
	return self;
}

VALUE rb_Window_getOY(VALUE self)
{
	auto& window = rb::Get<CWindow_Element>(self);
	return window.rOY;
}

VALUE rb_Window_setOrigin(VALUE self, VALUE x, VALUE y)
{
	auto& window = rb::Get<CWindow_Element>(self);
	NUM2LONG(x);
	NUM2LONG(y);
	window.rOX = x;
	window.rOY = y;
	window.updateContents();
	window.updateView();
	return self;
}

VALUE rb_Window_setStretch(VALUE self, VALUE val)
{
	auto& window = rb::Get<CWindow_Element>(self);
	window.rStretch = (RTEST(val) ? Qtrue : Qfalse);
	window.updateVertices();
	return self;
}

VALUE rb_Window_getStretch(VALUE self)
{
	auto& window = rb::Get<CWindow_Element>(self);
	return window.rStretch;
}

VALUE rb_Window_getCursorRect(VALUE self)
{
	auto& window = rb::Get<CWindow_Element>(self);
	VALUE rc = window.rCursorRect;
	if (!NIL_P(rc))
		return rc;
	/* Creating rect */
	VALUE argv[2];
	argv[0] = argv[1] = LONG2FIX(0);
	rc = rb_class_new_instance(2, argv, rb_cRect);
	/* Fetching data */
	auto& rect = rb::GetSafe<CRect_Element>(rc, rb_cRect);
	/* Linking Rect */
	if(rect.getElement() != nullptr) {
        rect.getElement()->setLinkedRect(nullptr);
    }
    rect.setElement(&window);
    if(window.getLinkedRect() != nullptr) {
        window.getLinkedRect()->setElement(nullptr);
    }
    window.setLinkedRect(&rect);
	window.rCursorRect = rc;
	return rc;
}

VALUE rb_Window_setCursorRect(VALUE self, VALUE val)
{
	auto& window = rb::Get<CWindow_Element>(self);
	VALUE rc = rb_Window_getCursorRect(self);
	if (RDATA(rc)->data == nullptr) { return Qnil; }
	/* Getting data to update the rect */
	auto& rect1 = rb::GetSafe<CRect_Element>(val, rb_cRect);
	auto& rect2 = rb::GetSafe<CRect_Element>(rc, rb_cRect);
	/* Copying the rect */
	sf::IntRect& rect_target = rect2.getRect();
	rect_copy(&rect_target, &rect1.getRect());
	/* Update cursor rect */
	window.resetCursorPosition(&rect_target);
	return self;
}

VALUE rb_Window_getCursorSkin(VALUE self)
{
	auto& window = rb::Get<CWindow_Element>(self);
	return window.rCursorSkin;
}

VALUE rb_Window_setCursorSkin(VALUE self, VALUE val)
{
	auto& window = rb::Get<CWindow_Element>(self);
	if (NIL_P(val))
	{
		window.rCursorSkin = Qnil;
	}
	else
	{
		window.getCursorSprite().setTexture(rb_Bitmap_getTexture(val));
		window.rCursorSkin = val;
		window.updateCursorSprite();
	}
	return self;
}

VALUE rb_Window_getPauseSkin(VALUE self)
{
	auto& window = rb::Get<CWindow_Element>(self);
	return window.rPauseSkin;
}

VALUE rb_Window_setPauseSkin(VALUE self, VALUE val)
{
	auto& window = rb::Get<CWindow_Element>(self);
	if (NIL_P(val))
	{
		window.rPauseSkin = Qnil;
	}
	else
	{
		window.getPauseSprite().setTexture(rb_Bitmap_getTexture(val));
		window.rPauseSkin = val;
		window.resetPausePosition();
		window.updatePauseSprite();
	}
	return self;
}

VALUE rb_Window_getPause(VALUE self)
{
	auto& window = rb::Get<CWindow_Element>(self);
	return window.rPause;
}

VALUE rb_Window_setPause(VALUE self, VALUE val)
{
	auto& window = rb::Get<CWindow_Element>(self);
	window.rPause = RTEST(val) ? Qtrue : Qfalse;
	window.updatePauseSprite();
	return self;
}

VALUE rb_Window_getPauseX(VALUE self)
{
	auto& window = rb::Get<CWindow_Element>(self);
	return window.rPauseX;
}

VALUE rb_Window_setPauseX(VALUE self, VALUE val)
{
	auto& window = rb::Get<CWindow_Element>(self);
	if (NIL_P(val))
	{
		window.rPauseX = Qnil;
	}
	else
	{
		NUM2LONG(val);
		window.rPauseX = val;
		window.resetPausePosition();
	}
	return self;
}

VALUE rb_Window_getPauseY(VALUE self)
{
	auto& window = rb::Get<CWindow_Element>(self);
	return window.rPauseY;
}

VALUE rb_Window_setPauseY(VALUE self, VALUE val)
{
	auto& window = rb::Get<CWindow_Element>(self);
	if (NIL_P(val))
	{
		window.rPauseY = Qnil;
	}
	else
	{
		NUM2LONG(val);
		window.rPauseY = val;
		window.resetPausePosition();
	}
	return self;
}

VALUE rb_Window_getActive(VALUE self)
{
	auto& window = rb::Get<CWindow_Element>(self);
	return window.rActive;
}

VALUE rb_Window_setActive(VALUE self, VALUE val)
{
	auto& window = rb::Get<CWindow_Element>(self);
	window.rActive = RTEST(val) ? Qtrue : Qfalse;
	window.resetCursorPosition(&rb::GetSafe<CRect_Element>(rb_Window_getCursorRect(self), rb_cRect).getRect());
	window.updateCursorSprite();
	return self;
}

VALUE rb_Window_update(VALUE self)
{
	auto& window = rb::Get<CWindow_Element>(self);
	window.update();
	return self;
}

VALUE rb_Window_getOpacity(VALUE self)
{
	auto& window = rb::Get<CWindow_Element>(self);
	return window.rOpacity;
}

VALUE rb_Window_setOpacity(VALUE self, VALUE val)
{
	auto& window = rb::Get<CWindow_Element>(self);
	window.rOpacity = LONG2NUM(normalize_long(NUM2LONG(val), 0, 255));
	window.updateBackOpacity();
	window.updateContentsOpacity();
	return self;
}

VALUE rb_Window_getBackOpacity(VALUE self)
{
	auto& window = rb::Get<CWindow_Element>(self);
	return window.rBackOpacity;
}

VALUE rb_Window_setBackOpacity(VALUE self, VALUE val)
{
	auto& window = rb::Get<CWindow_Element>(self);
	window.rBackOpacity = LONG2NUM(normalize_long(NUM2LONG(val), 0, 255));
	window.updateBackOpacity();
	return self;
}

VALUE rb_Window_getContentsOpacity(VALUE self)
{
	auto& window = rb::Get<CWindow_Element>(self);
	return window.rContentOpacity;
}

VALUE rb_Window_setContentsOpacity(VALUE self, VALUE val)
{
	auto& window = rb::Get<CWindow_Element>(self);
	window.rContentOpacity = LONG2NUM(normalize_long(NUM2LONG(val), 0, 255));
	window.updateContentsOpacity();
	return self;
}

VALUE rb_Window_getRect(VALUE self)
{
	auto& window = rb::Get<CWindow_Element>(self);
	return window.rRect;
}

VALUE rb_Window_getVisible(VALUE self)
{
	auto& window = rb::Get<CWindow_Element>(self);
	return window.getVisible() ? Qtrue : Qfalse;
}

VALUE rb_Window_setVisible(VALUE self, VALUE val)
{
	auto& window = rb::Get<CWindow_Element>(self);
	window.setVisible(RTEST(val) ? true : false);
	return self;
}

VALUE rb_Window_getIndex(VALUE self)
{
	auto& window = rb::Get<CWindow_Element>(self);
	return ULONG2NUM(window.getIndex());
}

VALUE rb_Window_lock(VALUE self)
{
	auto& window = rb::Get<CWindow_Element>(self);
	window.lock();
	return self;
}

VALUE rb_Window_unlock(VALUE self)
{
	auto& window = rb::Get<CWindow_Element>(self);
	window.unlock();
	return self;
}

VALUE rb_Window_locked(VALUE self)
{
	auto& window = rb::Get<CWindow_Element>(self);
	return window.is_locked() ? Qtrue : Qfalse;
}

VALUE rb_Window_Copy(VALUE self)
{
	rb_raise(rb_eRGSSError, "Windows cannot be cloned or duplicated.");
	return self;
}

