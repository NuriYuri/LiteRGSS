#include "LiteRGSS.h"

VALUE rb_cText = Qnil;

template<>
void rb::Mark<CText_Element>(CText_Element* text)
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
    rb_gc_mark(text->rZ);
}

void Init_Text()
{
    rb_cText = rb_define_class_under(rb_mLiteRGSS, "Text", rb_cObject);

    rb_define_alloc_func(rb_cText, rb::Alloc<CText_Element>);

    rb_define_method(rb_cText, "initialize", _rbf rb_Text_Initialize, -1);
    rb_define_method(rb_cText, "dispose", _rbf rb_Text_Dispose, 0);
    rb_define_method(rb_cText, "disposed?", _rbf rb_Text_Disposed, 0);
    rb_define_method(rb_cText, "set_position", _rbf rb_Text_setPosition, 2);
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
    rb_define_method(rb_cText, "text", _rbf rb_Text_get_Text, 0);
    rb_define_method(rb_cText, "text=", _rbf rb_Text_set_Text, 1);
    rb_define_method(rb_cText, "visible", _rbf rb_Text_get_visible, 0);
    rb_define_method(rb_cText, "visible=", _rbf rb_Text_set_visible, 1);
    rb_define_method(rb_cText, "draw_shadow", _rbf rb_Text_get_draw_shadow, 0);
    rb_define_method(rb_cText, "draw_shadow=", _rbf rb_Text_set_draw_shadow, 1);
    rb_define_method(rb_cText, "nchar_draw", _rbf rb_Text_get_num_char, 0);
    rb_define_method(rb_cText, "nchar_draw=", _rbf rb_Text_set_num_char, 1);
    rb_define_method(rb_cText, "real_width", _rbf rb_Text_getRealWidth, 0);
    rb_define_method(rb_cText, "opacity", _rbf rb_Text_getOpacity, 0);
    rb_define_method(rb_cText, "opacity=", _rbf rb_Text_setOpacity, 1);
    rb_define_method(rb_cText, "text_width", _rbf rb_Text_get_text_width, 1);
    rb_define_method(rb_cText, "z", _rbf rb_Text_getZ, 0);
    rb_define_method(rb_cText, "z=", _rbf rb_Text_setZ, 1);
    rb_define_method(rb_cText, "__index__", _rbf rb_Text_Index, 0);
    rb_define_method(rb_cText, "viewport", _rbf rb_Text_Viewport, 0);
	rb_define_method(rb_cText, "italic", _rbf rb_Text_getItalic, 0);
	rb_define_method(rb_cText, "italic=", _rbf rb_Text_setItalic, 1);
	rb_define_method(rb_cText, "bold", _rbf rb_Text_getBold, 0);
	rb_define_method(rb_cText, "bold=", _rbf rb_Text_setBold, 1);

    rb_define_method(rb_cText, "clone", _rbf rb_Text_Copy, 0);
    rb_define_method(rb_cText, "dup", _rbf rb_Text_Copy, 0);
}

