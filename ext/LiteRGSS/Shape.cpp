#include "LiteRGSS.h"
#include "CBitmap_Element.h"
#include "CRect_Element.h"

VALUE rb_cShape = Qnil;
ID rb_iShapeCircle = Qnil;
ID rb_iShapeConvex = Qnil;
ID rb_iShapeRectangle = Qnil;

#define SHAPE_PROTECT if(RDATA(self)->data == nullptr) \
{\
    rb_raise(rb_eRGSSError, "Disposed Shape."); \
}

#define GET_SHAPE CShape_Element* shape; \
    Data_Get_Struct(self, CShape_Element, shape); \
    SHAPE_PROTECT

void rb_Shape_Free(void* data)
{
	CShape_Element* shape = reinterpret_cast<CShape_Element*>(data);
	if (shape != nullptr)
	{
		if (NIL_P(shape->rViewport)) // I can drop a sprite from the viewport it's stored in its table
			shape->setOriginStack(nullptr);
		CRect_Element* rect = shape->getLinkedRect();
		if (rect != nullptr)
			rect->setElement(nullptr);
		delete shape;
	}
}

void rb_Shape_Mark(CShape_Element* shape)
{
	if (shape == nullptr)
		return;
	rb_gc_mark(shape->rViewport);
	rb_gc_mark(shape->rBitmap);
	rb_gc_mark(shape->rX);
	rb_gc_mark(shape->rY);
	rb_gc_mark(shape->rZ);
	rb_gc_mark(shape->rOX);
	rb_gc_mark(shape->rOY);
	rb_gc_mark(shape->rAngle);
	rb_gc_mark(shape->rZoomX);
	rb_gc_mark(shape->rZoomY);
	rb_gc_mark(shape->rRect);
	rb_gc_mark(shape->rRenderStates);
	rb_gc_mark(shape->rShapeType);
	rb_gc_mark(shape->rColor);
	rb_gc_mark(shape->rOutlineColor);
	rb_gc_mark(shape->rOutlineThickness);
}

VALUE rb_Shape_Alloc(VALUE klass)
{
	CShape_Element* shape = new CShape_Element();
	shape->setLinkedRect(nullptr);
	return Data_Wrap_Struct(klass, rb_Shape_Mark, rb_Shape_Free, shape);
}

