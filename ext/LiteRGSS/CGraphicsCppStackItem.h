#ifndef CGraphicsCppStackItem_H
#define CGraphicsCppStackItem_H

#include <SFML/Graphics.hpp>

#include "CGraphicsCppStack.h"

template <class T>
class CGraphicsCppStackItem {
    private:
        friend class CGraphicsCppStack;

        void setOriginStack(CGraphicsCppStack *o) {
            /* if the stack is already setted, nothing to be done */
            if(origin_stack != o) {
                //Detach & Attach C++
                if(origin_stack != nullptr) {
                    origin_stack->remove(static_cast<T*>(this));
                }
                origin_stack = o;
                if(origin_stack != nullptr) {
                    origin_stack->add(static_cast<T*>(this));
                    if(drawPriority == 0) {
                        drawPriority = origin_stack->size();
                    }
                }
            }
        }

        CGraphicsCppStack *origin_stack = nullptr;
        unsigned long drawPriority = 0;
    
    protected:    
        CGraphicsCppStackItem() = default;
        void setOriginStack(CGraphicsCppStack& o) { setOriginStack(&o); }
        void resetOriginStack() { setOriginStack(nullptr); }
        void overrideOriginStack(CGraphicsCppStack* stack = nullptr) { origin_stack = stack; }
    public:
        virtual ~CGraphicsCppStackItem() { resetOriginStack(); }
        unsigned long getDrawPriority() const { return drawPriority; }

        virtual void draw(sf::RenderTarget& target) const = 0;
        virtual void drawFast(sf::RenderTarget& target) const = 0;
        virtual bool isViewport() const = 0;
        virtual bool isPureSprite() const = 0;
		virtual bool isShape() const = 0;
};

#endif