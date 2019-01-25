#include "LiteRGSS.h"
#include "CViewport_Element.h"
#include "CRect_Element.h"
#include "CTone_Element.h"

VALUE rb_cViewport = Qnil;

template<>
void rb::Mark<CViewport_Element>(CViewport_Element* viewport)
{
    if(viewport == nullptr)
        return;
    rb_gc_mark(viewport->rRect);
    rb_gc_mark(viewport->rTone);
    rb_gc_mark(viewport->rColor);
    rb_gc_mark(viewport->rZ);
	rb_gc_mark(viewport->rAngle);
	rb_gc_mark(viewport->rZoom);
	rb_gc_mark(viewport->rRenderState);
}

void Init_Viewport()
{
    rb_cViewport = rb_define_class_under(rb_mLiteRGSS, "Viewport", rb_cObject);

    rb_define_alloc_func(rb_cViewport, rb::AllocDrawable<CViewport_Element>);

    rb_define_method(rb_cViewport, "initialize", _rbf rb_Viewport_Initialize, -1);
    rb_define_method(rb_cViewport, "ox", _rbf rb_Viewport_getOX, 0);
    rb_define_method(rb_cViewport, "ox=", _rbf rb_Viewport_setOX, 1);
    rb_define_method(rb_cViewport, "oy", _rbf rb_Viewport_getOY, 0);
    rb_define_method(rb_cViewport, "oy=", _rbf rb_Viewport_setOY, 1);
    rb_define_method(rb_cViewport, "rect", _rbf rb_Viewport_getRect, 0);
    rb_define_method(rb_cViewport, "rect=", _rbf rb_Viewport_setRect, 1);
    rb_define_method(rb_cViewport, "dispose", _rbf rb_Viewport_Dispose, 0);
    rb_define_method(rb_cViewport, "disposed?", _rbf rb_Viewport_Disposed, 0);
    rb_define_method(rb_cViewport, "tone", _rbf rb_Viewport_getTone, 0);
    rb_define_method(rb_cViewport, "tone=", _rbf rb_Viewport_setTone, 1);
    rb_define_method(rb_cViewport, "color", _rbf rb_Viewport_getColor, 0);
    rb_define_method(rb_cViewport, "color=", _rbf rb_Viewport_setColor, 1);
    rb_define_method(rb_cViewport, "update", _rbf rb_Viewport_Update, 0);
    rb_define_method(rb_cViewport, "visible", _rbf rb_Viewport_getVisible, 0);
    rb_define_method(rb_cViewport, "visible=", _rbf rb_Viewport_setVisible, 1);
    rb_define_method(rb_cViewport, "z", _rbf rb_Viewport_getZ, 0);
    rb_define_method(rb_cViewport, "z=", _rbf rb_Viewport_setZ, 1);
    rb_define_method(rb_cViewport, "zoom", _rbf rb_Viewport_getZoom, 0);
    rb_define_method(rb_cViewport, "zoom=", _rbf rb_Viewport_setZoom, 1);
    rb_define_method(rb_cViewport, "angle", _rbf rb_Viewport_getAngle, 0);
    rb_define_method(rb_cViewport, "angle=", _rbf rb_Viewport_setAngle, 1);
    rb_define_method(rb_cViewport, "shader", _rbf rb_Viewport_getRenderState, 0);
    rb_define_method(rb_cViewport, "shader=", _rbf rb_Viewport_setRenderState, 1);
	rb_define_method(rb_cViewport, "blendmode", _rbf rb_Viewport_getRenderState, 0);
	rb_define_method(rb_cViewport, "blendmode=", _rbf rb_Viewport_setRenderState, 1);
    rb_define_method(rb_cViewport, "reload_stack", _rbf rb_Viewport_ReloadStack, 0);
    rb_define_method(rb_cViewport, "__index__", _rbf rb_Viewport_Index, 0);

    rb_define_method(rb_cViewport, "clone", _rbf rb_Viewport_Copy, 0);
    rb_define_method(rb_cViewport, "dup", _rbf rb_Viewport_Copy, 0);
}

VALUE rb_Viewport_Copy(VALUE self)
{
    rb_raise(rb_eRGSSError, "Viewports can not be cloned or duplicated.");
    return self;
}

