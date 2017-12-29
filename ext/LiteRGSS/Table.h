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

#endif