VALUE rb_Text_Initialize(int argc, VALUE* argv, VALUE self)
{
    auto& text = rb::Get<CText_Element>(self);
    VALUE fontid, viewport, x, y, width, height, str, align, outlinesize, table;
	VALUE opacity = LONG2NUM(255);
    rb_scan_args(argc, argv,"72", &fontid, &viewport, &x, &y, &width, &height, &str, &align, &outlinesize);
    /* Viewport */
    if(rb_obj_is_kind_of(viewport, rb_cViewport) == Qtrue)
    {
        text.rViewport = viewport;

        CViewport_Element* viewporte;
        Data_Get_Struct(viewport, CViewport_Element, viewporte);
        viewporte->bind(text);
        table = rb_ivar_get(viewport, rb_iElementTable);
    }
	/* If a window is specified */
	else if (rb_obj_is_kind_of(viewport, rb_cWindow) == Qtrue)
	{
		auto& window = rb::GetSafe<CWindow_Element>(viewport, rb_cWindow);
		window.bind(&text);
		table = rb_ivar_get(viewport, rb_iElementTable);
		text.rViewport = viewport;
		opacity = LONG2NUM(NUM2LONG(window.rOpacity) * NUM2LONG(window.rContentOpacity) / 255);
	}
    else
    {
        global_Graphics_Bind(&text);
        table = rb_ivar_get(rb_mGraphics, rb_iElementTable);
        text.rViewport = Qnil;
    }
    rb_ary_push(table, self);
    /* Surface */
    rb_check_type(x, T_FIXNUM);
    text.rX = x;
    rb_check_type(y, T_FIXNUM);
    text.rY = y;
    rb_check_type(width, T_FIXNUM);
    text.rwidth = width;
    rb_check_type(height, T_FIXNUM);
    text.rheight = height;
    /* Aligment */
    if(!NIL_P(align))
    {
        long ralign = rb_num2long(align);
        if(ralign <= 2 && ralign >= 0)
            text.rAlign = align;
    }
    /* Outline size */
    if(!NIL_P(outlinesize))
    {
        text.getText().setOutlineThickness(static_cast<float>(rb_num2dbl(outlinesize)));
    }
    /* Font */
    VALUE fcol = rb_Fonts_get_fill_color(rb_mFonts, fontid);
    if(rb_obj_is_kind_of(fcol, rb_cColor) == Qtrue)
        rb_Text_set_fill_color(self, fcol);
    VALUE ocol;
    if(text.getText().getOutlineThickness() < 1.0f)
        ocol = rb_Fonts_get_shadow_color(rb_mFonts, fontid);
    else
        ocol = rb_Fonts_get_outline_color(rb_mFonts, fontid);
    if(rb_obj_is_kind_of(ocol, rb_cColor) == Qtrue)
        rb_Text_set_outline_color(self, ocol);
    text.getText().setFont(rb_Fonts_get_font(rb_num2long(fontid)));
    /* Size */
    VALUE size = rb_Fonts_get_default_size(rb_mFonts, fontid);
    if(!NIL_P(size))
        text.getText().setCharacterSize(static_cast<unsigned int>(normalize_long(rb_num2long(size), 1, 0xFFFF)));
    /* Text */
    rb_Text_set_Text(self, str); /* Invokes rb_Text_UpdateI */
	rb_Text_setOpacity(self, opacity);
    return self;
}

VALUE rb_Text_Copy(VALUE self)
{
    rb_raise(rb_eRGSSError, "Text can not be cloned or duplicated.");
    return self;
}

VALUE rb_Text_Dispose(VALUE self)
{
    auto& text = rb::Get<CText_Element>(self);
    
    /* Suppression du sprite de ses stacks */
    VALUE viewport = text.rViewport;
    VALUE table;
    if(NIL_P(viewport))
        table = rb_ivar_get(rb_mGraphics, rb_iElementTable);
    else
        table = rb_ivar_get(viewport, rb_iElementTable);
    rb_ary_delete(table, self);
    text.setOriginStack(nullptr); // Ensure the text is removed from the sprite stack
    delete &text;
    RDATA(self)->data = nullptr;
    return self;
}

VALUE rb_Text_DisposeFromViewport(VALUE self)
{
    if(RDATA(self)->data == nullptr)
        return self;
    auto& text = rb::Get<CText_Element>(self);
    delete &text;
    RDATA(self)->data = nullptr;
    return self;
}

VALUE rb_Text_Disposed(VALUE self)
{
    rb_check_type(self, T_DATA);
    return (RDATA(self)->data == nullptr ? Qtrue : Qfalse);
}


VALUE rb_Text_setPosition(VALUE self, VALUE x, VALUE y)
{
    auto& text = rb::Get<CText_Element>(self);
    rb_check_type(x, T_FIXNUM);
    rb_check_type(y, T_FIXNUM);
    text.rX = x;
    text.rY = y;
    rb_Text_UpdateI(text);
    return self;
}

