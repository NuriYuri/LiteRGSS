#include <iostream>
#include "CGraphicsStack.h"

extern VALUE rb_cDrawable;

CGraphicsStack::CGraphicsStack(VALUE table) :
	rubyStack(table) {
}

CGraphicsStack::~CGraphicsStack() {
	clear();
}

void CGraphicsStack::add(CGraphicsStackItem& el) {
	el.setOriginStack(cppStack, rubyStack);
}

void CGraphicsStack::clear() {
	for(auto& it : cppStack) {
		it->overrideOriginRubyStack();
		it->overrideOriginCppStack();
	}
	rubyStack.disposeAll();
	cppStack.detach();
}

void CGraphicsStack::syncStackCppFromRuby() {
	const long sz = RARRAY_LEN(rubyStack.self());
	VALUE* ori = RARRAY_PTR(rubyStack.self());
	auto data = std::vector<CGraphicsStackItem*>{};
	if(sz > 0) {
		data.reserve(sz);
	}
	for(long  i = 0; i < sz; i++) {
		const auto it = ori[i];
		if(rb_obj_is_kind_of(it, rb_cDrawable) == Qtrue && RDATA(it)->data != nullptr) {
			auto& element = *reinterpret_cast<CGraphicsStackItem*>(RDATA(it)->data);
			element.overrideOriginCppStack(&cppStack);
			data.push_back(&element);
		}		
	}
	cppStack.syncFromRawData(std::move(data), false);
}
