#include "LiteRGSS.h"
#include "CViewport_Element.h"
#include "CRect_Element.h"
#include "CTone_Element.h"

VALUE rb_cViewport = Qnil;

#define VIEWPORT_PROTECT if(RDATA(self)->data == nullptr) \
{\
    rb_raise(rb_eRGSSError, "Disposed Viewport."); \
    return self; \
}

#define GET_VIEWPORT CViewport_Element* viewport; \
    Data_Get_Struct(self, CViewport_Element, viewport); \
    VIEWPORT_PROTECT

void rb_Viewport_Free(void* data)
{
    CViewport_Element* viewport = reinterpret_cast<CViewport_Element*>(data);
    if(viewport != nullptr)
    {
        viewport->setOriginStack(nullptr);
        CRect_Element* rect = viewport->getLinkedRect();
        if(rect != nullptr)
            rect->setElement(nullptr);
        CTone_Element* tone = viewport->getLinkedTone();
        if(tone != nullptr)
            tone->setElement(nullptr);
        delete viewport;
    }
}

void rb_Viewport_Mark(CViewport_Element* viewport)
{
    if(viewport == nullptr)
        return;
    rb_gc_mark(viewport->rRect);
    rb_gc_mark(viewport->rTone);
    rb_gc_mark(viewport->rColor);
}

VALUE rb_Viewport_Alloc(VALUE klass)
{
    CViewport_Element* viewport = new CViewport_Element();
    viewport->setLinkedRect(nullptr);
    viewport->setLinkedTone(nullptr);
    return Data_Wrap_Struct(klass, rb_Viewport_Mark, rb_Viewport_Free, viewport);
}

void Init_Viewport()
{
    rb_cViewport = rb_define_class_under(rb_mLiteRGSS, "Viewport", rb_cObject);

    rb_define_alloc_func(rb_cViewport, rb_Viewport_Alloc);

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
    GET_VIEWPORT
    VALUE table;
    global_Graphics_Bind(viewport);
    table = rb_ivar_get(rb_mGraphics, rb_iElementTable);
    rb_ary_push(table, self);
    viewport->setOx(0);
    viewport->setOy(0);
    Viewport_SetView(viewport, rb_num2long(x), rb_num2long(y), rb_num2long(width), rb_num2long(height));
    /* Creating rect */
    VALUE rc = rb_class_new_instance(argc, argv, rb_cRect);
    /* Fetching data */
    CRect_Element* rect;
    Data_Get_Struct(rc, CRect_Element, rect);
    rect->setElement(viewport);
    viewport->setLinkedRect(rect);
    viewport->rRect = rc;
    viewport->rTone = Qnil;
    viewport->rColor = Qnil;
    viewport->clearStack();
    return self;
}

void __Viewport_Dispose_AllSprite(VALUE table)
{
    rb_check_type(table, T_ARRAY);
    long sz = RARRAY_LEN(table);
    VALUE* ori = RARRAY_PTR(table);
    for(long i = 0; i < sz; i++)
    {
        if(rb_obj_is_kind_of(ori[i], rb_cSprite) == Qtrue)
            rb_Sprite_DisposeFromViewport(ori[i]);
        else
            rb_Text_DisposeFromViewport(ori[i]);
    }
    rb_ary_clear(table);
}

VALUE rb_Viewport_Dispose(VALUE self)
{
    GET_VIEWPORT
    RDATA(self)->data = nullptr;
    VALUE table = rb_ivar_get(rb_mGraphics, rb_iElementTable);
    rb_ary_delete(table, self);
    viewport->clearStack();
    __Viewport_Dispose_AllSprite(rb_ivar_get(self, rb_iElementTable));
    rb_Viewport_Free(reinterpret_cast<void*>(viewport));
    return self;
}

VALUE rb_Viewport_Disposed(VALUE self)
{
    rb_check_type(self, T_DATA);
    return (RDATA(self)->data == nullptr ? Qtrue : Qfalse);
}

void Viewport_AdjustOXY(CViewport_Element* viewport, VALUE rect)
{
    if(RDATA(rect)->data == nullptr)
        return;
    CRect_Element* srect;
    Data_Get_Struct(rect, CRect_Element, srect);
    sf::IntRect* rc = srect->getRect();
    sf::View* view = viewport->getView();
    view->setCenter(static_cast<float>(viewport->getOx() + rc->width / 2), 
                    static_cast<float>(viewport->getOy() + rc->height / 2));   
}