VALUE rb_Viewport_Initialize(int argc, VALUE* argv, VALUE self)
{
    VALUE x, y, width, height;
    rb_scan_args(argc, argv, "22", &x, &y, &width, &height);
    /* Parameter normalization */
    if(NIL_P(width))
    {
        width = x;
        height = y;
        x = LONG2FIX(0);
        y = LONG2FIX(0);
    }
    else if(NIL_P(height))
    {
        height = width;
        width = y;
        y = LONG2FIX(0);
    }
    /* Sprite table creation */
    rb_ivar_set(self, rb_iElementTable, rb_ary_new());
    /* Viewport setting */
    auto& viewport = rb::Get<CViewport_Element>(self);
    global_Graphics_Bind(&viewport);
    VALUE table = rb_ivar_get(rb_mGraphics, rb_iElementTable);
    rb_ary_push(table, self);
    viewport.setOx(0);
    viewport.setOy(0);
	viewport.rAngle = LONG2FIX(0);
	viewport.rZoom = LONG2FIX(1);
	viewport.rZ = LONG2FIX(0);
    Viewport_SetView(viewport, rb_num2long(x), rb_num2long(y), rb_num2long(width), rb_num2long(height));
    /* Creating rect */
    VALUE rc = rb_class_new_instance(argc, argv, rb_cRect);
    /* Fetching data */
    CRect_Element* rect;
    Data_Get_Struct(rc, CRect_Element, rect);
	viewport.bindRect(rect);
    viewport.rRect = rc;
    viewport.rTone = Qnil;
    viewport.rColor = Qnil;
    viewport.detachSprites();
    return self;
}


VALUE rb_Viewport_Dispose(VALUE self)
{
	return rb::Dispose<CViewport_Element>(self);
}

VALUE rb_Viewport_Disposed(VALUE self)
{
    rb_check_type(self, T_DATA);
    return (RDATA(self)->data == nullptr ? Qtrue : Qfalse);
}

void Viewport_AdjustOXY(CViewport_Element& viewport, VALUE rect)
{
    if(RDATA(rect)->data == nullptr)
        return;
    CRect_Element* srect;
    Data_Get_Struct(rect, CRect_Element, srect);
    sf::IntRect& rc = srect->getRect();
    sf::View& view = viewport.getView();
    view.setCenter(std::roundf(static_cast<float>(viewport.getOx()) + static_cast<float>(rc.width) / 2.0f),
                    std::roundf(static_cast<float>(viewport.getOy()) + static_cast<float>(rc.height) / 2.0f));
}

VALUE rb_Viewport_getOX(VALUE self)
{
    auto& viewport = rb::Get<CViewport_Element>(self);
    return rb_int2inum(viewport.getOx());
}

VALUE rb_Viewport_setOX(VALUE self, VALUE val)
{
	auto& viewport = rb::Get<CViewport_Element>(self);
    viewport.setOx(rb_num2long(val));
    Viewport_AdjustOXY(viewport, viewport.rRect);
    return val;
}

VALUE rb_Viewport_getOY(VALUE self)
{
    auto& viewport = rb::Get<CViewport_Element>(self);
    return rb_int2inum(viewport.getOy());
}

VALUE rb_Viewport_setOY(VALUE self, VALUE val)
{
    auto& viewport = rb::Get<CViewport_Element>(self);
    viewport.setOy(rb_num2long(val));
    Viewport_AdjustOXY(viewport, viewport.rRect);
    return val;
}

VALUE rb_Viewport_getRect(VALUE self)
{
    auto& viewport = rb::Get<CViewport_Element>(self);
    return viewport.rRect;
}

VALUE rb_Viewport_setRect(VALUE self, VALUE val)
{
    VALUE rc = rb_Viewport_getRect(self);
    if(RDATA(rc)->data == nullptr) { return Qnil; }
    if(rb_obj_is_kind_of(val, rb_cRect) != Qtrue)
    {
        rb_raise(rb_eTypeError, "Expected Rect got %s.", RSTRING_PTR(rb_class_name(CLASS_OF(val))));
        return Qnil;
    }
    if(RDATA(val)->data == nullptr) { return Qnil; }
    CViewport_Element* viewport;
    Data_Get_Struct(self, CViewport_Element, viewport);
    /* Getting data to update the rect */
    CRect_Element* rect1;
    Data_Get_Struct(val, CRect_Element, rect1);
    CRect_Element* rect2;
    Data_Get_Struct(rc, CRect_Element, rect2);
    /* Copying the rect */
    sf::IntRect& rect_target = rect2->getRect();
    rect_copy(&rect_target, &rect1->getRect());
    /* Updating the viewport view */
    Viewport_SetView(*viewport, rect_target.left, rect_target.top, rect_target.width, rect_target.height);
    return val;

}

