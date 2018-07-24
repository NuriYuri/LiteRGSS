#ifndef L_TABLE32_HEADER
#define L_TABLE32_HEADER

struct rb_Table32_Struct_Header {
    unsigned int dim;
    unsigned int xsize;
    unsigned int ysize;
    unsigned int zsize;
    unsigned int data_size;
};

struct rb_Table32_Struct {
    rb_Table32_Struct_Header header;
    long* heap;
};

VALUE rb_Table32_initialize(int argc, VALUE* argv, VALUE self);
VALUE rb_Table32_get(int argc, VALUE* argv, VALUE self);
VALUE rb_Table32_set(int argc, VALUE* argv, VALUE self);
VALUE rb_Table32_xSize(VALUE self);
VALUE rb_Table32_ySize(VALUE self);
VALUE rb_Table32_zSize(VALUE self);
VALUE rb_Table32_dim(VALUE self);
VALUE rb_Table32_resize(int argc, VALUE* argv, VALUE self);
VALUE rb_Table32_Load(VALUE self, VALUE str);
VALUE rb_Table32_Save(VALUE self, VALUE limit);
VALUE rb_Table32_Fill(VALUE self, VALUE val);

#endif