#include "ruby.h"
#include "LiteRGSS.h"
#include "CViewport_Element.h"
#include "CRect_Element.h"
#include "Graphics.h"
#include "Sprite.h"
#include "Viewport.h"

VALUE rb_cViewport = Qnil;
ID rb_Viewport_ivRect = Qnil;


#define VIEWPORT_PROTECT if(RDATA(self)->data == nullptr) \
{\
    rb_raise(rb_eRGSSError, "Disposed Viewport."); \
    return self; \
}

void rb_Viewport_Free(void* data)
{
    CViewport_Element* viewport = reinterpret_cast<CViewport_Element*>(data);
    if(viewport)
    {
        viewport->setOriginStack(nullptr);
        delete viewport;
    }
}

VALUE rb_Viewport_Alloc(VALUE klass)
{
    CViewport_Element* viewport = new CViewport_Element();
    return Data_Wrap_Struct(klass, NULL, rb_Viewport_Free, viewport);
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

    rb_define_method(rb_cViewport, "clone", _rbf rb_Viewport_Copy, 0);
    rb_define_method(rb_cViewport, "dup", _rbf rb_Viewport_Copy, 0);

    rb_Viewport_ivRect = rb_intern("@rect");
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
    CViewport_Element* viewport;
    Data_Get_Struct(self, CViewport_Element, viewport);
    VALUE table;
    __Graphics_Bind(viewport);
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
    rb_ivar_set(self, rb_Viewport_ivRect, rc);
    return self;
}

void __Viewport_Dispose_AllSprite(VALUE table)
{
    rb_check_type(table, T_ARRAY);
    long sz = RARRAY_LEN(table);
    VALUE* ori = RARRAY_PTR(table);
    VALUE* copy = new VALUE[sz];
    for(long i = 0; i < sz; i++) // To prevent data conflict with rb_ary_delete()
        copy[i] = ori[i];
    for(long i = 0; i < sz; i++) // To dispose the sprites
        rb_Sprite_Dispose(copy[i]);
    delete[] copy;
}

VALUE rb_Viewport_Dispose(VALUE self)
{
    VIEWPORT_PROTECT
    CViewport_Element* viewport;
    Data_Get_Struct(self, CViewport_Element, viewport);
    RDATA(self)->data = nullptr;
    VALUE table = rb_ivar_get(rb_mGraphics, rb_iElementTable);
    rb_ary_delete(table, self);
    viewport->setOriginStack(nullptr);
    viewport->clearStack();
    __Viewport_Dispose_AllSprite(rb_ivar_get(self, rb_iElementTable));
    delete viewport;
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
    VIEWPORT_PROTECT
    CViewport_Element* viewport;
    Data_Get_Struct(self, CViewport_Element, viewport);
    return rb_int2inum(viewport->getOx());
}

VALUE rb_Viewport_setOX(VALUE self, VALUE val)
{
    VIEWPORT_PROTECT
    CViewport_Element* viewport;
    Data_Get_Struct(self, CViewport_Element, viewport);
    viewport->setOx(rb_num2long(val));
    Viewport_AdjustOXY(viewport, rb_ivar_get(self, rb_Viewport_ivRect));
    return val;
}

VALUE rb_Viewport_getOY(VALUE self)
{
    VIEWPORT_PROTECT
    CViewport_Element* viewport;
    Data_Get_Struct(self, CViewport_Element, viewport);
    return rb_int2inum(viewport->getOx());
}

VALUE rb_Viewport_setOY(VALUE self, VALUE val)
{
    VIEWPORT_PROTECT
    CViewport_Element* viewport;
    Data_Get_Struct(self, CViewport_Element, viewport);
    viewport->setOy(rb_num2long(val));
    Viewport_AdjustOXY(viewport, rb_ivar_get(self, rb_Viewport_ivRect));
    return val;
}

VALUE rb_Viewport_getRect(VALUE self)
{
    VIEWPORT_PROTECT
    return rb_ivar_get(self, rb_Viewport_ivRect);
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
}