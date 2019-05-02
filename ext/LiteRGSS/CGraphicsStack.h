#ifndef CGraphicsStack_H
#define CGraphicsStack_H
#include <memory>

#include "CGraphicsCppStack.h"
#include "CGraphicsRubyStack.h"
#include "CGraphicsStackItem.h"

class CGraphicsStack {
public:
	CGraphicsStack(VALUE table);
	virtual ~CGraphicsStack();
	
	void add(CGraphicsStackItem& el);
	void clear();
	void syncStackCppFromRuby();
	std::size_t size() const { return cppStack.size(); }

protected:	
	auto begin() const { return cppStack.begin(); }
	auto end() const { return cppStack.end(); }

private:
	CGraphicsCppStack cppStack;
	CGraphicsRubyStack rubyStack;
};
#endif