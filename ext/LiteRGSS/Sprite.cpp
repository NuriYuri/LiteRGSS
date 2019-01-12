#include "LiteRGSS.h"
#include "CBitmap_Element.h"
#include "CRect_Element.h"

VALUE rb_cSprite = Qnil;

template<>
void rb::Mark<CSprite_Element>(CSprite_Element* sprite)
{
    if(sprite == nullptr)
        return;
    rb_gc_mark(sprite->rViewport);
    rb_gc_mark(sprite->rBitmap);
    rb_gc_mark(sprite->rX);
    rb_gc_mark(sprite->rY);
    rb_gc_mark(sprite->rZ);
    rb_gc_mark(sprite->rOX);
    rb_gc_mark(sprite->rOY);
    rb_gc_mark(sprite->rAngle);
    rb_gc_mark(sprite->rZoomX);
    rb_gc_mark(sprite->rZoomY);
    rb_gc_mark(sprite->rRect);
}

void Init_Sprite() {
    rb_cSprite = rb_define_class_under(rb_mLiteRGSS, "Sprite", rb_cObject);
    rb_define_alloc_func(rb_cSprite, rb::AllocDrawable<CSprite_Element>);

    rb_define_method(rb_cSprite, "initialize", _rbf rb_Sprite_Initialize, -1);
    rb_define_method(rb_cSprite, "dispose", _rbf rb_Sprite_Dispose, 0);
    rb_define_method(rb_cSprite, "bitmap", _rbf rb_Sprite_getBitmap, 0);
    rb_define_method(rb_cSprite, "bitmap=", _rbf rb_Sprite_setBitmap, 1);
    rb_define_method(rb_cSprite, "x", _rbf rb_Sprite_getX, 0);
    rb_define_method(rb_cSprite, "x=", _rbf rb_Sprite_setX, 1);
    rb_define_method(rb_cSprite, "y", _rbf rb_Sprite_getY, 0);
    rb_define_method(rb_cSprite, "y=", _rbf rb_Sprite_setY, 1);
    rb_define_method(rb_cSprite, "z", _rbf rb_Sprite_getZ, 0);
    rb_define_method(rb_cSprite, "z=", _rbf rb_Sprite_setZ, 1);
    rb_define_method(rb_cSprite, "ox", _rbf rb_Sprite_getOX, 0);
    rb_define_method(rb_cSprite, "ox=", _rbf rb_Sprite_setOX, 1);
    rb_define_method(rb_cSprite, "oy", _rbf rb_Sprite_getOY, 0);
    rb_define_method(rb_cSprite, "oy=", _rbf rb_Sprite_setOY, 1);
    rb_define_method(rb_cSprite, "visible", _rbf rb_Sprite_getVisible, 0);
    rb_define_method(rb_cSprite, "visible=", _rbf rb_Sprite_setVisible, 1);
    rb_define_method(rb_cSprite, "angle", _rbf rb_Sprite_getAngle, 0);
    rb_define_method(rb_cSprite, "angle=", _rbf rb_Sprite_setAngle, 1);
    rb_define_method(rb_cSprite, "zoom_x", _rbf rb_Sprite_getZoomX, 0);
    rb_define_method(rb_cSprite, "zoom_x=", _rbf rb_Sprite_setZoomX, 1);
    rb_define_method(rb_cSprite, "zoom_y", _rbf rb_Sprite_getZoomY, 0);
    rb_define_method(rb_cSprite, "zoom_y=", _rbf rb_Sprite_setZoomY, 1);
    rb_define_method(rb_cSprite, "set_position", _rbf rb_Sprite_setPosition, 2);
    rb_define_method(rb_cSprite, "set_origin", _rbf rb_Sprite_setOrigin, 2);
    rb_define_method(rb_cSprite, "zoom=", _rbf rb_Sprite_setZoom, 1);
    rb_define_method(rb_cSprite, "opacity", _rbf rb_Sprite_getOpacity, 0);
    rb_define_method(rb_cSprite, "opacity=", _rbf rb_Sprite_setOpacity, 1);
    rb_define_method(rb_cSprite, "src_rect", _rbf rb_Sprite_getRect, 0);
    rb_define_method(rb_cSprite, "src_rect=", _rbf rb_Sprite_setRect, 1);
    rb_define_method(rb_cSprite, "disposed?", _rbf rb_Sprite_Disposed, 0);
    rb_define_method(rb_cSprite, "viewport", _rbf rb_Sprite_Viewport, 0);
	rb_define_method(rb_cSprite, "mirror", _rbf rb_Sprite_getMirror, 0);
	rb_define_method(rb_cSprite, "mirror=", _rbf rb_Sprite_setMirror, 1);
	rb_define_method(rb_cSprite, "width", _rbf rb_Sprite_width, 0);
	rb_define_method(rb_cSprite, "height", _rbf rb_Sprite_height, 0);
    rb_define_method(rb_cSprite, "__index__", _rbf rb_Sprite_Index, 0);

    rb_define_method(rb_cSprite, "clone", _rbf rb_Sprite_Copy, 0);
    rb_define_method(rb_cSprite, "dup", _rbf rb_Sprite_Copy, 0);
}

