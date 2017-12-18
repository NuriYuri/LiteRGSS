#include "ruby.h"
#include "LiteRGSS.h"
#include "Fonts.h"
#include "Text.h"
#include <cmath>

VALUE rb_cText = Qnil;

#define TEXT_PROTECT if(RDATA(self)->data == nullptr) \
{\
    rb_raise(rb_eRGSSError, "Disposed Text."); \
    return self; \
}

#define GET_TEXT CText_Element* text; \
    Data_Get_Struct(self, CText_Element, text); \
    TEXT_PROTECT


void rb_Text_Free(void* data)
{
    CText_Element* text = reinterpret_cast<CText_Element*>(data);
    if(text)
    {
        text->setOriginStack(nullptr);
        delete text;
    }
}

void rb_Text_Mark(CText_Element* text)
{
    if(text == nullptr)
        return;
    rb_gc_mark(text->rViewport);
    rb_gc_mark(text->rwidth);
    rb_gc_mark(text->rheight);
    rb_gc_mark(text->rtext);
    rb_gc_mark(text->rAlign);
    rb_gc_mark(text->rX);
    rb_gc_mark(text->rY);
}

VALUE rb_Text_Alloc(VALUE klass)
{
    CText_Element* text = new CText_Element();
    text->rAlign = LONG2FIX(0);
    return Data_Wrap_Struct(klass, rb_Text_Mark, rb_Text_Free, text);
}

void Init_Text()
{
    rb_cText = rb_define_class_under(rb_mLiteRGSS, "Text", rb_cObject);

    rb_define_alloc_func(rb_cText, rb_Text_Alloc);

    rb_define_method(rb_cText, "initialize", _rbf rb_Text_Initialize, -1);
    rb_define_method(rb_cText, "dispose", _rbf rb_Text_Dispose, 0);
    rb_define_method(rb_cText, "disposed?", _rbf rb_Text_Disposed, 0);
    rb_define_method(rb_cText, "x", _rbf rb_Text_get_x, 0);
    rb_define_method(rb_cText, "x=", _rbf rb_Text_set_x, 1);
    rb_define_method(rb_cText, "y", _rbf rb_Text_get_y, 0);
    rb_define_method(rb_cText, "y=", _rbf rb_Text_set_y, 1);
    rb_define_method(rb_cText, "width", _rbf rb_Text_get_width, 0);
    rb_define_method(rb_cText, "width=", _rbf rb_Text_set_width, 1);
    rb_define_method(rb_cText, "height", _rbf rb_Text_get_height, 0);
    rb_define_method(rb_cText, "height=", _rbf rb_Text_set_height, 1);
    rb_define_method(rb_cText, "fill_color", _rbf rb_Text_get_fill_color, 0);
    rb_define_method(rb_cText, "fill_color=", _rbf rb_Text_set_fill_color, 1);
    rb_define_method(rb_cText, "outline_color", _rbf rb_Text_get_outline_color, 0);
    rb_define_method(rb_cText, "outline_color=", _rbf rb_Text_set_outline_color, 1);
    rb_define_method(rb_cText, "outline_thickness", _rbf rb_Text_get_outline_thickness, 0);
    rb_define_method(rb_cText, "outline_thickness=", _rbf rb_Text_set_outline_thickness, 1);
    rb_define_method(rb_cText, "load_color", _rbf rb_Text_load_color, 1);
    rb_define_method(rb_cText, "align", _rbf rb_Text_get_align, 0);
    rb_define_method(rb_cText, "align=", _rbf rb_Text_set_align, 1);
    rb_define_method(rb_cText, "size", _rbf rb_Text_get_size, 0);
    rb_define_method(rb_cText, "size=", _rbf rb_Text_set_size, 1);
    rb_define_method(rb_cText, "set_size", _rbf rb_Text_set_size, 1);
    rb_define_method(rb_cText, "text=", _rbf rb_Text_set_Text, 1);

    rb_define_method(rb_cText, "clone", _rbf rb_Text_Copy, 0);
    rb_define_method(rb_cText, "dup", _rbf rb_Text_Copy, 0);
}