VALUE rb_Viewport_getOX(VALUE self)
{
    GET_VIEWPORT
    return rb_int2inum(viewport->getOx());
}

VALUE rb_Viewport_setOX(VALUE self, VALUE val)
{
    GET_VIEWPORT
    viewport->setOx(rb_num2long(val));
    Viewport_AdjustOXY(viewport, viewport->rRect);
    return val;
}

VALUE rb_Viewport_getOY(VALUE self)
{
    GET_VIEWPORT
    return rb_int2inum(viewport->getOx());
}

VALUE rb_Viewport_setOY(VALUE self, VALUE val)
{
    GET_VIEWPORT
    viewport->setOy(rb_num2long(val));
    Viewport_AdjustOXY(viewport, viewport->rRect);
    return val;
}

VALUE rb_Viewport_getRect(VALUE self)
{
    GET_VIEWPORT
    return viewport->rRect;
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
    sf::IntRect* rect_target = rect2->getRect();
    rect_copy(rect_target, rect1->getRect());
    /* Updating the viewport view */
    Viewport_SetView(viewport, rect_target->left, rect_target->top, rect_target->width, rect_target->height);
    return val;

}

VALUE rb_Viewport_getTone(VALUE self)
{
    GET_VIEWPORT
    VALUE tn = viewport->rTone;
    if(!NIL_P(tn))
        return tn;
    /* New tone */
    VALUE argv[4] = {LONG2FIX(0), LONG2FIX(0), LONG2FIX(0), LONG2FIX(0)};
    viewport->rColor = rb_class_new_instance(4, argv, rb_cColor);
    tn = rb_class_new_instance(4, argv, rb_cTone);
    CTone_Element* tone;
    Data_Get_Struct(tn, CTone_Element, tone);
    tone->setElement(viewport);
    viewport->setLinkedTone(tone);
    viewport->rTone = tn;
    viewport->create_render();
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
    GET_VIEWPORT
    CTone_Element* tonesrc;
    Data_Get_Struct(val, CTone_Element, tonesrc);
    CTone_Element* tonedest;
    Data_Get_Struct(tn, CTone_Element, tonedest);
    tone_copy(tonedest->getTone(), tonesrc->getTone());
    tone_copy(viewport->getTone(), tonesrc->getTone());
    return val;
}

VALUE rb_Viewport_getColor(VALUE self)
{
    VALUE tn = rb_Viewport_getTone(self);
    GET_VIEWPORT
    return viewport->rColor;
}

VALUE rb_Viewport_setColor(VALUE self, VALUE val)
{
    VALUE tn = rb_Viewport_getTone(self);
    GET_VIEWPORT
    if(rb_obj_is_kind_of(val, rb_cColor) != Qtrue)
    {
        rb_raise(rb_eTypeError, "Expected Color, got %s", RSTRING_PTR(rb_class_name(CLASS_OF(val))));
        return Qnil;
    }
    viewport->rColor = val;
    return self;
}

VALUE rb_Viewport_getVisible(VALUE self)
{
    GET_VIEWPORT
    return (viewport->getVisible() ? Qtrue : Qfalse);
}

VALUE rb_Viewport_setVisible(VALUE self, VALUE val)
{
    GET_VIEWPORT
    viewport->setVisible(RTEST(val));
    return self;
}

VALUE rb_Viewport_Update(VALUE self)
{
    GET_VIEWPORT
    return self;
}

void Viewport_SetView(CViewport_Element* viewport, long x, long y, long width, long height)
{
    sf::View* view = viewport->getView();
    view->setCenter(static_cast<float>(viewport->getOx() + width / 2), 
                    static_cast<float>(viewport->getOy() + height / 2));
    view->setSize(static_cast<float>(width), static_cast<float>(height));
    float sw = static_cast<float>(ScreenWidth);
    float sh = static_cast<float>(ScreenHeight);
    sf::FloatRect frect(x / sw, y / sh, width / sw, height / sh);
    view->setViewport(frect);
    viewport->reset_render();
}
