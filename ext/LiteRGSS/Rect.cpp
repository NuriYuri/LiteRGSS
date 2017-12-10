#include "ruby.h"
#include "LiteRGSS.h"
#include "CRect_Element.h"
#include "CViewport_Element.h"
#include "Rect.h"
#include "Viewport.h"
#include <string>

VALUE rb_cRect = Qnil;

#define RECT_PROTECT if(RDATA(self)->data == nullptr) { return Qnil; }
void __Rect_Check_LinkedObject(CRect_Element* rect);


void rb_Rect_Free(void* data)
{
    CRect_Element* rect = reinterpret_cast<CRect_Element*>(data);
    if(rect)
    {
        delete rect;
    }
}

VALUE rb_Rect_Alloc(VALUE klass)
{
    CRect_Element* rect = new CRect_Element();
    return Data_Wrap_Struct(klass, NULL, rb_Rect_Free, rect);
}

void Init_Rect()
{
    rb_cRect = rb_define_class_under(rb_mLiteRGSS, "Rect", rb_cObject);

    rb_define_alloc_func(rb_cRect, rb_Rect_Alloc);

    rb_define_method(rb_cRect, "initialize", _rbf rb_Rect_initialize, -1);
    rb_define_method(rb_cRect, "initialize_copy", _rbf rb_Rect_initialize_copy, 1);
    rb_define_method(rb_cRect, "set", _rbf rb_Rect_set, -1);
    rb_define_method(rb_cRect, "x", _rbf rb_Rect_getX, 0);
    rb_define_method(rb_cRect, "x=", _rbf rb_Rect_setX, 1);
    rb_define_method(rb_cRect, "y", _rbf rb_Rect_getY, 0);
    rb_define_method(rb_cRect, "y=", _rbf rb_Rect_setY, 1);
    rb_define_method(rb_cRect, "width", _rbf rb_Rect_getWidth, 0);
    rb_define_method(rb_cRect, "width=", _rbf rb_Rect_setWidth, 1);
    rb_define_method(rb_cRect, "height", _rbf rb_Rect_getHeight, 0);
    rb_define_method(rb_cRect, "height=", _rbf rb_Rect_setHeight, 1);
    rb_define_method(rb_cRect, "==", _rbf rb_Rect_eql, 1);
    rb_define_method(rb_cRect, "eql?", _rbf rb_Rect_eql, 1);
    rb_define_method(rb_cRect, "to_s", _rbf rb_Rect_to_s, 0);
    rb_define_method(rb_cRect, "inspect", _rbf rb_Rect_to_s, 0);
    rb_define_method(rb_cRect, "empty", _rbf rb_Rect_empty, 0);
    rb_define_method(rb_cRect, "_dump", _rbf rb_Rect_save, 1);
    rb_define_singleton_method(rb_cRect, "_load", _rbf rb_Rect_load, 1);
}

/*
 * Initialisation :
 * Rect.new(width, height)
 * Rect.new(x, width, height)
 * Rect.new(x, y, width, height)
 */
VALUE rb_Rect_initialize(int argc, VALUE* argv, VALUE self)
{
    VALUE x, y, width, height;
    rb_scan_args(argc, argv, "22", &x, &y, &width, &height);
    CRect_Element* rect;
    Data_Get_Struct(self, CRect_Element, rect);
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
    /* Rect definition */
    sf::IntRect* srect = rect->getRect();
    srect->left = rb_num2long(x);
    srect->top = rb_num2long(y);
    srect->width = rb_num2long(width);
    srect->height = rb_num2long(height);
    /* Pointed element nullification */
    rect->setElement(nullptr);
    return self;
}

VALUE rb_Rect_initialize_copy(VALUE self, VALUE other)
{
    RECT_PROTECT
    rb_check_type(other, T_DATA);
    if(RDATA(other)->data == nullptr) { return Qnil; }
    CRect_Element* rect;
    Data_Get_Struct(self, CRect_Element, rect);
    CRect_Element* rect2;
    Data_Get_Struct(other, CRect_Element, rect2);
    rect_copy(rect->getRect(), rect2->getRect());
    rect2->setElement(nullptr);
    return self;
}
/*
 * rect.set(x)
 * rect.set(x,y)
 * rect.set(x,y,width)
 * rect.set(x,y,width,height)
 * rect.set(x, nil, width) etc...
 */
VALUE rb_Rect_set(int argc, VALUE* argv, VALUE self)
{
    RECT_PROTECT
    VALUE x, y, width, height;
    rb_scan_args(argc, argv, "13", &x, &y, &width, &height);
    CRect_Element* rect;
    Data_Get_Struct(self, CRect_Element, rect);
    sf::IntRect* srect = rect->getRect();
    if(!NIL_P(x))
        srect->left = rb_num2long(x);
    if(!NIL_P(y))
        srect->top = rb_num2long(y);
    if(!NIL_P(width))
        srect->width = rb_num2long(width);
    if(!NIL_P(height))
        srect->height = rb_num2long(height);
    __Rect_Check_LinkedObject(rect);
    return self;
}

VALUE rb_Rect_getX(VALUE self)
{
    RECT_PROTECT
    CRect_Element* rect;
    Data_Get_Struct(self, CRect_Element, rect);
    return rb_int2inum(rect->getRect()->left);
}

VALUE rb_Rect_setX(VALUE self, VALUE val)
{
    RECT_PROTECT
    CRect_Element* rect;
    Data_Get_Struct(self, CRect_Element, rect);
    rect->getRect()->left = rb_num2long(val);
    __Rect_Check_LinkedObject(rect);
    return val;
}

VALUE rb_Rect_getY(VALUE self)
{
    RECT_PROTECT
    CRect_Element* rect;
    Data_Get_Struct(self, CRect_Element, rect);
    return rb_int2inum(rect->getRect()->top);
}

