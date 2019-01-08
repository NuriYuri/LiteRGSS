#include "CDrawable_Element.h"
#include "CRect_Element.h"
#include <iostream>

extern ID rb_iElementTable;
extern VALUE rb_mGraphics;
extern std::vector<CDrawable_Element*> Graphics_stack;

void CDrawable_Element::setOriginStack(std::vector<CDrawable_Element*> *o) 
{
    /* Removing from the old stack */
    if(origin_stack != nullptr)
    {
        auto it = std::find(origin_stack->begin(), origin_stack->end(), this);
        if(it != origin_stack->end()) {
            std::cout << "|C Drawable removed from " << (origin_stack == &Graphics_stack ? "GLOBAL " : "") << "graphic stack" << std::endl;
            origin_stack->erase(it);
        } else {
            std::cout << "|C NOT FOUND in "<< (origin_stack == &Graphics_stack ? "GLOBAL " : "") << "graphic stack" << std::endl;
        }
    }
    origin_stack = o;
    if(o != nullptr)
    {
        origin_stack->push_back(this);
        if(index == 0) {
            index = origin_stack->size();
        }
    }
}

CDrawable_Element::~CDrawable_Element() {
    std::cout << "Entering Drawable destructor for " << self << std::endl;
   
    if(origin_stack != nullptr) {
        std::cout << "|C stack = " << origin_stack->size() << std::endl;
    }

    setOriginStack(nullptr);
    CRect_Element* rect = getLinkedRect();
    if(rect != nullptr) {
        rect->setElement(nullptr);
    }

    /* Suppression du drawable de ses stacks */
	VALUE table = rb_ivar_get(NIL_P(rViewport) ? rb_mGraphics : rViewport, rb_iElementTable);
    if(rb_ary_delete(table, self) != Qnil) {
        std::cout << "|Ruby Deleted." << std::endl;
    } else {
        std::cout << "|Ruby NOT FOUND" << std::endl;
    }

    const auto sz = RARRAY_LEN(table);
    //TODO assert() ?
    std::cout << "|Ruby stack = " << sz << std::endl;
    RDATA(self)->data = nullptr;
}

unsigned long CDrawable_Element::getIndex()
{
    return index;
}