#include "CDrawable_Element.h"

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
        origin_stack->push_back(this);
}