VALUE rb_Text_get_x(VALUE self)
{
    auto& text = rb::Get<CText_Element>(self);
    return text.rX;
}

VALUE rb_Text_set_x(VALUE self, VALUE val)
{
    auto& text = rb::Get<CText_Element>(self);
    rb_check_type(val, T_FIXNUM);
    text.rX = val;
    rb_Text_UpdateI(text);
    return val;
}

VALUE rb_Text_get_y(VALUE self)
{
    auto& text = rb::Get<CText_Element>(self);
    return text.rY;
}

VALUE rb_Text_set_y(VALUE self, VALUE val)
{
    auto& text = rb::Get<CText_Element>(self);
    rb_check_type(val, T_FIXNUM);
    text.rY = val;
    rb_Text_UpdateI(text);
    return val;
}

VALUE rb_Text_get_width(VALUE self)
{
    auto& text = rb::Get<CText_Element>(self);
    return text.rwidth;
}

VALUE rb_Text_set_width(VALUE self, VALUE val)
{
    rb_check_type(val, T_FIXNUM);
    auto& text = rb::Get<CText_Element>(self);
    text.rwidth = val;
    rb_Text_UpdateI(text);
    return val;
}

VALUE rb_Text_get_height(VALUE self)
{
    auto& text = rb::Get<CText_Element>(self);
    return text.rheight;
}

VALUE rb_Text_set_height(VALUE self, VALUE val)
{
    rb_check_type(val, T_FIXNUM);
    auto& text = rb::Get<CText_Element>(self);
    text.rheight = val;
    rb_Text_UpdateI(text);
    return val;
}

VALUE rb_Text_get_fill_color(VALUE self)
{
    auto& text = rb::Get<CText_Element>(self);
    VALUE argv[4];
    sf::Color col = text.getText().getFillColor();
    argv[0] = LONG2FIX(col.r);
    argv[1] = LONG2FIX(col.g);
    argv[2] = LONG2FIX(col.b);
    argv[3] = LONG2FIX(col.a);
    return rb_class_new_instance(4, argv, rb_cColor);
}

VALUE rb_Text_set_fill_color(VALUE self, VALUE val)
{
    auto& text = rb::Get<CText_Element>(self);
    if(rb_obj_is_kind_of(val, rb_cColor) == Qfalse)
        rb_raise(rb_eTypeError, "Expected Color got %s.", RSTRING_PTR(rb_class_name(CLASS_OF(val))));
    sf::Color* color;
    Data_Get_Struct(val, sf::Color, color);
    if(color == nullptr)
        return Qnil;
    text.getText().setFillColor(*color);
    return val;
}

VALUE rb_Text_get_outline_color(VALUE self)
{
    auto& text = rb::Get<CText_Element>(self);
    VALUE argv[4];
    sf::Color col = text.getText().getOutlineColor();
    argv[0] = LONG2FIX(col.r);
    argv[1] = LONG2FIX(col.g);
    argv[2] = LONG2FIX(col.b);
    argv[3] = LONG2FIX(col.a);
    return rb_class_new_instance(4, argv, rb_cColor);
}

VALUE rb_Text_set_outline_color(VALUE self, VALUE val)
{
    auto& text = rb::Get<CText_Element>(self);
    if(rb_obj_is_kind_of(val, rb_cColor) == Qfalse)
        rb_raise(rb_eTypeError, "Expected Color got %s.", RSTRING_PTR(rb_class_name(CLASS_OF(val))));
    sf::Color* color;
    Data_Get_Struct(val, sf::Color, color);
    if(color == nullptr)
        return Qnil;
    text.getText().setOutlineColor(*color);
    return val;
}

VALUE rb_Text_get_outline_thickness(VALUE self)
{
    auto& text = rb::Get<CText_Element>(self);
    return rb_float_new(static_cast<double>(text.getText().getOutlineThickness()));
}