void Init_Shape()
{
	rb_cShape = rb_define_class_under(rb_mLiteRGSS, "Shape", rb_cObject);
	rb_define_alloc_func(rb_cShape, rb_Shape_Alloc);

	rb_iShapeCircle = rb_intern("circle");
	rb_iShapeConvex = rb_intern("convex");
	rb_iShapeRectangle = rb_intern("rectangle");

	rb_define_method(rb_cShape, "initialize", _rbf rb_Shape_Initialize, -1);
	rb_define_method(rb_cShape, "bitmap", _rbf rb_Shape_getBitmap, 0);
	rb_define_method(rb_cShape, "bitmap=", _rbf rb_Shape_setBitmap, 1);
	rb_define_method(rb_cShape, "src_rect", _rbf rb_Shape_getRect, 0);
	rb_define_method(rb_cShape, "src_rect=", _rbf rb_Shape_setRect, 1);
	rb_define_method(rb_cShape, "x", _rbf rb_Shape_getX, 0);
	rb_define_method(rb_cShape, "x=", _rbf rb_Shape_setX, 1);
	rb_define_method(rb_cShape, "y", _rbf rb_Shape_getY, 0);
	rb_define_method(rb_cShape, "y=", _rbf rb_Shape_setY, 1);
	rb_define_method(rb_cShape, "set_position", _rbf rb_Shape_setPosition, 2);
	rb_define_method(rb_cShape, "z", _rbf rb_Shape_getZ, 0);
	rb_define_method(rb_cShape, "z=", _rbf rb_Shape_setZ, 1);
	rb_define_method(rb_cShape, "ox", _rbf rb_Shape_getOX, 0);
	rb_define_method(rb_cShape, "ox=", _rbf rb_Shape_setOX, 1);
	rb_define_method(rb_cShape, "oy", _rbf rb_Shape_getOY, 0);
	rb_define_method(rb_cShape, "oy=", _rbf rb_Shape_setOY, 1);
	rb_define_method(rb_cShape, "set_origin", _rbf rb_Shape_setOrigin, 2);
	rb_define_method(rb_cShape, "angle", _rbf rb_Shape_getAngle, 0);
	rb_define_method(rb_cShape, "angle=", _rbf rb_Shape_setAngle, 1);
	rb_define_method(rb_cShape, "zoom_x", _rbf rb_Shape_getZoomX, 0);
	rb_define_method(rb_cShape, "zoom_x=", _rbf rb_Shape_setZoomX, 1);
	rb_define_method(rb_cShape, "zoom_y", _rbf rb_Shape_getZoomY, 0);
	rb_define_method(rb_cShape, "zoom_y=", _rbf rb_Shape_setZoomY, 1);
	rb_define_method(rb_cShape, "zoom=", _rbf rb_Shape_setZoom, 1);
	rb_define_method(rb_cShape, "viewport", _rbf rb_Shape_getViewport, 0);
	rb_define_method(rb_cShape, "visible", _rbf rb_Shape_getVisible, 0);
	rb_define_method(rb_cShape, "visible=", _rbf rb_Shape_setVisible, 1);
	rb_define_method(rb_cShape, "point_count", _rbf rb_Shape_getPointCount, 0);
	rb_define_method(rb_cShape, "point_count=", _rbf rb_Shape_setPointCount, 1);
	rb_define_method(rb_cShape, "get_point", _rbf rb_Shape_getPoint, 1);
	rb_define_method(rb_cShape, "set_point", _rbf rb_Shape_setPoint, 3);
	rb_define_method(rb_cShape, "color", _rbf rb_Shape_getColor, 0);
	rb_define_method(rb_cShape, "color=", _rbf rb_Shape_setColor, 1);
	rb_define_method(rb_cShape, "outline_color", _rbf rb_Shape_getOutlineColor, 0);
	rb_define_method(rb_cShape, "outline_color=", _rbf rb_Shape_setOutlineColor, 1);
	rb_define_method(rb_cShape, "outline_thickness", _rbf rb_Shape_getOutlineThickness, 0);
	rb_define_method(rb_cShape, "outline_thickness=", _rbf rb_Shape_setOutlineThickness, 1);
	rb_define_method(rb_cShape, "__index__", _rbf rb_Shape_getIndex, 0);
	rb_define_method(rb_cShape, "disposed?", _rbf rb_Shape_Disposed, 0);
	rb_define_method(rb_cShape, "dispose", _rbf rb_Shape_Dispose, 0);
	rb_define_method(rb_cShape, "radius", _rbf rb_Shape_getRadius, 0);
	rb_define_method(rb_cShape, "radius=", _rbf rb_Shape_setRadius, 1);
	rb_define_method(rb_cShape, "type", _rbf rb_Shape_getType, 0);
	rb_define_method(rb_cShape, "width", _rbf rb_Shape_getWidth, 0);
	rb_define_method(rb_cShape, "width=", _rbf rb_Shape_setWidth, 1);
	rb_define_method(rb_cShape, "height", _rbf rb_Shape_getHeight, 0);
	rb_define_method(rb_cShape, "height=", _rbf rb_Shape_setHeight, 1);

	rb_define_method(rb_cShape, "shader", _rbf rb_Shape_getShader, 0);
	rb_define_method(rb_cShape, "shader=", _rbf rb_Shape_setShader, 1);
	rb_define_method(rb_cShape, "blendmode", _rbf rb_Shape_getShader, 0);
	rb_define_method(rb_cShape, "blendmode=", _rbf rb_Shape_setShader, 1);

	rb_define_method(rb_cShape, "clone", _rbf rb_Shape_Copy, 0);
	rb_define_method(rb_cShape, "dup", _rbf rb_Shape_Copy, 0);

	rb_define_const(rb_cShader, "CIRCLE", ID2SYM(rb_iShapeCircle));
	rb_define_const(rb_cShader, "CONVEX", ID2SYM(rb_iShapeConvex));
	rb_define_const(rb_cShader, "RECTANGLE", ID2SYM(rb_iShapeRectangle));
}

void rb_Shape_InitObject(CShape_Element* shape)
{
	sf::Shape* sf_shape = shape->getShape();
	sf_shape->setFillColor(sf::Color::White);
	shape->rOutlineThickness = shape->rAngle = shape->rOX = shape->rOY = shape->rX = shape->rY = shape->rZ = LONG2NUM(0);
	shape->rZoomX = shape->rZoomY = LONG2NUM(1);
	shape->rColor = shape->rOutlineColor = shape->rBitmap = shape->rRect = shape->rRenderStates = Qnil;
}

