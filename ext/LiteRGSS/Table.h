#ifndef L_TABLE_HEADER
#define L_TABLE_HEADER

struct rb_Table_Struct_Header {
    unsigned int dim;
    unsigned int xsize;
    unsigned int ysize;
    unsigned int zsize;
    unsigned int data_size;
};

struct rb_Table_Struct {
    rb_Table_Struct_Header header;
    short* heap;
};

VALUE rb_Table_initialize(int argc, VALUE* argv, VALUE self);
VALUE rb_Table_get(int argc, VALUE* argv, VALUE self);
VALUE rb_Table_set(int argc, VALUE* argv, VALUE self);
VALUE rb_Table_xSize(VALUE self);
VALUE rb_Table_ySize(VALUE self);
VALUE rb_Table_zSize(VALUE self);
VALUE rb_Table_dim(VALUE self);
VALUE rb_Table_resize(int argc, VALUE* argv, VALUE self);
VALUE rb_Table_Load(VALUE self, VALUE str);
VALUE rb_Table_Save(VALUE self, VALUE limit);
VALUE rb_Table_Fill(VALUE self, VALUE val);
VALUE rb_Table_Copy(VALUE self, VALUE source, VALUE dest_offset_x, VALUE dest_offset_y);
VALUE rb_Table_CopyModulo(VALUE self, VALUE source, VALUE source_origin_x, VALUE source_origin_y, VALUE dest_offset_x, VALUE dest_offset_y, VALUE dest_width, VALUE dest_height);

rb_Table_Struct* rb_Table_get_table(VALUE self);
void rb_Table_test_table(VALUE self);

void rb_Table_internal_copyLine(short* &xheap1, short* &xheap2, long ini_x, long max_x);
void rb_Table_internal_copyModuloYpart(short* xheap1, short* yheap2, long ox2, long target_x, long offsetx, long src_xsize, long m);

#endif