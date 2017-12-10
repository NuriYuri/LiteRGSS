#include "ruby.h"
#include "LiteRGSS.h"
#include "Sprite.h"
#include "CBitmap_Element.h"
#include "CRect_Element.h"

VALUE rb_cSprite = Qnil;
ID rb_Sprite_ivViewport = Qnil;
ID rb_Sprite_ivBitmap = Qnil;
ID rb_Sprite_ivX = Qnil;
ID rb_Sprite_ivY = Qnil;
ID rb_Sprite_ivOX = Qnil;
ID rb_Sprite_ivOY = Qnil;
ID rb_Sprite_ivAngle = Qnil;
ID rb_Sprite_ivZoomX = Qnil;
ID rb_Sprite_ivZoomY = Qnil;
ID rb_Sprite_ivRect = Qnil;

#define SPRITE_PROTECT if(RDATA(self)->data == nullptr) \
{\
    rb_raise(rb_eRGSSError, "Disposed Sprite."); \
    return self; \
}

void rb_Sprite_Free(void* data)
{
    CSprite_Element* sprite = reinterpret_cast<CSprite_Element*>(data);
    if(sprite)
    {
        sprite->setOriginStack(nullptr);
        delete sprite;
    }
}

VALUE rb_Sprite_Alloc(VALUE klass)
{
    CSprite_Element* sprite = new CSprite_Element();
    return Data_Wrap_Struct(klass, NULL, rb_Sprite_Free, sprite);
}

void Init_Sprite() {
    rb_cSprite = rb_define_class_under(rb_mLiteRGSS, "Sprite", rb_cObject);
    rb_define_alloc_func(rb_cSprite, rb_Sprite_Alloc);

    rb_define_method(rb_cSprite, "initialize", _rbf rb_Sprite_Initialize, -1);
    rb_define_method(rb_cSprite, "dispose", _rbf rb_Sprite_Dispose, 0);
    rb_define_method(rb_cSprite, "bitmap", _rbf rb_Sprite_getBitmap, 0);
    rb_define_method(rb_cSprite, "bitmap=", _rbf rb_Sprite_setBitmap, 1);
    rb_define_method(rb_cSprite, "x", _rbf rb_Sprite_getX, 0);
    rb_define_method(rb_cSprite, "x=", _rbf rb_Sprite_setX, 1);
    rb_define_method(rb_cSprite, "y", _rbf rb_Sprite_getY, 0);
    rb_define_method(rb_cSprite, "y=", _rbf rb_Sprite_setY, 1);
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

    rb_define_method(rb_cSprite, "clone", _rbf rb_Sprite_Copy, 0);
    rb_define_method(rb_cSprite, "dup", _rbf rb_Sprite_Copy, 0);

    rb_Sprite_ivViewport = rb_intern("@viewport");
    rb_Sprite_ivBitmap = rb_intern("@bitmap");
    /* I use instance variable for performance and to have right values */
    rb_Sprite_ivX = rb_intern("@x");
    rb_Sprite_ivY = rb_intern("@y");
    rb_Sprite_ivOX = rb_intern("@ox");
    rb_Sprite_ivOY = rb_intern("@oy");
    rb_Sprite_ivAngle = rb_intern("@angle");
    rb_Sprite_ivZoomX = rb_intern("@zoom_x");
    rb_Sprite_ivZoomY = rb_intern("@zoom_y");
    rb_Sprite_ivRect = rb_intern("@src_rect");
}

