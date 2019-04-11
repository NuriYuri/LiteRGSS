#ifndef CDrawable_Element_H
#define CDrawable_Element_H
#include "ruby.h" // Fix Windows compilation
#include <algorithm>
#include "CGraphicsStackItem.h"

class CRect_Element;
class CTone_Element;
class CDrawable_Element : 
    public CGraphicsStackItem {
    private:
        CRect_Element* linkedRect = nullptr;

    public:
        CDrawable_Element() = default;
        virtual ~CDrawable_Element();

        void bindRect(CRect_Element* rect);
        CRect_Element* getRect() const { return linkedRect; };

        /* Instance variable for Ruby */
        VALUE rViewport = Qnil;
};
#endif