VALUE rb_Viewport_getTone(VALUE self)
{
    auto& viewport = rb::Get<CViewport_Element>(self);
    VALUE tn = viewport.rTone;
    if(!NIL_P(tn))
        return tn;
    /* New tone */
    VALUE argv[4] = {LONG2FIX(0), LONG2FIX(0), LONG2FIX(0), LONG2FIX(0)};
    viewport.rColor = rb_class_new_instance(4, argv, rb_cColor);
    tn = rb_class_new_instance(4, argv, rb_cTone);
    CTone_Element* tone;
    Data_Get_Struct(tn, CTone_Element, tone);
    tone->bindViewport(&viewport);
    viewport.rTone = tn;
    viewport.create_render();
    return tn;
}

VALUE rb_Viewport_setTone(VALUE self, VALUE val)
{
    VALUE tn = rb_Viewport_getTone(self);
    if(RDATA(tn)->data == nullptr) { return Qnil; }
    if(rb_obj_is_kind_of(val, rb_cTone) != Qtrue)
    {
        rb_raise(rb_eTypeError, "Expected Tone, got %s", RSTRING_PTR(rb_class_name(CLASS_OF(val))));
        return Qnil;
    }
    if(RDATA(val)->data == nullptr) { return Qnil; }
    auto& viewport = rb::Get<CViewport_Element>(self);
    CTone_Element* tonesrc;
    Data_Get_Struct(val, CTone_Element, tonesrc);
    CTone_Element* tonedest;
    Data_Get_Struct(tn, CTone_Element, tonedest);
	sf::Glsl::Vec4* stone = tonesrc->getTone();
	sf::Glsl::Vec4* vtone = viewport.getTone();
	if (vtone->x != stone->x || vtone->y != stone->y || vtone->z != stone->z || vtone->w != vtone->w)
	{
		tone_copy(tonedest->getTone(), stone);
		tone_copy(vtone, stone);
		viewport.updatetone();
	}
    return val;
}

VALUE rb_Viewport_getColor(VALUE self)
{
    VALUE tn = rb_Viewport_getTone(self);
    auto& viewport = rb::Get<CViewport_Element>(self);
    return viewport.rColor;
}

VALUE rb_Viewport_setColor(VALUE self, VALUE val)
{
    VALUE tn = rb_Viewport_getTone(self);
    auto& viewport = rb::Get<CViewport_Element>(self);
    if(rb_obj_is_kind_of(val, rb_cColor) != Qtrue)
    {
        rb_raise(rb_eTypeError, "Expected Color, got %s", RSTRING_PTR(rb_class_name(CLASS_OF(val))));
        return Qnil;
    }
    viewport.rColor = val;
    return self;
}

VALUE rb_Viewport_getVisible(VALUE self)
{
    auto& viewport = rb::Get<CViewport_Element>(self);
    return (viewport.getVisible() ? Qtrue : Qfalse);
}

VALUE rb_Viewport_setVisible(VALUE self, VALUE val)
{
    auto& viewport = rb::Get<CViewport_Element>(self);
    viewport.setVisible(RTEST(val));
    return self;
}

VALUE rb_Viewport_Update(VALUE self)
{
    auto& viewport = rb::Get<CViewport_Element>(self);
    return self;
}

VALUE rb_Viewport_getZ(VALUE self)
{
    auto& viewport = rb::Get<CViewport_Element>(self);
    return viewport.rZ;
}

VALUE rb_Viewport_setZ(VALUE self, VALUE val)
{
    auto& viewport = rb::Get<CViewport_Element>(self);
    rb_num2long(val);
    viewport.rZ = val;
    return self;
}

void Viewport_AdjustZoomAngle(CViewport_Element& viewport, VALUE rect)
{
	if (RDATA(rect)->data == nullptr)
		return;
	long x, y, width, height;
	CRect_Element* srect;
	Data_Get_Struct(rect, CRect_Element, srect);
	sf::IntRect& rc = srect->getRect();
	auto& view = viewport.getView();
	x = rc.left;
	y = rc.top;
	width = rc.width;
	height = rc.height;
	if (width & 1)
		width++;
	if (height & 1)
		height++;
	view.setCenter(static_cast<float>(viewport.getOx() + width / 2),
		static_cast<float>(viewport.getOy() + height / 2));
	view.setSize(static_cast<float>(width), static_cast<float>(height));
	view.setRotation(-NUM2DBL(viewport.rAngle));
	view.zoom(NUM2DBL(viewport.rZoom));
	float sw = static_cast<float>(ScreenWidth);
	float sh = static_cast<float>(ScreenHeight);
	sf::FloatRect frect(x / sw, y / sh, width / sw, height / sh);
	view.setViewport(frect);
}