VALUE rb_Shape_Initialize(int argc, VALUE* argv, VALUE self)
{
	CShape_Element* shape;
	VALUE viewport, type, rad_numPoint, numPoint;
	ID itype;

	Data_Get_Struct(self, CShape_Element, shape);
	rb_scan_args(argc, argv, "22", &viewport, &type, &rad_numPoint, &numPoint);

	// Viewport push
	if (rb_obj_is_kind_of(viewport, rb_cViewport) != Qtrue)
		rb_raise(rb_eRGSSError, "Shape require viewport to be initialized.");
	CViewport_Element* viewport_el;
	Data_Get_Struct(viewport, CViewport_Element, viewport_el);
	viewport_el->bind(shape);
	shape->rViewport = viewport;

	// Shape initialization
	itype = SYM2ID(type);
	/* Circle Shape */
	if (itype == rb_iShapeCircle)
	{
		float radius = 1.0f;
		unsigned long numpt;
		if (NIL_P(numPoint))
			numpt = NUM2ULONG(rad_numPoint);
		else
		{
			if (!NIL_P(rad_numPoint))
				radius = abs(NUM2DBL(rad_numPoint));
			numpt = NUM2ULONG(numPoint);
		}
		shape->setShape(new sf::CircleShape(radius, numpt));
		shape->rShapeType = type;
	}
	/* Convex Shape */
	else if (itype == rb_iShapeConvex)
	{
		unsigned long numpt = 4;
		if (!NIL_P(rad_numPoint))
			numpt = NUM2ULONG(rad_numPoint);
		shape->setShape(new sf::ConvexShape(numpt));
		shape->rShapeType = type;
	}
	/* Rectangle Shape */
	else
	{
		if (!NIL_P(rad_numPoint) && !NIL_P(numPoint))
			shape->setShape(new sf::RectangleShape(sf::Vector2f(NUM2DBL(rad_numPoint), NUM2DBL(numPoint))));
		else
			shape->setShape(new sf::RectangleShape());
		shape->rShapeType = ID2SYM(rb_iShapeRectangle);
	}
	rb_Shape_InitObject(shape);
	return self;
}


VALUE rb_Shape_Dispose(VALUE self)
{
	GET_SHAPE;
	RDATA(self)->data = nullptr;
	/* Suppression du shape de ses stacks */
	VALUE viewport = shape->rViewport;
	VALUE table;
	if (NIL_P(viewport))
		table = rb_ivar_get(rb_mGraphics, rb_iElementTable);
	else
		table = rb_ivar_get(viewport, rb_iElementTable);
	rb_ary_delete(table, self);
	shape->setOriginStack(nullptr);
	rb_Shape_Free(shape);
	return self;
}

VALUE rb_Shape_Disposed(VALUE self)
{
	Check_Type(self, T_DATA);
	return (RDATA(self)->data == nullptr ? Qtrue : Qfalse);
}

VALUE rb_Shape_getBitmap(VALUE self)
{
	GET_SHAPE;
	return shape->rBitmap;
}

VALUE rb_Shape_setBitmap(VALUE self, VALUE bitmap)
{
	GET_SHAPE;
	CRect_Element* rect;
	if (NIL_P(bitmap))
	{
		shape->getShape()->setTexture(nullptr);
		shape->rBitmap = Qnil;
		/* unlink rect */
		if (!NIL_P(shape->rRect))
		{
			rect = rb_Rect_get_rect(shape->rRect);
			rect->setElement(nullptr);
			shape->setLinkedRect(nullptr);
			shape->rRect = Qnil;
		}
		return self;
	}
	sf::Texture* texture = rb_Bitmap_getTexture(bitmap);
	shape->getShape()->setTexture(texture, true);
	shape->rBitmap = bitmap;
	/* update rect */
	if (!NIL_P(shape->rRect))
	{
		rect = rb_Rect_get_rect(shape->rRect);
		/* Setting rect parameter */
		const sf::IntRect rectorigin = shape->getShape()->getTextureRect();
		rect_copy(rect->getRect(), &rectorigin);
	}
	return self;
}