VALUE rb_Sprite_Initialize(int argc, VALUE* argv, VALUE self)
{
    CSprite_Element* sprite;
    Data_Get_Struct(self, CSprite_Element, sprite);
    VALUE table;
    /* If a viewport was specified */
    if(argc == 1 && rb_obj_is_kind_of(argv[0], rb_cViewport) == Qtrue)//rb_class_of(argv[0]) == rb_cViewport*/ && false)
    {
        CViewport_Element* viewport;
        Data_Get_Struct(argv[0], CViewport_Element, viewport);
        viewport->bindSprite(sprite);
        table = rb_ivar_get(argv[0], rb_iElementTable);
        rb_ivar_set(self, rb_Sprite_ivViewport, argv[0]);
    }
    /* Otherwise */
    else
    {
        __Graphics_Bind(sprite);
        table = rb_ivar_get(rb_mGraphics, rb_iElementTable);
    }
    /* Ajout à la table de sauvegarde */
    rb_ary_push(table, self);
    /* Initializing Instance variables */
    rb_ivar_set(self, rb_Sprite_ivX, LONG2FIX(0));
    rb_ivar_set(self, rb_Sprite_ivY, LONG2FIX(0));
    rb_ivar_set(self, rb_Sprite_ivOX, LONG2FIX(0));
    rb_ivar_set(self, rb_Sprite_ivOY, LONG2FIX(0));
    rb_ivar_set(self, rb_Sprite_ivAngle, LONG2FIX(0));
    rb_ivar_set(self, rb_Sprite_ivZoomX, LONG2FIX(1));
    rb_ivar_set(self, rb_Sprite_ivZoomY, LONG2FIX(1));
    return self;
}

VALUE rb_Sprite_Copy(VALUE self)
{
    rb_raise(rb_eRGSSError, "Sprites can not be cloned or duplicated.");
    return self;
}

VALUE rb_Sprite_Dispose(VALUE self)
{
    SPRITE_PROTECT
    CSprite_Element* sprite;
    Data_Get_Struct(self, CSprite_Element, sprite);
    RDATA(self)->data = nullptr;
    /* Suppression du sprite de ses stacks */
    VALUE viewport = rb_ivar_get(self, rb_Sprite_ivViewport);
    VALUE table;
    if(NIL_P(viewport))
        table = rb_ivar_get(rb_mGraphics, rb_iElementTable);
    else
        table = rb_ivar_get(viewport, rb_iElementTable);
    rb_ary_delete(table, self);
    sprite->setOriginStack(nullptr);
    delete sprite;
    return self;
}

VALUE rb_Sprite_Disposed(VALUE self)
{
    rb_check_type(self, T_DATA);
    return (RDATA(self)->data == nullptr ? Qtrue : Qfalse);
}