VALUE rb_Sprite_Initialize(int argc, VALUE* argv, VALUE self)
{
    auto& sprite = rb::Get<CSprite_Element>(self);
    VALUE table = Qnil;
    /* If a viewport was specified */
    if(argc == 1 && rb_obj_is_kind_of(argv[0], rb_cViewport) == Qtrue)
    {
        CViewport_Element* viewport;
        Data_Get_Struct(argv[0], CViewport_Element, viewport);
        viewport->bind(sprite);
        table = rb_ivar_get(argv[0], rb_iElementTable);
        sprite.rViewport = argv[0];
    }
	/* If a window is specified */
	else if (argc == 1 && rb_obj_is_kind_of(argv[0], rb_cWindow) == Qtrue)
	{
		auto& window = rb::GetSafe<CWindow_Element>(argv[0], rb_cWindow);
		window.bind(&sprite);
		table = rb_ivar_get(argv[0], rb_iElementTable);
		sprite.rViewport = argv[0];
		VALUE opacity = LONG2NUM(NUM2LONG(window.rOpacity) * NUM2LONG(window.rBackOpacity) / 255);
		rb_Sprite_setOpacity(self, opacity);
	}
    /* Otherwise */
    else
    {
        global_Graphics_Bind(&sprite);
        table = rb_ivar_get(rb_mGraphics, rb_iElementTable);
        sprite.rViewport = Qnil;
    }
    /* Ajout à la table de sauvegarde */
    rb_ary_push(table, self);
    /* Initializing Instance variables */
    sprite.rX = LONG2FIX(0);
    sprite.rY = LONG2FIX(0);
    sprite.rZ = LONG2FIX(0);
    sprite.rOX = LONG2FIX(0);
    sprite.rOY = LONG2FIX(0);
    sprite.rAngle = LONG2FIX(0);
    sprite.rZoomX = LONG2FIX(1);
    sprite.rZoomY = LONG2FIX(1);
    sprite.rBitmap = Qnil;
    sprite.rRect = Qnil;
	sprite.rMirror = Qfalse;
    return self;
}

VALUE rb_Sprite_Copy(VALUE self)
{
    rb_raise(rb_eRGSSError, "Sprites cannot be cloned or duplicated.");
    return self;
}

VALUE rb_Sprite_Dispose(VALUE self)
{
	return rb::Dispose<CSprite_Element>(self);
}

VALUE rb_Sprite_DisposeFromViewport(VALUE self)
{
    auto& sprite = rb::Get<CSprite_Element>(self);
	sprite.disposeFromViewport();
	return rb::Dispose<CSprite_Element>(self);
}

VALUE rb_Sprite_Disposed(VALUE self)
{
    rb_check_type(self, T_DATA);
    return (RDATA(self)->data == nullptr ? Qtrue : Qfalse);
}

VALUE rb_Sprite_setBitmap(VALUE self, VALUE bitmap)
{
    auto& sprite = rb::Get<CSprite_Element>(self);
    if(rb_obj_is_kind_of(bitmap, rb_cBitmap) == Qfalse)
    {
        if(bitmap == Qnil)
        {
            sprite.setDrawable(false);
            sprite.rBitmap = bitmap;
        }
        else
            rb_raise(rb_eTypeError, "Expected a Bitmap.");
        return self;
    }
    /* Bitmap Protection */
    if(RDATA(bitmap)->data == nullptr)
    {
        rb_raise(rb_eRGSSError, "Disposed Bitmap."); 
        return self;
    }
    /* Retreiving Bitmap Objecy */
    CBitmap_Element* bmp;
    Data_Get_Struct(bitmap, CBitmap_Element, bmp);
    sf::Sprite& sp = sprite.getSprite();
    sp.setTexture(bmp->getTexture(), true);
    sprite.setDrawable(true);
    sprite.rBitmap = bitmap;
	if (!NIL_P(sprite.rRect))
	{
		CRect_Element* rect;
		Data_Get_Struct(sprite.rRect, CRect_Element, rect);
		/* Setting rect parameter */
		const sf::IntRect rectorigin = sp.getTextureRect();
		rect_copy(&rect->getRect(), &rectorigin);
	}
    return self;
}

VALUE rb_Sprite_getBitmap(VALUE self)
{
    auto& sprite = rb::Get<CSprite_Element>(self);
    return sprite.rBitmap;
}

