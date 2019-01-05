#include "LiteRGSS.h"

VALUE rb_cTable32 = Qnil;

void rb_Table32_Free(void* data)
{
    rb_Table32_Struct* table = reinterpret_cast<rb_Table32_Struct*>(data);
    if(table != nullptr)
    {
        //std::cout << "TH : " << table->heap << std::endl;
        if(table->heap != nullptr)
            delete[] table->heap;
        table->heap = nullptr;
        delete table;
    }
}

VALUE rb_Table32_Alloc(VALUE klass)
{
    return Data_Wrap_Struct(klass, NULL, rb_Table32_Free, new rb_Table32_Struct());
}

void Init_Table32()
{
    rb_cTable32 = rb_define_class("Table32", rb_cObject);
    rb_define_alloc_func(rb_cTable32, rb_Table32_Alloc);
    rb_define_method(rb_cTable32, "initialize", _rbf rb_Table32_initialize, -1);
    rb_define_method(rb_cTable32, "[]", _rbf rb_Table32_get, -1);
    rb_define_method(rb_cTable32, "[]=", _rbf rb_Table32_set, -1);
    rb_define_method(rb_cTable32, "xsize", _rbf rb_Table32_xSize, 0);
    rb_define_method(rb_cTable32, "ysize", _rbf rb_Table32_ySize, 0);
    rb_define_method(rb_cTable32, "zsize", _rbf rb_Table32_zSize, 0);
    rb_define_method(rb_cTable32, "dim", _rbf rb_Table32_dim, 0);
    rb_define_method(rb_cTable32, "resize", _rbf rb_Table32_resize, -1);
    rb_define_method(rb_cTable32, "fill", _rbf rb_Table32_Fill, 1);

    rb_define_method(rb_cTable32, "_dump", _rbf rb_Table32_Save, 1);
    rb_define_singleton_method(rb_cTable32, "_load", _rbf rb_Table32_Load, 1);
}


VALUE rb_Table32_initialize(int argc, VALUE* argv, VALUE self)
{
    auto& table = rb::Get<rb_Table32_Struct>(self);
    switch(argc)
    {
        case 1:
            table.header.xsize = rb_num2ulong(argv[0]);
            table.header.ysize = 1;
            table.header.zsize = 1;
            break;
        case 2:
            table.header.xsize = rb_num2ulong(argv[0]);
            table.header.ysize = rb_num2ulong(argv[1]);
            table.header.zsize = 1;
            break;
        case 3:
            table.header.xsize = rb_num2ulong(argv[0]);
            table.header.ysize = rb_num2ulong(argv[1]);
            table.header.zsize = rb_num2ulong(argv[2]);
            break;
        default:
            rb_raise(rb_eRGSSError, "Table32 can be 1D, 2D or 3D but nothing else, requested dimension : %dD", argc);
            return Qnil;
    }
    if(table.header.xsize == 0)
        table.header.xsize = 1;
    if(table.header.ysize == 0)
        table.header.ysize = 1;
    if(table.header.zsize == 0)
        table.header.zsize = 1;
    table.header.dim = argc;
    table.header.data_size = table.header.xsize * table.header.ysize * table.header.zsize;
    table.heap = new int32_t[table.header.data_size]();
    return self;
}

VALUE rb_Table32_get(int argc, VALUE* argv, VALUE self)
{
    auto& table = rb::Get<rb_Table32_Struct>(self);
    VALUE rx, ry, rz;
    unsigned long x, y, z;
    rb_scan_args(argc, argv, "12", &rx, &ry, &rz);
    x = rb_num2ulong(rx);
    y = NIL_P(ry) ? 0 : rb_num2ulong(ry);
    z = NIL_P(rz) ? 0 : rb_num2ulong(rz);
    if(x >= table.header.xsize || y >= table.header.ysize || z >= table.header.zsize)
        return Qnil;
    return rb_int2inum(table.heap[x + (y * table.header.xsize) + (z * table.header.xsize * table.header.ysize)]);
}

