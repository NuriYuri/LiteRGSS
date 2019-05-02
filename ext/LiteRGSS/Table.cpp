#include "LiteRGSS.h"

VALUE rb_cTable = Qnil;

void rb_Table_Free(void* data)
{
	rb_Table_Struct* table = reinterpret_cast<rb_Table_Struct*>(data);
	if(table != nullptr)
	{
		if(table->heap != nullptr)
			delete[] table->heap;
		table->heap = nullptr;
		delete table;
	}
}

VALUE rb_Table_Alloc(VALUE klass)
{
	return Data_Wrap_Struct(klass, NULL, rb_Table_Free, new rb_Table_Struct());
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
	rb_define_method(rb_cTable, "copy_modulo", _rbf rb_Table_CopyModulo, 7);

	rb_define_method(rb_cTable, "_dump", _rbf rb_Table_Save, 1);
	rb_define_singleton_method(rb_cTable, "_load", _rbf rb_Table_Load, 1);
}


VALUE rb_Table_initialize(int argc, VALUE* argv, VALUE self)
{
	auto& table = rb::Get<rb_Table_Struct>(self);
	switch(argc)
	{
		case 1:
			table.header.xsize = NUM2ULONG(argv[0]);
			table.header.ysize = 1;
			table.header.zsize = 1;
			break;
		case 2:
			table.header.xsize = NUM2ULONG(argv[0]);
			table.header.ysize = NUM2ULONG(argv[1]);
			table.header.zsize = 1;
			break;
		case 3:
			table.header.xsize = NUM2ULONG(argv[0]);
			table.header.ysize = NUM2ULONG(argv[1]);
			table.header.zsize = NUM2ULONG(argv[2]);
			break;
		default:
			rb_raise(rb_eRGSSError, "Table can be 1D, 2D or 3D but nothing else, requested dimension : %dD", argc);
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
	if(table.heap != nullptr) {
		delete[] table.heap;
	}
	table.heap = new short[table.header.data_size]();
	return self;
}

VALUE rb_Table_get(int argc, VALUE* argv, VALUE self)
{
	auto& table = rb::Get<rb_Table_Struct>(self);
	VALUE rx, ry, rz;
	unsigned long x, y, z;
	rb_scan_args(argc, argv, "12", &rx, &ry, &rz);
	x = NUM2ULONG(rx);
	y = NIL_P(ry) ? 0 : NUM2ULONG(ry);
	z = NIL_P(rz) ? 0 : NUM2ULONG(rz);
	if(x >= table.header.xsize || y >= table.header.ysize || z >= table.header.zsize)
		return Qnil;
	return rb_int2inum(table.heap[x + (y * table.header.xsize) + (z * table.header.xsize * table.header.ysize)]);
}

VALUE rb_Table_set(int argc, VALUE* argv, VALUE self)
{
	auto& table = rb::Get<rb_Table_Struct>(self);
	VALUE rx, ry, rz, rv;
	unsigned long x, y, z;
	short v;
	rb_scan_args(argc, argv, "22", &rx, &ry, &rz, &rv);
	x = NUM2ULONG(rx);
	if(NIL_P(rz))
	{
		v = NUM2SHORT(ry);
		z = y = 0;
	}
	else if(NIL_P(rv))
	{
		v = NUM2SHORT(rz);
		y = NUM2ULONG(ry);
		z = 0;
	}
	else
	{
		y = NUM2ULONG(ry);
		z = NUM2ULONG(rz);
		v = NUM2SHORT(rv);
	}
	if(x >= table.header.xsize || y >= table.header.ysize || z >= table.header.zsize)
		return Qnil;
	table.heap[x + (y * table.header.xsize) + (z * table.header.xsize * table.header.ysize)] = v;
	return self;
}

VALUE rb_Table_xSize(VALUE self)
{
	auto& table = rb::Get<rb_Table_Struct>(self);
	return rb_uint2inum(table.header.xsize);
}

VALUE rb_Table_ySize(VALUE self)
{
	auto& table = rb::Get<rb_Table_Struct>(self);
	return rb_uint2inum(table.header.ysize);
}

VALUE rb_Table_zSize(VALUE self)
{
	auto& table = rb::Get<rb_Table_Struct>(self);
	return rb_uint2inum(table.header.zsize);
}

VALUE rb_Table_dim(VALUE self)
{
	auto& table = rb::Get<rb_Table_Struct>(self);
	return rb_uint2inum(table.header.dim);
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
	auto& table = rb::Get<rb_Table_Struct>(self);
	auto table2 = table;
	table2.heap = nullptr;
	rb_Table_initialize(argc, argv, self);
	// Copying data
	table_copy(table.heap, table2.heap,
		table.header.xsize, table.header.ysize, table.header.zsize,
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
	arr[0] = UINT2NUM(table->header.xsize);
	arr[1] = UINT2NUM(table->header.ysize);
	arr[2] = UINT2NUM(table->header.zsize);
	table->header.dim = normalize_long(table->header.dim, 1, 3);
	VALUE rtable = rb_class_new_instance(table->header.dim, arr, self);
	Data_Get_Struct(rtable, rb_Table_Struct, table);
	memcpy(table->heap, RSTRING_PTR(str) + sizeof(rb_Table_Struct_Header), table->header.data_size * sizeof(std::remove_pointer<decltype(table->heap)>::type));
	return rtable;
}

VALUE rb_Table_Save(VALUE self, VALUE limit)
{
	auto& table = rb::Get<rb_Table_Struct>(self);
	VALUE str1 = rb_str_new(reinterpret_cast<char*>(&table.header), sizeof(rb_Table_Struct_Header));
	VALUE str2 = rb_str_new(reinterpret_cast<char*>(table.heap), table.header.data_size * sizeof(std::remove_pointer<decltype(table.heap)>::type));
	return rb_str_concat(str1, str2);
}

VALUE rb_Table_Fill(VALUE self, VALUE val)
{
	auto& table = rb::Get<rb_Table_Struct>(self);
	short v = NUM2SHORT(val);
	short* max_data = table.heap + table.header.data_size;
	short* data = table.heap;
	for(;data < max_data;data++)
		*data = v;
	return self;
}

VALUE rb_Table_Copy(VALUE self, VALUE source, VALUE dest_offset_x, VALUE dest_offset_y)
{
	auto& table = rb::Get<rb_Table_Struct>(self);
	auto& source_table = rb::GetSafe<rb_Table_Struct>(source, rb_cTable);
	long offsetx = NUM2LONG(dest_offset_x);
	long offsety = NUM2LONG(dest_offset_y);
	if (offsetx < 0 || offsetx >= table.header.xsize)
		return Qfalse;
	if (offsety < 0 || offsety >= table.header.ysize)
		return Qfalse;

	// Usefull variables
	long target_z = table.header.zsize;
	if (source_table.header.zsize < target_z)
		target_z = source_table.header.zsize;

	long target_x = offsetx + source_table.header.xsize;
	if (table.header.xsize < target_x)
		target_x = table.header.xsize;

	long target_y = offsety + source_table.header.ysize;
	if (table.header.ysize < target_y)
		target_y = table.header.ysize;

	long deltay = table.header.xsize;
	long deltaz = deltay * table.header.ysize;

	long deltay2 = source_table.header.xsize;
	long deltaz2 = deltay2 * source_table.header.ysize;

	short *zheap1, *zheap2, *yheap1, *yheap2, *xheap1, *xheap2;

	zheap1 = table.heap + (offsety * deltay + offsetx);
	zheap2 = source_table.heap;

	// Copy loops
	long y, z;
	for (z = 0; z < target_z; z++)
	{
		yheap1 = zheap1;
		yheap2 = zheap2;

		for (y = offsety; y < target_y; y++)
		{
			xheap1 = yheap1;
			xheap2 = yheap2;

			rb_Table_internal_copyLine(xheap1, xheap2, offsetx, target_x);

			yheap1 += deltay;
			yheap2 += deltay2;
		}

		zheap1 += deltaz;
		zheap2 += deltaz2;
	}

	return Qtrue;
}

VALUE rb_Table_CopyModulo(VALUE self, VALUE source, VALUE source_origin_x, VALUE source_origin_y, VALUE dest_offset_x, VALUE dest_offset_y, VALUE dest_width, VALUE dest_height)
{
	auto& table = rb::Get<rb_Table_Struct>(self);
	auto& source_table = rb::GetSafe<rb_Table_Struct>(source, rb_cTable);
	long offsetx = NUM2LONG(dest_offset_x);
	long offsety = NUM2LONG(dest_offset_y);
	long ox2 = NUM2LONG(source_origin_x);
	long oy2 = NUM2LONG(source_origin_y);
	if (offsetx < 0 || offsetx >= table.header.xsize)
		return Qfalse;
	if (offsety < 0 || offsety >= table.header.ysize)
		return Qfalse;
	if (ox2 < 0 || ox2 >= source_table.header.xsize)
		return Qfalse;
	if (oy2 < 0 || oy2 >= source_table.header.ysize)
		return Qfalse;

	long src_xsize = (long)source_table.header.xsize;
	long src_ysize = (long)source_table.header.ysize;

	// Usefull variables


	long target_z = table.header.zsize;
	if (source_table.header.zsize < target_z)
		target_z = source_table.header.zsize;

	long target_x = offsetx + NUM2LONG(dest_width);
	if (table.header.xsize < target_x)
		target_x = table.header.xsize;

	long target_y = offsety + NUM2LONG(dest_height);
	if (table.header.ysize < target_y)
		target_y = table.header.ysize;

	long n = (target_y - offsety - src_ysize + oy2) / src_ysize;
	long m = (target_x - offsetx - src_xsize + ox2) / src_xsize;
	if (n < 0)
		n = 0;
	if (m < 0)
		m = 0;

	long deltay = table.header.xsize;
	long deltaz = deltay * table.header.ysize;

	long deltay2 = source_table.header.xsize;
	long deltaz2 = deltay2 * source_table.header.ysize;

	short *zheap1, *zheap2, *yheap1, *yheap2, *xheap1, *xheap2;
	long target_x2, target_y2;

	zheap1 = table.heap + (offsety * deltay + offsetx);
	zheap2 = source_table.heap;

	// Copy loops
	long j, y, z;

	for (z = 0; z < target_z; z++)
	{
		yheap1 = zheap1;
		// Y Init Loop (top copied from oy2 to what it can copy in height)
		yheap2 = zheap2 + oy2 * deltay2;
		target_y2 = target_y - offsety;
		if (target_y2 > (src_ysize - oy2))
			target_y2 = (src_ysize - oy2);

		//printf("target_y2 = %d; %d %d\n", target_y2, src_ysize, target_y - offsety);

		for (y = 0; y < target_y2; y++)
		{
			rb_Table_internal_copyModuloYpart(yheap1, yheap2, ox2, target_x, offsetx, src_xsize, m);
			yheap1 += deltay;
			yheap2 += deltay2;
		}
		// End of Y init loop

		// Y middle loops
		for (j = 0; j < n; j++)
		{
			yheap2 = zheap2;
			for (y = 0; y < src_ysize; y++)
			{
				rb_Table_internal_copyModuloYpart(yheap1, yheap2, ox2, target_x, offsetx, src_xsize, m);
				yheap1 += deltay;
				yheap2 += deltay2;
			}
		}

		// Y end loop
		yheap2 = zheap2;
		target_y2 = target_y - offsety + oy2 - (n + 1) * src_ysize;
		if (target_y2 > src_ysize)
			target_y2 = src_ysize;

		for (y = 0; y < target_y2; y++)
		{
			rb_Table_internal_copyModuloYpart(yheap1, yheap2, ox2, target_x, offsetx, src_xsize, m);
			yheap1 += deltay;
			yheap2 += deltay2;
		}

		// End of Z loop
		zheap1 += deltaz;
		zheap2 += deltaz2;
	}
	return Qtrue;
}

void rb_Table_internal_copyLine(short* &xheap1, short* &xheap2, long ini_x, long max_x)
{
	short* max_xheap2 = xheap2 + (max_x - ini_x);
	for (; xheap2 < max_xheap2; xheap2++)//(int x = ini_x; x < max_x; x++)
	{
		*xheap1 = *xheap2;
		xheap1++;
		//xheap2++;
	}
}

void rb_Table_internal_copyModuloYpart(short* xheap1, short* yheap2, long ox2, long target_x, long offsetx, long src_xsize, long m)
{
	long i;
	// X Init Loop (line copied from ox2 to what it can copy in width)
	short* xheap2 = yheap2 + ox2;
	long target_x2 = target_x - offsetx;
	if (target_x2 > (src_xsize - ox2))
		target_x2 = (src_xsize - ox2);

	//printf("target_x2 = %d; %d %d %d\n", target_x2, src_xsize, target_x - offsetx, m);

	rb_Table_internal_copyLine(xheap1, xheap2, 0, target_x2);

	// X Middle loops
	for (i = 0; i < m; i++)
	{
		// here xheap1 should have been altered by rb_Table_internal_copyLine
		xheap2 = yheap2;
		rb_Table_internal_copyLine(xheap1, xheap2, 0, src_xsize);
	}

	// X End loop
	xheap2 = yheap2;
	target_x2 = target_x - offsetx + ox2 - (m + 1) * src_xsize;
	if (target_x2 > src_xsize) // If this condition validates, our computation is incorrect
	{
		//printf("Failure ! %d %d\n", target_x2, src_xsize);
		target_x2 = src_xsize;
	}

	//printf("target_x2 = %d; %d %d %d\n", target_x2, src_xsize, target_x - offsetx, m);

	rb_Table_internal_copyLine(xheap1, xheap2, 0, target_x2);
}