VALUE rb_Sprite_setX(VALUE self, VALUE val)
{
    auto& sprite = rb::Get<CSprite_Element>(self);
    sf::Sprite& sp = sprite.getSprite();
    const sf::Vector2f& vect = sp.getPosition();
    sp.setPosition(static_cast<float>(rb_num2long(val)), vect.y);
    sprite.rX = val;
    return val;
}

VALUE rb_Sprite_getX(VALUE self)
{
    auto& sprite = rb::Get<CSprite_Element>(self);
    return sprite.rX;
}

VALUE rb_Sprite_setY(VALUE self, VALUE val)
{
    auto& sprite = rb::Get<CSprite_Element>(self);
    sf::Sprite& sp = sprite.getSprite();
    const sf::Vector2f vect = sp.getPosition();
    sp.setPosition(vect.x, static_cast<float>(rb_num2long(val)));
    sprite.rY = val;
    return val;
}

VALUE rb_Sprite_getY(VALUE self)
{
    auto& sprite = rb::Get<CSprite_Element>(self);
    return sprite.rY;
}

VALUE rb_Sprite_setZ(VALUE self, VALUE val)
{
    auto& sprite = rb::Get<CSprite_Element>(self);
    rb_num2long(val);
    sprite.rZ = val;
    return val;
}

VALUE rb_Sprite_getZ(VALUE self)
{
    auto& sprite = rb::Get<CSprite_Element>(self);
    return sprite.rZ;
}

VALUE rb_Sprite_setOX(VALUE self, VALUE val)
{
    auto& sprite = rb::Get<CSprite_Element>(self);
    sf::Sprite& sp = sprite.getSprite();
    const sf::Vector2f vect = sp.getOrigin();
    sp.setOrigin(static_cast<float>(rb_num2long(val)), vect.y);
    sprite.rOX = val;
    return val;
}

VALUE rb_Sprite_getOX(VALUE self)
{
    auto& sprite = rb::Get<CSprite_Element>(self);
    return sprite.rOX;
}

VALUE rb_Sprite_setOY(VALUE self, VALUE val)
{
    auto& sprite = rb::Get<CSprite_Element>(self);
    sf::Sprite& sp = sprite.getSprite();
    const sf::Vector2f vect = sp.getOrigin();
    sp.setOrigin(vect.x, static_cast<float>(rb_num2long(val)));
    sprite.rOY = val;
    return val;
}

VALUE rb_Sprite_getOY(VALUE self)
{
    auto& sprite = rb::Get<CSprite_Element>(self);
    return sprite.rOY;
}

VALUE rb_Sprite_setVisible(VALUE self, VALUE val)
{
    auto& sprite = rb::Get<CSprite_Element>(self);
    sprite.setVisible(RTEST(val));
    return val;
}

VALUE rb_Sprite_getVisible(VALUE self)
{
    auto& sprite = rb::Get<CSprite_Element>(self);
    return sprite.getVisible() ? Qtrue : Qfalse;
}

VALUE rb_Sprite_setAngle(VALUE self, VALUE val)
{
    auto& sprite = rb::Get<CSprite_Element>(self);
    sprite.getSprite().setRotation(static_cast<float>(-NUM2DBL(val))); // RGSS rotation is trigo, SFML looks like anti-trigo
    sprite.rAngle = val;
    return val;
}

VALUE rb_Sprite_getAngle(VALUE self)
{
    auto& sprite = rb::Get<CSprite_Element>(self);
    return sprite.rAngle;
}

VALUE rb_Sprite_setZoomX(VALUE self, VALUE val)
{
    auto& sprite = rb::Get<CSprite_Element>(self);
    sf::Sprite& sp = sprite.getSprite();
    const sf::Vector2f vect = sp.getScale();
    sp.setScale(static_cast<float>(rb_num2dbl(val)), vect.y);
    sprite.rZoomX = val;
    return val;
}

VALUE rb_Sprite_getZoomX(VALUE self)
{
    auto& sprite = rb::Get<CSprite_Element>(self);
    return sprite.rZoomX;
}

VALUE rb_Sprite_setZoomY(VALUE self, VALUE val)
{
    auto& sprite = rb::Get<CSprite_Element>(self);
    sf::Sprite& sp = sprite.getSprite();
    const sf::Vector2f vect = sp.getScale();
    sp.setScale(vect.x, static_cast<float>(rb_num2dbl(val)));
    sprite.rZoomY = val;
    return val;
}

VALUE rb_Sprite_getZoomY(VALUE self)
{
    auto& sprite = rb::Get<CSprite_Element>(self);
    return sprite.rZoomY;
}


VALUE rb_Sprite_setPosition(VALUE self, VALUE x, VALUE y)
{
    auto& sprite = rb::Get<CSprite_Element>(self);
    sprite.getSprite().setPosition(static_cast<float>(rb_num2long(x)), static_cast<float>(rb_num2long(y)));
    sprite.rX = x;
    sprite.rY = y;
    return self;
}

