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
            std::cout << "Drawable removed from graphic stack" << std::endl;
            origin_stack->erase(it);
        } else {
            std::cout << "Drawable NOT FOUND in graphic stack" << std::endl;
        }
    }

    auto it = std::find(Graphics_stack.begin(), Graphics_stack.end(), this);
    if(it != Graphics_stack.end()) {
        std::cout << "Drawable removed from global graphic stack" << std::endl;
        Graphics_stack.erase(it);
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
   
    setOriginStack(nullptr);
    CRect_Element* rect = getLinkedRect();
    if(rect != nullptr) {
        rect->setElement(nullptr);
    }

    /* Suppression du drawable de ses stacks */
	VALUE viewport = rViewport;
	VALUE table;
	if (NIL_P(viewport)) {
		table = rb_ivar_get(rb_mGraphics, rb_iElementTable);
        //std::cout << "Table element No Viewport :" << table << std::endl;
    } else {
		table = rb_ivar_get(viewport, rb_iElementTable);
        //std::cout << "Table element Viewport :" << table << std::endl;
    }
    if(rb_ary_includes(table, self) == Qtrue) {
	    rb_ary_delete(table, self);
        std::cout << "Deleted." << std::endl;
    } else {
        std::cout << "NOT FOUND" << std::endl;
    }
    RDATA(self)->data = nullptr;
}

void CDrawable_Element::setIndex(unsigned long nindex)
{
    index = nindex;
}

unsigned long CDrawable_Element::getIndex()
{
    return index;
}