VALUE rb_Sprite_setBitmap(VALUE self, VALUE bitmap)
{
    SPRITE_PROTECT
    CSprite_Element* sprite;
    Data_Get_Struct(self, CSprite_Element, sprite);
    if(rb_obj_is_kind_of(bitmap, rb_cBitmap) == Qfalse)//rb_class_of(bitmap) != rb_cBitmap)
    {
        if(bitmap == Qnil)
        {
            sprite->setDrawable(false);
            rb_ivar_set(self, rb_Sprite_ivBitmap, bitmap);
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
    sf::Sprite* sp = sprite->getSprite();
    sp->setTexture(*bmp->getTexture(), true);
    sprite->setDrawable(true);
    /* resseting the SRC_RECT */
    // TODO
    rb_ivar_set(self, rb_Sprite_ivBitmap, bitmap);
    return self;
}

VALUE rb_Sprite_getBitmap(VALUE self)
{
    SPRITE_PROTECT
    return rb_ivar_get(self, rb_Sprite_ivBitmap);
}

VALUE rb_Sprite_setX(VALUE self, VALUE val)
{
    SPRITE_PROTECT
    CSprite_Element* sprite;
    Data_Get_Struct(self, CSprite_Element, sprite);
    sf::Sprite* sp = sprite->getSprite();
    const sf::Vector2f vect = sp->getPosition();
    sp->setPosition(static_cast<float>(rb_num2long(val)), vect.y);
    rb_ivar_set(self, rb_Sprite_ivX, val);
    return val;
}

VALUE rb_Sprite_getX(VALUE self)
{
    SPRITE_PROTECT
    return rb_ivar_get(self, rb_Sprite_ivX);
}

VALUE rb_Sprite_setY(VALUE self, VALUE val)
{
    SPRITE_PROTECT
    CSprite_Element* sprite;
    Data_Get_Struct(self, CSprite_Element, sprite);
    sf::Sprite* sp = sprite->getSprite();
    const sf::Vector2f vect = sp->getPosition();
    sp->setPosition(vect.x, static_cast<float>(rb_num2long(val)));
    rb_ivar_set(self, rb_Sprite_ivY, val);
    return val;
}

VALUE rb_Sprite_getY(VALUE self)
{
    SPRITE_PROTECT
    return rb_ivar_get(self, rb_Sprite_ivY);
}

VALUE rb_Sprite_setOX(VALUE self, VALUE val)
{
    SPRITE_PROTECT
    CSprite_Element* sprite;
    Data_Get_Struct(self, CSprite_Element, sprite);
    sf::Sprite* sp = sprite->getSprite();
    const sf::Vector2f vect = sp->getOrigin();
    sp->setOrigin(static_cast<float>(rb_num2long(val)), vect.y);
    rb_ivar_set(self, rb_Sprite_ivOX, val);
    return val;
}

VALUE rb_Sprite_getOX(VALUE self)
{
    SPRITE_PROTECT
    return rb_ivar_get(self, rb_Sprite_ivOX);
}

VALUE rb_Sprite_setOY(VALUE self, VALUE val)
{
    SPRITE_PROTECT
    CSprite_Element* sprite;
    Data_Get_Struct(self, CSprite_Element, sprite);
    sf::Sprite* sp = sprite->getSprite();
    const sf::Vector2f vect = sp->getOrigin();
    sp->setOrigin(vect.x, static_cast<float>(rb_num2long(val)));
    rb_ivar_set(self, rb_Sprite_ivOY, val);
    return val;
}

VALUE rb_Sprite_getOY(VALUE self)
{
    SPRITE_PROTECT
    return rb_ivar_get(self, rb_Sprite_ivOY);
}

VALUE rb_Sprite_setVisible(VALUE self, VALUE val)
{
    SPRITE_PROTECT
    CSprite_Element* sprite;
    Data_Get_Struct(self, CSprite_Element, sprite);
    sprite->setVisible(RTEST(val));
    return val;
}

VALUE rb_Sprite_getVisible(VALUE self)
{
    SPRITE_PROTECT
    CSprite_Element* sprite;
    Data_Get_Struct(self, CSprite_Element, sprite);
    return sprite->getVisible() ? Qtrue : Qfalse;
}

VALUE rb_Sprite_setAngle(VALUE self, VALUE val)
{
    SPRITE_PROTECT
    CSprite_Element* sprite;
    Data_Get_Struct(self, CSprite_Element, sprite);
    sprite->getSprite()->setRotation(static_cast<float>(rb_num2dbl(val)));
    rb_ivar_set(self, rb_Sprite_ivAngle, val);
    return val;
}

VALUE rb_Sprite_getAngle(VALUE self)
{
    SPRITE_PROTECT
    return rb_ivar_get(self, rb_Sprite_ivAngle);
}

VALUE rb_Sprite_setZoomX(VALUE self, VALUE val)
{
    SPRITE_PROTECT
    CSprite_Element* sprite;
    Data_Get_Struct(self, CSprite_Element, sprite);
    sf::Sprite* sp = sprite->getSprite();
    const sf::Vector2f vect = sp->getScale();
    sp->setScale(static_cast<float>(rb_num2dbl(val)), vect.y);
    rb_ivar_set(self, rb_Sprite_ivZoomX, val);
    return val;
}

VALUE rb_Sprite_getZoomX(VALUE self)
{
    SPRITE_PROTECT
    return rb_ivar_get(self, rb_Sprite_ivZoomX);
}

VALUE rb_Sprite_setZoomY(VALUE self, VALUE val)
{
    SPRITE_PROTECT
    CSprite_Element* sprite;
    Data_Get_Struct(self, CSprite_Element, sprite);
    sf::Sprite* sp = sprite->getSprite();
    const sf::Vector2f vect = sp->getScale();
    sp->setScale(vect.x, static_cast<float>(rb_num2dbl(val)));
    rb_ivar_set(self, rb_Sprite_ivZoomY, val);
    return val;
}

VALUE rb_Sprite_getZoomY(VALUE self)
{
    SPRITE_PROTECT
    return rb_ivar_get(self, rb_Sprite_ivZoomY);
}


VALUE rb_Sprite_setPosition(VALUE self, VALUE x, VALUE y)
{
    SPRITE_PROTECT
    CSprite_Element* sprite;
    Data_Get_Struct(self, CSprite_Element, sprite);
    sprite->getSprite()->setPosition(static_cast<float>(rb_num2long(x)), static_cast<float>(rb_num2long(y)));
    rb_ivar_set(self, rb_Sprite_ivY, y);
    rb_ivar_set(self, rb_Sprite_ivX, x);
    return self;
}

VALUE rb_Sprite_setOrigin(VALUE self, VALUE x, VALUE y)
{
    SPRITE_PROTECT
    CSprite_Element* sprite;
    Data_Get_Struct(self, CSprite_Element, sprite);
    sprite->getSprite()->setOrigin(static_cast<float>(rb_num2long(x)), static_cast<float>(rb_num2long(y)));
    rb_ivar_set(self, rb_Sprite_ivOY, y);
    rb_ivar_set(self, rb_Sprite_ivOX, x);
    return self;
}

VALUE rb_Sprite_setZoom(VALUE self, VALUE zoom)
{
    SPRITE_PROTECT
    CSprite_Element* sprite;
    Data_Get_Struct(self, CSprite_Element, sprite);
    float scale = static_cast<float>(rb_num2dbl(zoom));
    sprite->getSprite()->setScale(scale, scale);
    rb_ivar_set(self, rb_Sprite_ivZoomY, zoom);
    rb_ivar_set(self, rb_Sprite_ivZoomX, zoom);
    return zoom;
}

VALUE rb_Sprite_setOpacity(VALUE self, VALUE val)
{
    SPRITE_PROTECT
    CSprite_Element* sprite;
    Data_Get_Struct(self, CSprite_Element, sprite);
    sf::Sprite* sp = sprite->getSprite();
    const sf::Color col = sp->getColor();
    sp->setColor(sf::Color(col.r, col.g, col.b, normalize_long(rb_num2long(val), 0, 255)));
    return val;
}

VALUE rb_Sprite_getOpacity(VALUE self)
{
    SPRITE_PROTECT
    CSprite_Element* sprite;
    Data_Get_Struct(self, CSprite_Element, sprite);
    sf::Sprite* sp = sprite->getSprite();
    const sf::Color col = sp->getColor();
    return rb_int2inum(col.r);
}

VALUE rb_Sprite_getRect(VALUE self)
{
    SPRITE_PROTECT
    VALUE rc = rb_ivar_get(self, rb_Sprite_ivRect);
    if(!NIL_P(rc))
        return rc;
    /* Creating rect */
    VALUE argv[2];
    argv[0] = argv[1] = LONG2FIX(0);
    rc = rb_class_new_instance(2, argv, rb_cRect);
    /* Fetching data */
    CRect_Element* rect;
    Data_Get_Struct(rc, CRect_Element, rect);
    CSprite_Element* sprite;
    Data_Get_Struct(self, CSprite_Element, sprite);
    /* Setting rect parameter */
    const sf::IntRect rectorigin = sprite->getSprite()->getTextureRect();
    rect_copy(rect->getRect(), &rectorigin);
    /* Linking Rect */
    rect->setElement(sprite);
    rb_ivar_set(self, rb_Sprite_ivRect, rc);
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
    sf::IntRect* rect_target = rect2->getRect();
    rect_copy(rect_target, rect1->getRect());
    /* Updating the texture rect */
    sprite->getSprite()->setTextureRect(*rect_target);
    return val;
}