#include "LiteRGSS.h"
#include "CBitmap_Element.h"
#include "CRect_Element.h"

VALUE rb_cSprite = Qnil;

#define SPRITE_PROTECT if(RDATA(self)->data == nullptr) \
{\
    rb_raise(rb_eRGSSError, "Disposed Sprite."); \
    return self; \
}

#define GET_SPRITE CSprite_Element* sprite; \
    Data_Get_Struct(self, CSprite_Element, sprite); \
    SPRITE_PROTECT

void rb_Sprite_Free(void* data)
{
    CSprite_Element* sprite = reinterpret_cast<CSprite_Element*>(data);
    if(sprite != nullptr)
    {
        if(NIL_P(sprite->rViewport)) // I can drop a sprite from the viewport it's stored in its table
            sprite->setOriginStack(nullptr);
        CRect_Element* rect = sprite->getLinkedRect();
        if(rect != nullptr)
            rect->setElement(nullptr);
        delete sprite;
    }
}

void rb_Sprite_Mark(CSprite_Element* sprite)
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

VALUE rb_Sprite_Alloc(VALUE klass)
{
    CSprite_Element* sprite = new CSprite_Element();
    sprite->getSprite()->setColor(sf::Color(255, 255, 255, 255));
    sprite->setLinkedRect(nullptr);
    return Data_Wrap_Struct(klass, rb_Sprite_Mark, rb_Sprite_Free, sprite);
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

    rb_define_method(rb_cSprite, "clone", _rbf rb_Sprite_Copy, 0);
    rb_define_method(rb_cSprite, "dup", _rbf rb_Sprite_Copy, 0);
}

VALUE rb_Sprite_Initialize(int argc, VALUE* argv, VALUE self)
{
    GET_SPRITE
    VALUE table;
    /* If a viewport was specified */
    if(argc == 1 && rb_obj_is_kind_of(argv[0], rb_cViewport) == Qtrue)
    {
        CViewport_Element* viewport;
        Data_Get_Struct(argv[0], CViewport_Element, viewport);
        viewport->bind(sprite);
        table = rb_ivar_get(argv[0], rb_iElementTable);
        sprite->rViewport = argv[0];
    }
    /* Otherwise */
    else
    {
        global_Graphics_Bind(sprite);
        table = rb_ivar_get(rb_mGraphics, rb_iElementTable);
        sprite->rViewport = Qnil;
    }
    /* Ajout à la table de sauvegarde */
    rb_ary_push(table, self);
    /* Initializing Instance variables */
    sprite->rX = LONG2FIX(0);
    sprite->rY = LONG2FIX(0);
    sprite->rZ = LONG2FIX(0);
    sprite->rOX = LONG2FIX(0);
    sprite->rOY = LONG2FIX(0);
    sprite->rAngle = LONG2FIX(0);
    sprite->rZoomX = LONG2FIX(1);
    sprite->rZoomY = LONG2FIX(1);
    sprite->rBitmap = Qnil;
    sprite->rRect = Qnil;
    return self;
}

VALUE rb_Sprite_Copy(VALUE self)
{
    rb_raise(rb_eRGSSError, "Sprites can not be cloned or duplicated.");
    return self;
}

VALUE rb_Sprite_Dispose(VALUE self)
{
    GET_SPRITE
    RDATA(self)->data = nullptr;
    /* Suppression du sprite de ses stacks */
    VALUE viewport = sprite->rViewport;
    VALUE table;
    if(NIL_P(viewport))
        table = rb_ivar_get(rb_mGraphics, rb_iElementTable);
    else
        table = rb_ivar_get(viewport, rb_iElementTable);
    rb_ary_delete(table, self);
    sprite->setOriginStack(nullptr); // Ensure the sprite has been removed from the sprite stack
    rb_Sprite_Free(reinterpret_cast<void*>(sprite));
    return self;
}

VALUE rb_Sprite_DisposeFromViewport(VALUE self)
{
    if(RDATA(self)->data == nullptr)
        return self;
    GET_SPRITE
    RDATA(self)->data = nullptr;
    rb_Sprite_Free(reinterpret_cast<void*>(sprite));
    return self;
}

VALUE rb_Sprite_Disposed(VALUE self)
{
    rb_check_type(self, T_DATA);
    return (RDATA(self)->data == nullptr ? Qtrue : Qfalse);
}

