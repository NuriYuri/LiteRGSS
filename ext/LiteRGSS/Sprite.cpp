#include "ruby.h"
#include "LiteRGSS.h"
#include "Sprite.h"
#include "CBitmap_Element.h"

VALUE rb_cSprite = Qnil;
ID rb_Sprite_ivViewport = Qnil;
ID rb_Sprite_ivBitmap = Qnil;

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
    rb_define_method(rb_cSprite, "bitmap=", _rbf rb_Sprite_setBitmap, 1);

    rb_define_method(rb_cSprite, "clone", _rbf rb_Sprite_Copy, 0);
    rb_define_method(rb_cSprite, "dup", _rbf rb_Sprite_Copy, 0);

    rb_Sprite_ivViewport = rb_intern("@viewport");
    rb_Sprite_ivBitmap = rb_intern("@bitmap");
}

VALUE rb_Sprite_Initialize(int argc, VALUE* argv, VALUE self)
{
    CSprite_Element* sprite;
    Data_Get_Struct(self, CSprite_Element, sprite);
    VALUE table;
    /* If a viewport was specified */
    if(argc == 1 /*&& rb_class_of(argv[0]) == rb_cViewport*/ && false)
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

VALUE rb_Sprite_setBitmap(VALUE self, VALUE bitmap)
{
    SPRITE_PROTECT
    CSprite_Element* sprite;
    Data_Get_Struct(self, CSprite_Element, sprite);
    if(rb_class_of(bitmap) != rb_cBitmap)
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