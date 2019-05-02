#ifndef CGraphicsRubyStack_H
#define CGraphicsRubyStack_H

#include "ruby.h"

class CGraphicsRubyStackItem;
class CGraphicsRubyStack {
public:
	CGraphicsRubyStack(VALUE self);
	~CGraphicsRubyStack() = default;

	void add(VALUE el);
	void remove(VALUE el);
	void disposeAll();

	VALUE self() const { return m_table; }

private:
	VALUE m_table = Qnil;
};

#endif