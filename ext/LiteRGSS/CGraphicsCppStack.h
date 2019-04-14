#ifndef CGraphicsCppStack_H
#define CGraphicsCppStack_H
#include "vector_tracker.h"

class CGraphicsStackItem;
class CGraphicsCppStack {
public:
    CGraphicsCppStack() = default;
    ~CGraphicsCppStack();

    void syncFromRawData(std::vector<CGraphicsStackItem*> data = {}, bool overrideOriginStack = true);

    void detach();
    bool remove(CGraphicsStackItem* el);
    void add(CGraphicsStackItem* el);
    std::size_t size() const { return stack.size(); }
    
    auto begin() const { return stack.begin(); }
    auto end() const { return stack.end(); }

private:
    vector_tracker<CGraphicsStackItem*> stack;
};
#endif