VALUE rb_Text_Initialize(int argc, VALUE* argv, VALUE self)
{
    GET_TEXT
    VALUE fontid, viewport, x, y, width, height, str, align, outlinesize, table;
    rb_scan_args(argc, argv,"72", &fontid, &viewport, &x, &y, &width, &height, &str, &align, &outlinesize);
    /* Viewport */
    if(rb_obj_is_kind_of(viewport, rb_cViewport) == Qtrue)
    {
        text->rViewport = viewport;

        CViewport_Element* viewporte;
        Data_Get_Struct(viewport, CViewport_Element, viewporte);
        viewporte->bind(text);
        table = rb_ivar_get(viewport, rb_iElementTable);
    }
    else
    {
        __Graphics_Bind(text);
        table = rb_ivar_get(rb_mGraphics, rb_iElementTable);
        text->rViewport = Qnil;
    }
    rb_ary_push(table, self);
    /* Font */
    VALUE fcol = rb_Fonts_get_fill_color(rb_mFonts, fontid);
    if(rb_obj_is_kind_of(fcol, rb_cColor) == Qtrue)
        rb_Text_set_fill_color(self, fcol);
    VALUE ocol = rb_Fonts_get_outline_color(rb_mFonts, fontid);
    if(rb_obj_is_kind_of(ocol, rb_cColor) == Qtrue)
        rb_Text_set_outline_color(self, ocol);
    text->getText()->setFont(rb_Fonts_get_font(rb_num2long(fontid)));
    /* Size */
    VALUE size = rb_Fonts_get_default_size(rb_mFonts, fontid);
    if(!NIL_P(size))
        text->getText()->setCharacterSize(static_cast<unsigned int>(normalize_long(rb_num2long(size), 1, 0xFFFF)));
    /* Surface */
    rb_check_type(x, T_FIXNUM);
    text->rX = x;
    rb_check_type(y, T_FIXNUM);
    text->rY = y;
    rb_check_type(width, T_FIXNUM);
    text->rwidth = width;
    rb_check_type(height, T_FIXNUM);
    text->rheight = height;
    /* Aligment */
    if(!NIL_P(align))
    {
        long ralign = rb_num2long(align);
        if(ralign <= 2 && ralign >= 0)
            text->rAlign = align;
    }
    /* Outline size */
    if(!NIL_P(outlinesize))
    {
        text->getText()->setOutlineThickness(static_cast<float>(rb_num2dbl(outlinesize)));
    }
    /* Text */
    rb_Text_set_Text(self, str); /* Invokes rb_Text_UpdateI */
    return self;
}

VALUE rb_Text_Copy(VALUE self)
{
    rb_raise(rb_eRGSSError, "Text can not be cloned or duplicated.");
    return self;
}

VALUE rb_Text_Dispose(VALUE self)
{
    GET_TEXT
    RDATA(self)->data = nullptr;
    /* Suppression du sprite de ses stacks */
    VALUE viewport = text->rViewport;
    VALUE table;
    if(NIL_P(viewport))
        table = rb_ivar_get(rb_mGraphics, rb_iElementTable);
    else
        table = rb_ivar_get(viewport, rb_iElementTable);
    rb_ary_delete(table, self);
    rb_Text_Free(reinterpret_cast<void*>(text));
    return self;
}

VALUE rb_Text_DisposeFromViewport(VALUE self)
{
    GET_TEXT
    RDATA(self)->data = nullptr;
    rb_Text_Free(reinterpret_cast<void*>(text));
    return self;
}

VALUE rb_Text_Disposed(VALUE self)
{
    rb_check_type(self, T_DATA);
    return (RDATA(self)->data == nullptr ? Qtrue : Qfalse);
}

VALUE rb_Text_get_x(VALUE self)
{
    GET_TEXT
    return text->rX;
}

VALUE rb_Text_set_x(VALUE self, VALUE val)
{
    GET_TEXT
    rb_check_type(val, T_FIXNUM);
    text->rX = val;
    rb_Text_UpdateI(text);
    return val;
}

VALUE rb_Text_get_y(VALUE self)
{
    GET_TEXT
    return text->rY;
}

VALUE rb_Text_set_y(VALUE self, VALUE val)
{
    GET_TEXT
    rb_check_type(val, T_FIXNUM);
    text->rY = val;
    rb_Text_UpdateI(text);
    return val;
}

VALUE rb_Text_get_width(VALUE self)
{
    GET_TEXT
    return text->rwidth;
}

VALUE rb_Text_set_width(VALUE self, VALUE val)
{
    rb_check_type(val, T_FIXNUM);
    GET_TEXT
    text->rwidth = val;
    rb_Text_UpdateI(text);
    return val;
}

VALUE rb_Text_get_height(VALUE self)
{
    GET_TEXT
    return text->rheight;
}

VALUE rb_Text_set_height(VALUE self, VALUE val)
{
    rb_check_type(val, T_FIXNUM);
    GET_TEXT
    text->rheight = val;
    rb_Text_UpdateI(text);
    return val;
}

VALUE rb_Text_get_fill_color(VALUE self)
{
    GET_TEXT
    VALUE argv[4];
    sf::Color col = text->getText()->getFillColor();
    argv[0] = LONG2FIX(col.r);
    argv[1] = LONG2FIX(col.g);
    argv[2] = LONG2FIX(col.b);
    argv[3] = LONG2FIX(col.a);
    return rb_class_new_instance(4, argv, rb_cColor);
}

