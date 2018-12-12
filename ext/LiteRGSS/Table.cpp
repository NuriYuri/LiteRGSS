#include "LiteRGSS.h"

VALUE rb_cTable = Qnil;

#define GET_TABLE rb_Table_Struct* table; \
    Data_Get_Struct(self, rb_Table_Struct, table); \
    if(table == nullptr) \
        return Qnil;

void rb_Table_Free(void* data)
{
    rb_Table_Struct* table = reinterpret_cast<rb_Table_Struct*>(data);
    if(table != nullptr)
    {
        //std::cout << "TH : " << table->heap << std::endl;
        if(table->heap != nullptr)
            delete[] table->heap;
        table->heap = nullptr;
        delete table;
    }
}

VALUE rb_Table_Alloc(VALUE klass)
{
    rb_Table_Struct* table = new rb_Table_Struct();
    table->heap = nullptr;
    table->header.dim = 0;
    table->header.data_size = 0;
    return Data_Wrap_Struct(klass, NULL, rb_Table_Free, table);
}

void Init_Table()
{
    rb_cTable = rb_define_class("Table", rb_cObject);
    rb_define_alloc_func(rb_cTable, rb_Table_Alloc);
    rb_define_method(rb_cTable, "initialize", _rbf rb_Table_initialize, -1);
    rb_define_method(rb_cTable, "[]", _rbf rb_Table_get, -1);
    rb_define_method(rb_cTable, "[]=", _rbf rb_Table_set, -1);
    rb_define_method(rb_cTable, "xsize", _rbf rb_Table_xSize, 0);
    rb_define_method(rb_cTable, "ysize", _rbf rb_Table_ySize, 0);
    rb_define_method(rb_cTable, "zsize", _rbf rb_Table_zSize, 0);
    rb_define_method(rb_cTable, "dim", _rbf rb_Table_dim, 0);
    rb_define_method(rb_cTable, "resize", _rbf rb_Table_resize, -1);
    rb_define_method(rb_cTable, "fill", _rbf rb_Table_Fill, 1);
	rb_define_method(rb_cTable, "copy", _rbf rb_Table_Copy, 3);

    rb_define_method(rb_cTable, "_dump", _rbf rb_Table_Save, 1);
    rb_define_singleton_method(rb_cTable, "_load", _rbf rb_Table_Load, 1);
}


VALUE rb_Table_initialize(int argc, VALUE* argv, VALUE self)
{
    GET_TABLE
    switch(argc)
    {
        case 1:
            table->header.xsize = rb_num2ulong(argv[0]);
            table->header.ysize = 1;
            table->header.zsize = 1;
            break;
        case 2:
            table->header.xsize = rb_num2ulong(argv[0]);
            table->header.ysize = rb_num2ulong(argv[1]);
            table->header.zsize = 1;
            break;
        case 3:
            table->header.xsize = rb_num2ulong(argv[0]);
            table->header.ysize = rb_num2ulong(argv[1]);
            table->header.zsize = rb_num2ulong(argv[2]);
            break;
        default:
            rb_raise(rb_eRGSSError, "Table can be 1D, 2D or 3D but nothing else, requested dimension : %dD", argc);
            return Qnil;
    }
    if(table->header.xsize == 0)
        table->header.xsize = 1;
    if(table->header.ysize == 0)
        table->header.ysize = 1;
    if(table->header.zsize == 0)
        table->header.zsize = 1;
    table->header.dim = argc;
    table->header.data_size = table->header.xsize * table->header.ysize * table->header.zsize;
    table->heap = new short[table->header.data_size];
    return self;
}

VALUE rb_Table_get(int argc, VALUE* argv, VALUE self)
{
    GET_TABLE
    VALUE rx, ry, rz;
    unsigned long x, y, z;
    rb_scan_args(argc, argv, "12", &rx, &ry, &rz);
    x = rb_num2ulong(rx);
    y = NIL_P(ry) ? 0 : rb_num2ulong(ry);
    z = NIL_P(rz) ? 0 : rb_num2ulong(rz);
    if(x >= table->header.xsize || y >= table->header.ysize || z >= table->header.zsize)
        return Qnil;
    return rb_int2inum(table->heap[x + (y * table->header.xsize) + (z * table->header.xsize * table->header.ysize)]);
}

VALUE rb_Table_set(int argc, VALUE* argv, VALUE self)
{
    GET_TABLE
    VALUE rx, ry, rz, rv;
    unsigned long x, y, z;
    short v;
    rb_scan_args(argc, argv, "22", &rx, &ry, &rz, &rv);
    x = rb_num2ulong(rx);
    if(NIL_P(rz))
    {
        v = RB_NUM2SHORT(ry);
        z = y = 0;
    }
    else if(NIL_P(rv))
    {
        v = RB_NUM2SHORT(rz);
        y = rb_num2ulong(ry);
        z = 0;
    }
    else
    {
        y = rb_num2ulong(ry);
        z = rb_num2ulong(rz);
        v = RB_NUM2SHORT(rv);
    }
    if(x >= table->header.xsize || y >= table->header.ysize || z >= table->header.zsize)
        return Qnil;
    table->heap[x + (y * table->header.xsize) + (z * table->header.xsize * table->header.ysize)] = v;
    return self;
}

VALUE rb_Table_xSize(VALUE self)
{
    GET_TABLE
    return rb_uint2inum(table->header.xsize);
}

VALUE rb_Table_ySize(VALUE self)
{
    GET_TABLE
    return rb_uint2inum(table->header.ysize);
}

VALUE rb_Table_zSize(VALUE self)
{
    GET_TABLE
    return rb_uint2inum(table->header.zsize);
}