VALUE rb_Sprite_setBitmap(VALUE self, VALUE bitmap)
{
    GET_SPRITE
    if(rb_obj_is_kind_of(bitmap, rb_cBitmap) == Qfalse)
    {
        if(bitmap == Qnil)
        {
            sprite->setDrawable(false);
            sprite->rBitmap = bitmap;
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
    sprite->rBitmap = bitmap;
    return self;
}

VALUE rb_Sprite_getBitmap(VALUE self)
{
    GET_SPRITE
    return sprite->rBitmap;
}

VALUE rb_Sprite_setX(VALUE self, VALUE val)
{
    GET_SPRITE
    sf::Sprite* sp = sprite->getSprite();
    const sf::Vector2f vect = sp->getPosition();
    sp->setPosition(static_cast<float>(rb_num2long(val)), vect.y);
    sprite->rX = val;
    return val;
}

VALUE rb_Sprite_getX(VALUE self)
{
    GET_SPRITE
    return sprite->rX;
}

VALUE rb_Sprite_setY(VALUE self, VALUE val)
{
    GET_SPRITE
    sf::Sprite* sp = sprite->getSprite();
    const sf::Vector2f vect = sp->getPosition();
    sp->setPosition(vect.x, static_cast<float>(rb_num2long(val)));
    sprite->rY = val;
    return val;
}

VALUE rb_Sprite_getY(VALUE self)
{
    GET_SPRITE
    return sprite->rY;
}

VALUE rb_Sprite_setZ(VALUE self, VALUE val)
{
    GET_SPRITE
    rb_num2long(val);
    sprite->rZ = val;
    return val;
}

VALUE rb_Sprite_getZ(VALUE self)
{
    GET_SPRITE
    return sprite->rZ;
}

VALUE rb_Sprite_setOX(VALUE self, VALUE val)
{
    GET_SPRITE
    sf::Sprite* sp = sprite->getSprite();
    const sf::Vector2f vect = sp->getOrigin();
    sp->setOrigin(static_cast<float>(rb_num2long(val)), vect.y);
    sprite->rOX = val;
    return val;
}

VALUE rb_Sprite_getOX(VALUE self)
{
    GET_SPRITE
    return sprite->rOX;
}

VALUE rb_Sprite_setOY(VALUE self, VALUE val)
{
    GET_SPRITE
    sf::Sprite* sp = sprite->getSprite();
    const sf::Vector2f vect = sp->getOrigin();
    sp->setOrigin(vect.x, static_cast<float>(rb_num2long(val)));
    sprite->rOY = val;
    return val;
}

VALUE rb_Sprite_getOY(VALUE self)
{
    GET_SPRITE
    return sprite->rOY;
}

VALUE rb_Sprite_setVisible(VALUE self, VALUE val)
{
    GET_SPRITE
    sprite->setVisible(RTEST(val));
    return val;
}

VALUE rb_Sprite_getVisible(VALUE self)
{
    GET_SPRITE
    return sprite->getVisible() ? Qtrue : Qfalse;
}

VALUE rb_Sprite_setAngle(VALUE self, VALUE val)
{
    GET_SPRITE
    sprite->getSprite()->setRotation(static_cast<float>(rb_num2dbl(val)));
    sprite->rAngle = val;
    return val;
}

VALUE rb_Sprite_getAngle(VALUE self)
{
    GET_SPRITE
    return sprite->rAngle;
}

VALUE rb_Sprite_setZoomX(VALUE self, VALUE val)
{
    GET_SPRITE
    sf::Sprite* sp = sprite->getSprite();
    const sf::Vector2f vect = sp->getScale();
    sp->setScale(static_cast<float>(rb_num2dbl(val)), vect.y);
    sprite->rZoomX = val;
    return val;
}

VALUE rb_Sprite_getZoomX(VALUE self)
{
    GET_SPRITE
    return sprite->rZoomX;
}

VALUE rb_Sprite_setZoomY(VALUE self, VALUE val)
{
    GET_SPRITE
    sf::Sprite* sp = sprite->getSprite();
    const sf::Vector2f vect = sp->getScale();
    sp->setScale(vect.x, static_cast<float>(rb_num2dbl(val)));
    sprite->rZoomY = val;
    return val;
}

VALUE rb_Sprite_getZoomY(VALUE self)
{
    GET_SPRITE
    return sprite->rZoomY;
}


VALUE rb_Sprite_setPosition(VALUE self, VALUE x, VALUE y)
{
    GET_SPRITE
    sprite->getSprite()->setPosition(static_cast<float>(rb_num2long(x)), static_cast<float>(rb_num2long(y)));
    sprite->rX = x;
    sprite->rY = y;
    return self;
}

VALUE rb_Sprite_setOrigin(VALUE self, VALUE x, VALUE y)
{
    GET_SPRITE
    sprite->getSprite()->setOrigin(static_cast<float>(rb_num2long(x)), static_cast<float>(rb_num2long(y)));
    sprite->rOX = x;
    sprite->rOY = y;
    return self;
}

VALUE rb_Sprite_setZoom(VALUE self, VALUE zoom)
{
    GET_SPRITE
    float scale = static_cast<float>(rb_num2dbl(zoom));
    sprite->getSprite()->setScale(scale, scale);
    sprite->rZoomX = zoom;
    sprite->rZoomY = zoom;
    return zoom;
}

VALUE rb_Sprite_setOpacity(VALUE self, VALUE val)
{
    GET_SPRITE
    sf::Sprite* sp = sprite->getSprite();
    const sf::Color col = sp->getColor();
    sp->setColor(sf::Color(col.r, col.g, col.b, normalize_long(rb_num2long(val), 0, 255)));
    return val;
}

VALUE rb_Sprite_getOpacity(VALUE self)
{
    GET_SPRITE
    sf::Sprite* sp = sprite->getSprite();
    const sf::Color col = sp->getColor();
    return rb_int2inum(col.r);
}

VALUE rb_Sprite_getRect(VALUE self)
{
    GET_SPRITE
    VALUE rc = sprite->rRect;
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
    const sf::IntRect rectorigin = sprite->getSprite()->getTextureRect();
    rect_copy(rect->getRect(), &rectorigin);
    /* Linking Rect */
    rect->setElement(sprite);
    sprite->setLinkedRect(rect);
    sprite->rRect = rc;
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

VALUE rb_Sprite_Viewport(VALUE self)
{
    GET_SPRITE
    return sprite->rViewport;
}