VALUE rb_Text_set_fill_color(VALUE self, VALUE val)
{
    GET_TEXT
    if(rb_obj_is_kind_of(val, rb_cColor) == Qfalse)
        rb_raise(rb_eTypeError, "Expected Color got %s.", RSTRING_PTR(rb_class_name(CLASS_OF(val))));
    sf::Color* color;
    Data_Get_Struct(val, sf::Color, color);
    if(color == nullptr)
        return Qnil;
    text->getText()->setFillColor(*color);
    return val;
}

VALUE rb_Text_get_outline_color(VALUE self)
{
    GET_TEXT
    VALUE argv[4];
    sf::Color col = text->getText()->getOutlineColor();
    argv[0] = LONG2FIX(col.r);
    argv[1] = LONG2FIX(col.g);
    argv[2] = LONG2FIX(col.b);
    argv[3] = LONG2FIX(col.a);
    return rb_class_new_instance(4, argv, rb_cColor);
}

VALUE rb_Text_set_outline_color(VALUE self, VALUE val)
{
    GET_TEXT
    if(rb_obj_is_kind_of(val, rb_cColor) == Qfalse)
        rb_raise(rb_eTypeError, "Expected Color got %s.", RSTRING_PTR(rb_class_name(CLASS_OF(val))));
    sf::Color* color;
    Data_Get_Struct(val, sf::Color, color);
    if(color == nullptr)
        return Qnil;
    text->getText()->setOutlineColor(*color);
    return val;
}

VALUE rb_Text_get_outline_thickness(VALUE self)
{
    GET_TEXT
    return rb_float_new(static_cast<double>(text->getText()->getOutlineThickness()));
}

VALUE rb_Text_set_outline_thickness(VALUE self, VALUE val)
{
    GET_TEXT
    text->getText()->setOutlineThickness(static_cast<float>(rb_num2dbl(val)));
    return val;
}

VALUE rb_Text_load_color(VALUE self, VALUE id)
{
    rb_Text_set_fill_color(self, rb_Fonts_get_fill_color(rb_mFonts, id));
    rb_Text_set_outline_color(self, rb_Fonts_get_outline_color(rb_mFonts, id));
    return self;
}

VALUE rb_Text_get_align(VALUE self)
{
    GET_TEXT
    return text->rAlign;
}

VALUE rb_Text_set_align(VALUE self, VALUE val)
{
    GET_TEXT
    long align = rb_num2long(val);
    if(align == 1 || align == 2)
        text->rAlign = val;
    else
        text->rAlign = LONG2FIX(0);
    rb_Text_UpdateI(text);
}

VALUE rb_Text_get_size(VALUE self)
{
    GET_TEXT
    return rb_int2inum(static_cast<long>(text->getText()->getCharacterSize()));
}

VALUE rb_Text_set_size(VALUE self, VALUE val)
{
    GET_TEXT
    text->getText()->setCharacterSize(static_cast<unsigned int>(normalize_long(rb_num2long(val), 1, 0xFFFF)));
    rb_Text_UpdateI(text);
    return self;
}

VALUE rb_Text_set_Text(VALUE self, VALUE str)
{
    GET_TEXT
    rb_check_type(str, T_STRING);
    text->rtext = str;
    std::string stru8(RSTRING_PTR(str));
    text->getText()->setString(sf::String::fromUtf8(stru8.begin(), stru8.end()));
    rb_Text_UpdateI(text);
    return str;
}

VALUE rb_Text_UpdateI(CText_Element* text)
{
    sf::Text* sftext = text->getText();
    float x, y, width, height, ox;
    long align;
    VALUE zero = LONG2FIX(0);
    x = rb_num2long(text->rX);
    y = rb_num2long(text->rY);
    sf::FloatRect bounds = sftext->getLocalBounds();
    align = rb_num2long(text->rAlign);
    width = rb_num2long(text->rwidth);
    height = static_cast<float>(rb_num2long(text->rheight)) / 2;
    switch(align)
    {
        case 1: /* Center */
            ox = round(bounds.width / 2);
            x = round(x + width / 2);
            break;
        case 2: /* Right */
            ox = round(bounds.width);
            x = round(x + width);
            break;
        default: /* Left */
            ox = 0.0f;
    }
    y = round(y + height);
    /* Position update */
    sftext->setPosition(x, y);
    sftext->setOrigin(ox, round(height));
    //std::cout << "(" << x << "," << y << ") / (" << ox << ", " << height << ") ->" << sftext->getCharacterSize() << " // " << bounds.height << std::endl;
    return Qnil;
}