VALUE rb_Text_set_outline_thickness(VALUE self, VALUE val)
{
    auto& text = rb::Get<CText_Element>(self);
    text.getText().setOutlineThickness(static_cast<float>(rb_num2dbl(val)));
    return val;
}

VALUE rb_Text_load_color(VALUE self, VALUE id)
{
    auto& text = rb::Get<CText_Element>(self);
    rb_Text_set_fill_color(self, rb_Fonts_get_fill_color(rb_mFonts, id));
    if(text.getText().getDrawShadow())
        rb_Text_set_outline_color(self, rb_Fonts_get_shadow_color(rb_mFonts, id));
    else
        rb_Text_set_outline_color(self, rb_Fonts_get_outline_color(rb_mFonts, id));
    return self;
}

VALUE rb_Text_get_align(VALUE self)
{
    auto& text = rb::Get<CText_Element>(self);
    return text.rAlign;
}

VALUE rb_Text_set_align(VALUE self, VALUE val)
{
    auto& text = rb::Get<CText_Element>(self);
    long align = rb_num2long(val);
    if(align == 1 || align == 2)
        text.rAlign = val;
    else
        text.rAlign = LONG2FIX(0);
    rb_Text_UpdateI(text);
    return self;
}

VALUE rb_Text_get_size(VALUE self)
{
    auto& text = rb::Get<CText_Element>(self);
    return rb_int2inum(static_cast<long>(text.getText().getCharacterSize()));
}

VALUE rb_Text_set_size(VALUE self, VALUE val)
{
    auto& text = rb::Get<CText_Element>(self);
    text.getText().setCharacterSize(static_cast<unsigned int>(normalize_long(rb_num2long(val), 1, 0xFFFF)));
    rb_Text_UpdateI(text);
    return self;
}

VALUE rb_Text_set_Text(VALUE self, VALUE str)
{
    auto& text = rb::Get<CText_Element>(self);
    rb_check_type(str, T_STRING);
    text.rtext = str;
    text.getText().setLineHeight(rb_num2dbl(text.rheight));
    std::string stru8(RSTRING_PTR(str));
    text.getText().setString(sf::String::fromUtf8(stru8.begin(), stru8.end()));
    rb_Text_UpdateI(text);
    return str;
}

VALUE rb_Text_get_text_width(VALUE self, VALUE val)
{
    auto& text = rb::Get<CText_Element>(self);
    rb_check_type(val, T_STRING);
    std::string stru8(RSTRING_PTR(val));
    sf::Uint32 width = text.getText().getTextWidth(sf::String::fromUtf8(stru8.begin(), stru8.end()));
    return RB_UINT2NUM(width);
}

VALUE rb_Text_get_Text(VALUE self)
{
    auto& text = rb::Get<CText_Element>(self);
    return text.rtext;
}

VALUE rb_Text_get_visible(VALUE self)
{
    auto& text = rb::Get<CText_Element>(self);
    return (text.getVisible() ? Qtrue : Qfalse);
}

VALUE rb_Text_set_visible(VALUE self, VALUE val)
{
    auto& text = rb::Get<CText_Element>(self);
    text.setVisible(RTEST(val));
    return self;
}

VALUE rb_Text_set_num_char(VALUE self, VALUE val)
{
    auto& text = rb::Get<CText_Element>(self);
    text.getText().setNumCharToDraw(rb_num2ulong(val));
    rb_Text_UpdateI(text);
    return self;
}

VALUE rb_Text_get_num_char(VALUE self)
{
    auto& text = rb::Get<CText_Element>(self);
    return RB_UINT2NUM(text.getText().getNumCharToDraw());
}

VALUE rb_Text_set_draw_shadow(VALUE self, VALUE val)
{
    auto& text = rb::Get<CText_Element>(self);
    text.getText().setDrawShadow(RTEST(val));
    return self;
}

