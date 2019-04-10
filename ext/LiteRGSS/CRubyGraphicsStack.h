#ifndef CRubyGraphicsStack_H
#define CRubyGraphicsStack_H

#include "ruby.h"

class CDrawable_Element;
class CGraphicsStack_Element;
class CRubyGraphicsStack {
public:
    CRubyGraphicsStack(VALUE self);
    virtual ~CRubyGraphicsStack();

    void add(VALUE el);
    void remove(VALUE el);
    void syncStackCppFromRuby(CGraphicsStack_Element& destStack);
    void clear();

    VALUE self() { return m_table; }

private:
    VALUE m_table = Qnil;
};

#endif