VALUE rb_Table32_set(int argc, VALUE* argv, VALUE self)
{
    auto& table = rb::Get<rb_Table32_Struct>(self);
    VALUE rx, ry, rz, rv;
    unsigned long x, y, z;
    long v;
    rb_scan_args(argc, argv, "22", &rx, &ry, &rz, &rv);
    x = rb_num2ulong(rx);
    if(NIL_P(rz))
    {
        v = RB_NUM2LONG(ry);
        z = y = 0;
    }
    else if(NIL_P(rv))
    {
        v = RB_NUM2LONG(rz);
        y = rb_num2ulong(ry);
        z = 0;
    }
    else
    {
        y = rb_num2ulong(ry);
        z = rb_num2ulong(rz);
        v = RB_NUM2LONG(rv);
    }
    if(x >= table.header.xsize || y >= table.header.ysize || z >= table.header.zsize)
        return Qnil;
    table.heap[x + (y * table.header.xsize) + (z * table.header.xsize * table.header.ysize)] = v;
    return self;
}

VALUE rb_Table32_xSize(VALUE self)
{
    auto& table = rb::Get<rb_Table32_Struct>(self);
    return rb_uint2inum(table.header.xsize);
}

VALUE rb_Table32_ySize(VALUE self)
{
    auto& table = rb::Get<rb_Table32_Struct>(self);
    return rb_uint2inum(table.header.ysize);
}

VALUE rb_Table32_zSize(VALUE self)
{
    auto& table = rb::Get<rb_Table32_Struct>(self);
    return rb_uint2inum(table.header.zsize);
}

VALUE rb_Table32_dim(VALUE self)
{
    auto& table = rb::Get<rb_Table32_Struct>(self);
    return rb_uint2inum(table.header.dim);
}

void table_copy(int32_t* dheap, int32_t* sheap, unsigned long dxsize, unsigned long dysize,
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

VALUE rb_Table32_resize(int argc, VALUE* argv, VALUE self)
{
    auto& table = rb::Get<rb_Table32_Struct>(self);
    auto table2 = table;
    rb_Table32_initialize(argc, argv, self);
    // Copying data
    table_copy(table.heap, table2.heap,
        table.header.xsize, table.header.ysize, table.header.zsize,
        table2.header.xsize, table2.header.ysize, table2.header.zsize);
    // Freeing heap
    delete[] table2.heap;
    return self;
}


VALUE rb_Table32_Load(VALUE self, VALUE str)
{
    rb_check_type(str, T_STRING);
    rb_Table32_Struct* table = reinterpret_cast<rb_Table32_Struct*>(RSTRING_PTR(str));
    VALUE arr[3];
    arr[0] = RB_UINT2NUM(table->header.xsize);
    arr[1] = RB_UINT2NUM(table->header.ysize);
    arr[2] = RB_UINT2NUM(table->header.zsize);
    table->header.dim = normalize_long(table->header.dim, 1, 3);
    VALUE rtable = rb_class_new_instance(table->header.dim, arr, self);
    Data_Get_Struct(rtable, rb_Table32_Struct, table);
    memcpy(table->heap, RSTRING_PTR(str) + sizeof(rb_Table32_Struct_Header), table->header.data_size * sizeof(long));
    return rtable;
}

VALUE rb_Table32_Save(VALUE self, VALUE limit)
{
    auto& table = rb::Get<rb_Table32_Struct>(self);
    VALUE str1 = rb_str_new(reinterpret_cast<char*>(&table.header), sizeof(rb_Table32_Struct_Header));
    VALUE str2 = rb_str_new(reinterpret_cast<char*>(table.heap), table.header.data_size * sizeof(long));
    return rb_str_concat(str1, str2);
}

VALUE rb_Table32_Fill(VALUE self, VALUE val)
{
    auto& table = rb::Get<rb_Table32_Struct>(self);
    long v = RB_NUM2LONG(val);
    unsigned long sz = table.header.data_size;
	int32_t* data = table.heap;
    for(unsigned long i = 0;i < sz;i++)
    {
        data[i] = v;
    }
    return self;
}
