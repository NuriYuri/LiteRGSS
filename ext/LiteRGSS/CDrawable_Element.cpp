#include "CDrawable_Element.h"
#include <iostream>

CDrawable_Element::CDrawable_Element()
{
    origin_stack = nullptr;
    linkedRect = nullptr;
    index = 0;
}

void CDrawable_Element::setOriginStack(std::vector<CDrawable_Element*> *o) 
{
    /* Removing from the old stack */
    if(origin_stack != nullptr)
    {
        auto it = std::find(origin_stack->begin(), origin_stack->end(), this);
        if(it != origin_stack->end())
            origin_stack->erase(it);
    }
    origin_stack = o;
    if(o != nullptr)
    {
        origin_stack->push_back(this);
        if(index == 0)
            index = o->size();
    }
}


void CDrawable_Element::setIndex(unsigned long nindex)
{
    index = nindex;
}

unsigned long CDrawable_Element::getIndex()
{
    return index;
}