VALUE rb_Shape_getRect(VALUE self)
{
	GET_SHAPE;
	VALUE rc = shape->rRect;
	if (!NIL_P(rc))
		return rc;
	/* Creating rect */
	VALUE argv[2];
	argv[0] = argv[1] = LONG2FIX(0);
	rc = rb_class_new_instance(2, argv, rb_cRect);
	/* Fetching data */
	CRect_Element* rect = rb_Rect_get_rect(rc);
	/* Setting rect parameter */
	const sf::IntRect rectorigin = shape->getShape()->getTextureRect();
	rect_copy(rect->getRect(), &rectorigin);
	/* Linking Rect */
	rect->setElement(shape);
	shape->setLinkedRect(rect);
	shape->rRect = rc;
	return rc;
}

VALUE rb_Shape_setRect(VALUE self, VALUE rect)
{
	GET_SHAPE;
	CRect_Element* rect1;
	CRect_Element* rect2;
	VALUE rc = rb_Shape_getRect(self);
	if (RDATA(rc)->data == nullptr) { return Qnil; }
	/* Getting data to update the rect */
	rect1 = rb_Rect_get_rect(rect);
	rect2 = rb_Rect_get_rect(rc);
	Data_Get_Struct(rc, CRect_Element, rect2);
	/* Copying the rect */
	sf::IntRect* rect_target = rect2->getRect();
	rect_copy(rect_target, rect1->getRect());
	/* Update texture rect */
	shape->getShape()->setTextureRect(*rect_target);
	return self;
}

VALUE rb_Shape_getX(VALUE self)
{
	GET_SHAPE;
	return shape->rX;
}

VALUE rb_Shape_setX(VALUE self, VALUE val)
{
	GET_SHAPE;
	shape->getShape()->setPosition(sf::Vector2f(NUM2DBL(val), NUM2DBL(shape->rY)));
	shape->rX = val;
	return self;
}

VALUE rb_Shape_getY(VALUE self)
{
	GET_SHAPE;
	return shape->rY;
}

VALUE rb_Shape_setY(VALUE self, VALUE val)
{
	GET_SHAPE;
	shape->getShape()->setPosition(sf::Vector2f(NUM2DBL(shape->rX), NUM2DBL(val)));
	shape->rY = val;
	return self;
}

VALUE rb_Shape_setPosition(VALUE self, VALUE x, VALUE y)
{
	GET_SHAPE;
	shape->getShape()->setPosition(sf::Vector2f(NUM2DBL(x), NUM2DBL(y)));
	shape->rX = x;
	shape->rY = y;
	return self;
}

VALUE rb_Shape_getZ(VALUE self)
{
	GET_SHAPE;
	return shape->rZ;
}

VALUE rb_Shape_setZ(VALUE self, VALUE val)
{
	GET_SHAPE;
	NUM2LONG(val);
	shape->rZ = val;
	return self;
}

VALUE rb_Shape_getOX(VALUE self)
{
	GET_SHAPE;
	return shape->rOX;
}

VALUE rb_Shape_setOX(VALUE self, VALUE val)
{
	GET_SHAPE;
	shape->getShape()->setOrigin(sf::Vector2f(NUM2DBL(val), NUM2DBL(shape->rOY)));
	shape->rOX = val;
	return self;
}

VALUE rb_Shape_getOY(VALUE self)
{
	GET_SHAPE;
	return shape->rOY;
}

VALUE rb_Shape_setOY(VALUE self, VALUE val)
{
	GET_SHAPE;
	shape->getShape()->setOrigin(sf::Vector2f(NUM2DBL(shape->rOX), NUM2DBL(val)));
	shape->rOY = val;
	return self;
}

VALUE rb_Shape_setOrigin(VALUE self, VALUE ox, VALUE oy)
{
	GET_SHAPE;
	shape->getShape()->setOrigin(sf::Vector2f(NUM2DBL(ox), NUM2DBL(oy)));
	shape->rOX = ox;
	shape->rOY = oy;
	return self;
}

VALUE rb_Shape_getAngle(VALUE self)
{
	GET_SHAPE;
	return shape->rAngle;
}

VALUE rb_Shape_setAngle(VALUE self, VALUE val)
{
	GET_SHAPE;
	float angle = NUM2DBL(val);
	if (angle >= 360 || angle < 0)
	{
		angle = fmod(angle, 360);
		if (angle == (long)angle)
			shape->rAngle = LONG2NUM((long)angle);
		else
			shape->rAngle = DBL2NUM(angle);
	}
	else
		shape->rAngle = val;
	shape->getShape()->setRotation(angle);
	return self;
}

VALUE rb_Shape_getZoomX(VALUE self)
{
	GET_SHAPE;
	return shape->rZoomX;
}