VALUE rb_Table_dim(VALUE self)
{
    GET_TABLE
    return rb_uint2inum(table->header.dim);
}

void table_copy(short* dheap, short* sheap, unsigned long dxsize, unsigned long dysize, 
    unsigned long dzsize, unsigned long sxsize, unsigned long sysize, unsigned long szsize)
{
    unsigned long xsize, ysize, zsize, soy, soz, doy, doz, x, y, z;
    xsize = dxsize < sxsize ? dxsize : sxsize;
    ysize = dysize < sysize ? dysize : sysize;
    zsize = dzsize < szsize ? dzsize : szsize;
    for(z = 0; z < zsize; z++)
    {
        doz = z * dxsize * dysize;
        soz = z * sxsize * sysize; // optimisable => del soz, repl soz par soy
        for(y = 0; y < ysize; y++)
        {
            doy = doz + (y * dxsize);
            soy = soz + (y * sxsize); // optimisable => add after for{} soy += sxsize; 
            for(x = 0; x < xsize; x++)
            {
                dheap[doy++] = sheap[soy++];
            }
        }
    }
}

VALUE rb_Table_resize(int argc, VALUE* argv, VALUE self)
{
    GET_TABLE
    auto table2 = *table;
    rb_Table_initialize(argc, argv, self);
    // Copying data
    table_copy(table->heap, table2.heap,
        table->header.xsize, table->header.ysize, table->header.zsize,
        table2.header.xsize, table2.header.ysize, table2.header.zsize);
    // Freeing heap
    delete[] table2.heap;
    return self;
}


VALUE rb_Table_Load(VALUE self, VALUE str)
{
    rb_check_type(str, T_STRING);
    rb_Table_Struct* table = reinterpret_cast<rb_Table_Struct*>(RSTRING_PTR(str));
    VALUE arr[3];
    arr[0] = RB_UINT2NUM(table->header.xsize);
    arr[1] = RB_UINT2NUM(table->header.ysize);
    arr[2] = RB_UINT2NUM(table->header.zsize);
    table->header.dim = normalize_long(table->header.dim, 1, 3);
    VALUE rtable = rb_class_new_instance(table->header.dim, arr, self);
    Data_Get_Struct(rtable, rb_Table_Struct, table);
    memcpy(table->heap, RSTRING_PTR(str) + sizeof(rb_Table_Struct_Header), table->header.data_size * sizeof(short));
    return rtable;
}

VALUE rb_Table_Save(VALUE self, VALUE limit)
{
    GET_TABLE
    VALUE str1 = rb_str_new(reinterpret_cast<char*>(&table->header), sizeof(rb_Table_Struct_Header));
    VALUE str2 = rb_str_new(reinterpret_cast<char*>(table->heap), table->header.data_size * sizeof(short));
    return rb_str_concat(str1, str2);
}

VALUE rb_Table_Fill(VALUE self, VALUE val)
{
    GET_TABLE
    short v = RB_NUM2SHORT(val);
	short* max_data = table->heap + table->header.data_size;
    short* data = table->heap;
    for(;data < max_data;data++)
		*data = v;
    return self;
}

VALUE rb_Table_Copy(VALUE self, VALUE source, VALUE dest_offset_x, VALUE dest_offset_y)
{
	GET_TABLE;
	rb_Table_Struct* source_table = rb_Table_get_table(source);
	long offsetx = NUM2LONG(dest_offset_x);
	long offsety = NUM2LONG(dest_offset_y);
	if (offsetx < 0 || offsetx >= table->header.xsize)
		return Qfalse;
	if (offsety < 0 || offsety >= table->header.ysize)
		return Qfalse;

	// Usefull variables
	long target_z = table->header.zsize;
	if (source_table->header.zsize < target_z)
		target_z = source_table->header.zsize;

	long target_x = offsetx + source_table->header.xsize;
	if (table->header.xsize < target_x)
		target_x = table->header.xsize;

	long target_y = offsety + source_table->header.ysize;
	if (table->header.ysize < target_y)
		target_y = table->header.ysize;

	long deltay = table->header.xsize;
	long deltaz = deltay * table->header.ysize;

	long deltay2 = source_table->header.xsize;
	long deltaz2 = deltay2 * source_table->header.ysize;

	short *zheap1, *zheap2, *yheap1, *yheap2, *xheap1, *xheap2;

	zheap1 = table->heap + (offsety * deltay + offsetx);
	zheap2 = source_table->heap;

	// Copy loops
	long x, y, z;
	for (z = 0; z < target_z; z++)
	{
		yheap1 = zheap1;
		yheap2 = zheap2;

		for (y = offsety; y < target_y; y++)
		{
			xheap1 = yheap1;
			xheap2 = yheap2;

			for (x = offsetx; x < target_x; x++)
			{
				*xheap1 = *xheap2;
				xheap1++;
				xheap2++;
			}

			yheap1 += deltay;
			yheap2 += deltay2;
		}

		zheap1 += deltaz;
		zheap2 += deltaz2;
	}

	return Qtrue;
}

rb_Table_Struct* rb_Table_get_table(VALUE self)
{
	rb_Table_test_table(self);
	rb_Table_Struct* table;
	Data_Get_Struct(self, rb_Table_Struct, table);
	return table;
}

void rb_Table_test_table(VALUE self)
{
	if (rb_obj_is_kind_of(self, rb_cTable) != Qtrue)
	{
		rb_raise(rb_eTypeError, "Expected Table got %s.", RSTRING_PTR(rb_class_name(CLASS_OF(self))));
	}
}