VALUE rb_Viewport_getAngle(VALUE self)
{
	auto& viewport = rb::Get<CViewport_Element>(self);
	return viewport.rAngle;
}

VALUE rb_Viewport_setAngle(VALUE self, VALUE val)
{
	auto& viewport = rb::Get<CViewport_Element>(self);
	viewport.rAngle = LONG2NUM(NUM2LONG(val) % 360);
	Viewport_AdjustZoomAngle(viewport, viewport.rRect);
	return self;
}

VALUE rb_Viewport_getZoom(VALUE self)
{
	auto& viewport = rb::Get<CViewport_Element>(self);
	return viewport.rZoom;
}

VALUE rb_Viewport_setZoom(VALUE self, VALUE val)
{
	auto& viewport = rb::Get<CViewport_Element>(self);
	viewport.rZoom = DBL2NUM(1.0 / normalize_double(NUM2DBL(val), 0.001, 1000.0));
	Viewport_AdjustZoomAngle(viewport, viewport.rRect);
	return self;
}

VALUE rb_Viewport_getRenderState(VALUE self)
{
	auto& viewport = rb::Get<CViewport_Element>(self);
	return viewport.rRenderState;
}

VALUE rb_Viewport_setRenderState(VALUE self, VALUE val)
{
	sf::RenderStates* render_state;
	auto& viewport = rb::Get<CViewport_Element>(self);
	if (rb_obj_is_kind_of(val, rb_cBlendMode) == Qtrue)
	{
		Data_Get_Struct(val, sf::RenderStates, render_state);
		if (render_state)
		{
			viewport.setRenderStates(render_state);
			viewport.rRenderState = val;
			viewport.create_render(); // Make sure the global render is initialized
			viewport.updatetone();
			return self;
		}
	}
	viewport.setRenderStates(nullptr);
	if (viewport.rColor != Qnil && viewport.rTone != Qnil)
	{
		viewport.rRenderState = Qnil;
		viewport.create_render(); // Restore the old render
		viewport.updatetone();
	}
	else
		viewport.rRenderState = Qfalse; // False to prevent intempestive delete
	return self;
}

VALUE rb_Viewport_ReloadStack(VALUE self)
{
    auto& viewport = rb::Get<CViewport_Element>(self);
    VALUE table = rb_ivar_get(self, rb_iElementTable);
    rb_check_type(table, T_ARRAY);
    viewport.detachSprites();
    long sz = RARRAY_LEN(table);
    VALUE* ori = RARRAY_PTR(table);
    for(long i = 0; i < sz; i++)
    {
        if(rb_obj_is_kind_of(ori[i], rb_cViewport) == Qtrue ||
            rb_obj_is_kind_of(ori[i], rb_cSprite) == Qtrue ||
            rb_obj_is_kind_of(ori[i], rb_cText) == Qtrue ||
			rb_obj_is_kind_of(ori[i], rb_cShape) == Qtrue ||
			rb_obj_is_kind_of(ori[i], rb_cWindow) == Qtrue)
        {
            if(RDATA(ori[i])->data != nullptr)
            {
                viewport.bind(*reinterpret_cast<CDrawable_Element*>(RDATA(ori[i])->data));
            }
        }
    }
    return self;
}

VALUE rb_Viewport_Index(VALUE self)
{
    auto& viewport = rb::Get<CViewport_Element>(self);
    return rb_uint2inum(viewport.getIndex());
}

void Viewport_SetView(CViewport_Element& viewport, long x, long y, long width, long height)
{
    auto& view = viewport.getView();
    /* Adjustment for text */
    if(width & 1)
        width++;
    if(height & 1)
        height++;
    view.setCenter(static_cast<float>(viewport.getOx() + width / 2), 
                    static_cast<float>(viewport.getOy() + height / 2));
    view.setSize(static_cast<float>(width), static_cast<float>(height));
	view.setRotation(-NUM2DBL(viewport.rAngle));
	view.zoom(NUM2DBL(viewport.rZoom));
    float sw = static_cast<float>(ScreenWidth);
    float sh = static_cast<float>(ScreenHeight);
    sf::FloatRect frect(x / sw, y / sh, width / sw, height / sh);
    view.setViewport(frect);
    // viewport.reset_render();
}