VALUE rb_Text_get_draw_shadow(VALUE self)
{
    auto& text = rb::Get<CText_Element>(self);
    return (text.getText().getDrawShadow() ? Qtrue : Qfalse);
}

VALUE rb_Text_getRealWidth(VALUE self)
{
    auto& text = rb::Get<CText_Element>(self);
    return rb_int2inum(text.getText().getLocalBounds().width);
}

VALUE rb_Text_getOpacity(VALUE self)
{
    auto& text = rb::Get<CText_Element>(self);
    return rb_int2inum(text.getText().getFillColor().a);
}

VALUE rb_Text_setOpacity(VALUE self, VALUE val)
{
    auto& text = rb::Get<CText_Element>(self);
    long opacity = normalize_long(rb_num2long(val), 0, 255);
    auto& t = text.getText();
    sf::Color col = t.getFillColor();
    col.a = opacity;
    t.setFillColor(col);
    if(text.getText().getOutlineThickness() < 1.0f && opacity != 255)
    {
        col = t.getOutlineColor();
        col.a = opacity / 3;
        t.setOutlineColor(col);
    }
    return self;
}

VALUE rb_Text_getZ(VALUE self)
{
    auto& text = rb::Get<CText_Element>(self);
    return text.rZ;
}

VALUE rb_Text_setZ(VALUE self, VALUE val)
{
    auto& text = rb::Get<CText_Element>(self);
    rb_num2long(val);
    text.rZ = val;
    return self;
}

VALUE rb_Text_Index(VALUE self)
{
    auto& text = rb::Get<CText_Element>(self);
    return rb_uint2inum(text.getIndex());
}

VALUE rb_Text_Viewport(VALUE self)
{
    auto& text = rb::Get<CText_Element>(self);
    return text.rViewport;
}

VALUE rb_Text_getItalic(VALUE self)
{
	auto& text = rb::Get<CText_Element>(self);
	return ((text.getText().getStyle() & sf::Text2::Style::Italic) ? Qtrue : Qfalse);
}

VALUE rb_Text_setItalic(VALUE self, VALUE val)
{
	auto& text = rb::Get<CText_Element>(self);
	sf::Uint32 style = text.getText().getStyle();
	if (RTEST(val))
		style |= sf::Text2::Style::Italic;
	else
		style &= ~(sf::Text2::Style::Italic);
	text.getText().setStyle(style);
	return self;
}

VALUE rb_Text_getBold(VALUE self)
{
	auto& text = rb::Get<CText_Element>(self);
		return ((text.getText().getStyle() & sf::Text2::Style::Bold) ? Qtrue : Qfalse);
}

VALUE rb_Text_setBold(VALUE self, VALUE val)
{
	auto& text = rb::Get<CText_Element>(self);
		sf::Uint32 style = text.getText().getStyle();
	if (RTEST(val))
		style |= sf::Text2::Style::Bold;
	else
		style &= ~(sf::Text2::Style::Bold);
	text.getText().setStyle(style);
	return self;
}

VALUE rb_Text_UpdateI(CText_Element& text)
{
    sf::Text2& sftext = text.getText();
    float x, y, width, height, ox;
    long align;
    VALUE zero = LONG2FIX(0);
    x = rb_num2long(text.rX);
    y = rb_num2long(text.rY);
    height = rb_num2long(text.rheight);// / 2.0f;//static_cast<float>(rb_num2long(text.rheight)) / 2;
    sf::FloatRect bounds = sftext.getLocalBounds();
    align = rb_num2long(text.rAlign);
    width = rb_num2long(text.rwidth);
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
    y = round(y + height / 2);// + (height - bounds.height));
    /* Position update */
    sftext.setPosition(x, y);
    sftext.setOrigin(ox, round(sftext.getCharacterSize() / 2.0f));
    //std::cout << "(" << x << "," << y << ") / (" << ox << ", " << height << ") ->" << sftext->getCharacterSize() << " // " << bounds.height << std::endl;
    return Qnil;
}