VALUE rb_Rect_setY(VALUE self, VALUE val)
{
    RECT_PROTECT
    CRect_Element* rect;
    Data_Get_Struct(self, CRect_Element, rect);
    rect->getRect()->top = rb_num2long(val);
    __Rect_Check_LinkedObject(rect);
    return val;
}

VALUE rb_Rect_getWidth(VALUE self)
{
    RECT_PROTECT
    CRect_Element* rect;
    Data_Get_Struct(self, CRect_Element, rect);
    return rb_int2inum(rect->getRect()->width);
}

VALUE rb_Rect_setWidth(VALUE self, VALUE val)
{
    RECT_PROTECT
    CRect_Element* rect;
    Data_Get_Struct(self, CRect_Element, rect);
    rect->getRect()->width = rb_num2long(val);
    __Rect_Check_LinkedObject(rect);
    return val;
}

VALUE rb_Rect_getHeight(VALUE self)
{
    RECT_PROTECT
    CRect_Element* rect;
    Data_Get_Struct(self, CRect_Element, rect);
    return rb_int2inum(rect->getRect()->height);
}

VALUE rb_Rect_setHeight(VALUE self, VALUE val)
{
    RECT_PROTECT
    CRect_Element* rect;
    Data_Get_Struct(self, CRect_Element, rect);
    rect->getRect()->height = rb_num2long(val);
    __Rect_Check_LinkedObject(rect);
    return val;
}

VALUE rb_Rect_load(VALUE self, VALUE str)
{
    rb_check_type(str, T_STRING);
    VALUE arr[4];
    if(RSTRING_LEN(str) < (sizeof(int) * 4))
    {
        arr[1] = arr[0] = LONG2FIX(1);
        return rb_class_new_instance(2, arr, self);
    }
    int* rc = reinterpret_cast<int*>(RSTRING_PTR(str));
    arr[0] = LONG2FIX(rc[0]);
    arr[1] = LONG2FIX(rc[1]);
    arr[2] = LONG2FIX(rc[2]);
    arr[3] = LONG2FIX(rc[3]);
    return rb_class_new_instance(4, arr, self);
}

VALUE rb_Rect_save(VALUE self, VALUE limit)
{
    RECT_PROTECT
    CRect_Element* rect;
    Data_Get_Struct(self, CRect_Element, rect);
    sf::IntRect* srect = rect->getRect();
    int rc[4];
    rc[0] = srect->left;
    rc[1] = srect->top;
    rc[2] = srect->width;
    rc[3] = srect->height;
    return rb_str_new(reinterpret_cast<const char*>(rc), sizeof(int) * 4);
}

VALUE rb_Rect_eql_rect(CRect_Element* rect, VALUE self)
{
    RECT_PROTECT
    CRect_Element* rect2;
    Data_Get_Struct(self, CRect_Element, rect2);
    sf::IntRect* or1 = rect->getRect();
    sf::IntRect* or2 = rect2->getRect();
    if(or1->left != or2->left)
        return Qfalse;
    if(or1->top != or2->top)
        return Qfalse;
    if(or1->width != or2->width)
        return Qfalse;
    if(or1->height != or2->height)
        return Qfalse;
    return Qtrue;
}

VALUE rb_Rect_eql_array(CRect_Element* rect, VALUE oth)
{
    sf::IntRect* or1 = rect->getRect();
    if(RARRAY_LEN(oth) != 4)
        return Qfalse;
    VALUE* arr = RARRAY_PTR(oth);
    if(rb_num2long(arr[0]) != or1->left)
        return Qfalse;
    if(rb_num2long(arr[1]) != or1->top)
        return Qfalse;
    if(rb_num2long(arr[2]) != or1->width)
        return Qfalse;
    if(rb_num2long(arr[3]) != or1->height)
        return Qfalse;
    return Qtrue;
}

VALUE rb_Rect_eql(VALUE self, VALUE other)
{
    RECT_PROTECT
    CRect_Element* rect;
    Data_Get_Struct(self, CRect_Element, rect);
    if(rb_obj_is_kind_of(other, rb_cRect) == Qtrue)
    {
        return rb_Rect_eql_rect(rect, other);
    }
    else if(rb_obj_is_kind_of(other, rb_cArray) == Qtrue)
    {
        return rb_Rect_eql_array(rect, other);
    }
    return Qfalse;
}

VALUE rb_Rect_empty(VALUE self)
{
    RECT_PROTECT
    CRect_Element* rect;
    Data_Get_Struct(self, CRect_Element, rect);
    sf::IntRect* srect = rect->getRect();
    srect->left = srect->top = srect->width = srect->height = 0;
    return self;
}

VALUE rb_Rect_to_s(VALUE self)
{
    RECT_PROTECT
    CRect_Element* rect;
    Data_Get_Struct(self, CRect_Element, rect);
    sf::IntRect* srect = rect->getRect();
    return rb_sprintf("(%d, %d, %d, %d)", srect->left, srect->top, srect->width, srect->height);
}

void __Rect_Check_LinkedObject(CRect_Element* rect)
{
    CDrawable_Element* el = rect->getElement();
    if(el == nullptr)
        return;
    /* Viewport processing */
    if(el->isViewport())
    {
        sf::IntRect* srect = rect->getRect();
        Viewport_SetView(reinterpret_cast<CViewport_Element*>(el), 
            srect->left, srect->top, srect->width, srect->height);
    }
    /* Sprite Processing */
    else if(el->isPureSprite())
    {
        reinterpret_cast<CSprite_Element*>(el)->getSprite()->setTextureRect(*rect->getRect());
    }
    /* Window Processing */
    else
    {

    }
}