VALUE rb_Shape_setZoomX(VALUE self, VALUE val)
{
	GET_SHAPE;
	shape->getShape()->setScale(sf::Vector2f(NUM2DBL(val), NUM2DBL(shape->rZoomY)));
	shape->rZoomX = val;
	return self;
}

VALUE rb_Shape_getZoomY(VALUE self)
{
	GET_SHAPE;
	return shape->rZoomY;
}

VALUE rb_Shape_setZoomY(VALUE self, VALUE val)
{
	GET_SHAPE;
	shape->getShape()->setScale(sf::Vector2f(NUM2DBL(shape->rZoomX), NUM2DBL(val)));
	shape->rZoomY = val;
	return self;
}

VALUE rb_Shape_setZoom(VALUE self, VALUE val)
{
	GET_SHAPE;
	float zoom = NUM2DBL(val);
	shape->getShape()->setScale(sf::Vector2f(zoom, zoom));
	shape->rZoomX = val;
	shape->rZoomY = val;
	return self;
}

VALUE rb_Shape_getIndex(VALUE self)
{
	GET_SHAPE;
	return LONG2NUM(shape->getIndex());
}

VALUE rb_Shape_getViewport(VALUE self)
{
	GET_SHAPE;
	return shape->rViewport;
}

VALUE rb_Shape_getVisible(VALUE self)
{
	GET_SHAPE;
	return shape->getVisible() ? Qtrue : Qfalse;
}

VALUE rb_Shape_setVisible(VALUE self, VALUE val)
{
	GET_SHAPE;
	shape->setVisible(RTEST(val));
	return self;
}

VALUE rb_Shape_getPointCount(VALUE self)
{
	GET_SHAPE;
	return LONG2NUM(shape->getShape()->getPointCount());
}

VALUE rb_Shape_setPointCount(VALUE self, VALUE val)
{
	GET_SHAPE;
	sf::ConvexShape* convex = dynamic_cast<sf::ConvexShape*>(shape->getShape());
	if (convex != nullptr)
	{
		convex->setPointCount(NUM2ULONG(val));
	}
	else
	{
		sf::CircleShape* circle = dynamic_cast<sf::CircleShape*>(shape->getShape());
		if (circle != nullptr)
		{
			circle->setPointCount(NUM2ULONG(val));
		}
	}
	return self;
}

VALUE rb_Shape_getRadius(VALUE self)
{
	GET_SHAPE;
	sf::CircleShape* circle = dynamic_cast<sf::CircleShape*>(shape->getShape());
	if (circle != nullptr)
	{
		return DBL2NUM(circle->getRadius());
	}
	return LONG2NUM(-1);
}

VALUE rb_Shape_setRadius(VALUE self, VALUE val)
{
	GET_SHAPE;
	sf::CircleShape* circle = dynamic_cast<sf::CircleShape*>(shape->getShape());
	if (circle != nullptr)
	{
		circle->setRadius(NUM2DBL(val));
	}
	return self;
}

VALUE rb_Shape_getType(VALUE self)
{
	GET_SHAPE;
	return shape->rShapeType;
}

VALUE rb_Shape_getPoint(VALUE self, VALUE index)
{
	GET_SHAPE;
	long lindex = NUM2LONG(index);
	if (lindex < 0 || lindex >= shape->getShape()->getPointCount())
		return Qnil;
	sf::Vector2f point = shape->getShape()->getPoint(lindex);
	VALUE arr = rb_ary_new();
	rb_ary_push(arr, DBL2NUM(point.x));
	rb_ary_push(arr, DBL2NUM(point.y));
	return arr;
}

VALUE rb_Shape_setPoint(VALUE self, VALUE index, VALUE x, VALUE y)
{
	GET_SHAPE;
	sf::ConvexShape* convex = dynamic_cast<sf::ConvexShape*>(shape->getShape());
	if (convex != nullptr)
	{
		long lindex = NUM2LONG(index);
		if (lindex < 0 || lindex >= shape->getShape()->getPointCount())
			return self;
		convex->setPoint(lindex, sf::Vector2f(NUM2DBL(x), NUM2DBL(y)));
	}
	return self;
}

VALUE rb_Shape_getColor(VALUE self)
{
	GET_SHAPE;
	if (shape->rColor == Qnil)
	{
		sf::Color col = shape->getShape()->getFillColor();
		VALUE args[4] = {
			LONG2NUM(col.r),
			LONG2NUM(col.g),
			LONG2NUM(col.b),
			LONG2NUM(col.a)
		};
		shape->rColor = rb_class_new_instance(4, args, rb_cColor);
	}
	return shape->rColor;
}

