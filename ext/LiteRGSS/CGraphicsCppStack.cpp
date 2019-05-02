#include "CGraphicsCppStack.h"
#include "CGraphicsStackItem.h"

CGraphicsCppStack::~CGraphicsCppStack() {
	syncFromRawData({});
}

void CGraphicsCppStack::detach() {
	stack.clear();
}

void CGraphicsCppStack::syncFromRawData(std::vector<CGraphicsStackItem*> data, bool overrideOriginStack) { 
	if(overrideOriginStack) {
		for(auto& it : stack) {
			it->CGraphicsCppStackItem::overrideOriginStack();
		}
	}
	stack = std::move(data);
}

bool CGraphicsCppStack::remove(CGraphicsStackItem* el) {
	return stack.remove(el);
}

void CGraphicsCppStack::add(CGraphicsStackItem* el) {
	stack.push_back(el);
}