VALUE rb_Sprite_setOrigin(VALUE self, VALUE x, VALUE y)
{
    auto& sprite = rb::Get<CSprite_Element>(self);
    sprite.getSprite().setOrigin(static_cast<float>(rb_num2long(x)), static_cast<float>(rb_num2long(y)));
    sprite.rOX = x;
    sprite.rOY = y;
    return self;
}

VALUE rb_Sprite_setZoom(VALUE self, VALUE zoom)
{
    auto& sprite = rb::Get<CSprite_Element>(self);
    float scale = static_cast<float>(rb_num2dbl(zoom));
    sprite.getSprite().setScale(scale, scale);
    sprite.rZoomX = zoom;
    sprite.rZoomY = zoom;
    return zoom;
}

VALUE rb_Sprite_setOpacity(VALUE self, VALUE val)
{
    auto& sprite = rb::Get<CSprite_Element>(self);
    sf::Sprite& sp = sprite.getSprite();
    const sf::Color col = sp.getColor();
    sp.setColor(sf::Color(col.r, col.g, col.b, normalize_long(rb_num2long(val), 0, 255)));
    return val;
}

VALUE rb_Sprite_getOpacity(VALUE self)
{
    auto& sprite = rb::Get<CSprite_Element>(self);
    sf::Sprite& sp = sprite.getSprite();
    const sf::Color col = sp.getColor();
    return rb_int2inum(col.a);
}

VALUE rb_Sprite_getRect(VALUE self)
{
    auto& sprite = rb::Get<CSprite_Element>(self);
    VALUE rc = sprite.rRect;
    if(!NIL_P(rc))
        return rc;
    /* Creating rect */
    VALUE argv[2];
    argv[0] = argv[1] = LONG2FIX(0);
    rc = rb_class_new_instance(2, argv, rb_cRect);
    /* Fetching data */
    CRect_Element* rect;
    Data_Get_Struct(rc, CRect_Element, rect);
    /* Setting rect parameter */
    const sf::IntRect rectorigin = sprite.getSprite().getTextureRect();
    rect_copy(&rect->getRect(), &rectorigin);
    /* Linking Rect */
	sprite.bindRect(rect);
    sprite.rRect = rc;
    return rc;
}

VALUE rb_Sprite_setRect(VALUE self, VALUE val)
{
    VALUE rc = rb_Sprite_getRect(self);
    if(RDATA(rc)->data == nullptr) { return Qnil; }
    if(rb_obj_is_kind_of(val, rb_cRect) != Qtrue)
    {
        rb_raise(rb_eTypeError, "Expected Rect got %s.", RSTRING_PTR(rb_class_name(CLASS_OF(val))));
        return Qnil;
    }
    if(RDATA(val)->data == nullptr) { return Qnil; }
    CSprite_Element* sprite;
    Data_Get_Struct(self, CSprite_Element, sprite);
    /* Getting data to update the rect */
    CRect_Element* rect1;
    Data_Get_Struct(val, CRect_Element, rect1);
    CRect_Element* rect2;
    Data_Get_Struct(rc, CRect_Element, rect2);
    /* Copying the rect */
    sf::IntRect& rect_target = rect2->getRect();
    rect_copy(&rect_target, &rect1->getRect());
	/* Mirror management */
	sf::IntRect rect_setter = rect_target;
	if (RTEST(sprite->rMirror))
	{
		rect_setter.left += rect_setter.width;
		rect_setter.width = -rect_setter.width;
	}
    /* Updating the texture rect */
    sprite->getSprite().setTextureRect(rect_setter);
    return val;
}

VALUE rb_Sprite_getMirror(VALUE self)
{
	auto& sprite = rb::Get<CSprite_Element>(self);
	return sprite.rMirror;
}

VALUE rb_Sprite_setMirror(VALUE self, VALUE val)
{
	auto& sprite = rb::Get<CSprite_Element>(self);
	sprite.rMirror = RTEST(val) ? Qtrue : Qfalse;
	rb_Sprite_setRect(self, rb_Sprite_getRect(self));
	return self;
}

VALUE rb_Sprite_Viewport(VALUE self)
{
    auto& sprite = rb::Get<CSprite_Element>(self);
    return sprite.rViewport;
}

VALUE rb_Sprite_Index(VALUE self)
{
    auto& sprite = rb::Get<CSprite_Element>(self);
    return rb_uint2inum(sprite.getIndex());
}

VALUE rb_Sprite_width(VALUE self)
{
	VALUE rc = rb_Sprite_getRect(self);
	return rb_Rect_getWidth(rc);
}

VALUE rb_Sprite_height(VALUE self)
{
	VALUE rc = rb_Sprite_getRect(self);
	return rb_Rect_getHeight(rc);
}