VALUE rb_Shape_setColor(VALUE self, VALUE val)
{
	GET_SHAPE;
	sf::Color* color = rb_Color_get_color(val);
	shape->getShape()->setFillColor(*color);
	shape->rColor = val;
	return self;
}

VALUE rb_Shape_getOutlineColor(VALUE self)
{
	GET_SHAPE;
	if (shape->rOutlineColor == Qnil)
	{
		sf::Color col = shape->getShape()->getFillColor();
		VALUE args[4] = {
			LONG2NUM(col.r),
			LONG2NUM(col.g),
			LONG2NUM(col.b),
			LONG2NUM(col.a)
		};
		shape->rOutlineColor = rb_class_new_instance(4, args, rb_cColor);
	}
	return shape->rOutlineColor;
}

VALUE rb_Shape_setOutlineColor(VALUE self, VALUE val)
{
	GET_SHAPE;
	sf::Color* color = rb_Color_get_color(val);
	shape->getShape()->setOutlineColor(*color);
	shape->rOutlineColor = val;
	return self;
}

VALUE rb_Shape_getOutlineThickness(VALUE self)
{
	GET_SHAPE;
	return shape->rOutlineThickness;
}

VALUE rb_Shape_setOutlineThickness(VALUE self, VALUE val)
{
	GET_SHAPE;
	shape->getShape()->setOutlineThickness(NUM2DBL(val));
	shape->rOutlineThickness = val;
	return self;
}

VALUE rb_Shape_getWidth(VALUE self)
{
	GET_SHAPE;
	return LONG2NUM(shape->getShape()->getGlobalBounds().width);
}

VALUE rb_Shape_setWidth(VALUE self, VALUE val)
{
	GET_SHAPE;
	sf::RectangleShape* rectangle = dynamic_cast<sf::RectangleShape*>(shape->getShape());
	if (rectangle != nullptr)
	{
		sf::Vector2f size = rectangle->getSize();
		size.x = NUM2DBL(val);
		rectangle->setSize(size);
	}
	return self;
}

VALUE rb_Shape_getHeight(VALUE self)
{
	GET_SHAPE;
	return LONG2NUM(shape->getShape()->getGlobalBounds().height);
}

VALUE rb_Shape_setHeight(VALUE self, VALUE val)
{
	GET_SHAPE;
	sf::RectangleShape* rectangle = dynamic_cast<sf::RectangleShape*>(shape->getShape());
	if (rectangle != nullptr)
	{
		sf::Vector2f size = rectangle->getSize();
		size.y = NUM2DBL(val);
		rectangle->setSize(size);
	}
	return self;
}

VALUE rb_Shape_getShader(VALUE self)
{
	GET_SHAPE;
	return shape->rRenderStates;
}

VALUE rb_Shape_setShader(VALUE self, VALUE shader)
{
	sf::RenderStates* render_state;
	GET_SHAPE;
	if (rb_obj_is_kind_of(shader, rb_cBlendMode) == Qtrue)
	{
		Data_Get_Struct(shader, sf::RenderStates, render_state);
		if (render_state != nullptr)
		{
			shape->rRenderStates = shader;
			shape->setRenderState(render_state);
			return self;
		}
	}
	shape->rRenderStates = Qnil;
	shape->setRenderState(nullptr);
	return self;
}

VALUE rb_Shape_Copy(VALUE self)
{
	rb_raise(rb_eRGSSError, "Shapes cannot be cloned or duplicated.");
	return self;
}

CShape_Element* rb_Shape_get_shape(VALUE self)
{
	rb_Shape_test_shape(self);
	GET_SHAPE;
	return shape;
}

void rb_Shape_test_shape(VALUE self)
{
	if (rb_obj_is_kind_of(self, rb_cShape) != Qtrue)
	{
		rb_raise(rb_eTypeError, "Expected Shape got %s.", RSTRING_PTR(rb_class_name(CLASS_OF(self))));
	}
}

VALUE rb_Shape_DisposeFromViewport(VALUE self)
{
	if (RDATA(self)->data == nullptr)
		return self;
	GET_SHAPE;
	RDATA(self)->data = nullptr;
	rb_Shape_Free(reinterpret_cast<void*>(shape));
	return self;
}
