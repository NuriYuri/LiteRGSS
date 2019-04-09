#ifndef CDrawable_Element_H
#define CDrawable_Element_H
#include "ruby.h" // Windows Build Patch
#include <SFML/Graphics.hpp>
#include "sf_Text2.hpp"
#include "utils/container/vector_tracker.h"
#include <algorithm>

class CRect_Element;
class CTone_Element;
class CRubyGraphicsStack;
class CGraphicsStack_Element;

class CDrawable_Element {
    private:
        void resetOriginStack();
        void setOriginStack(CRubyGraphicsStack* oRuby, vector_tracker<CDrawable_Element*> *o);
        
        vector_tracker<CDrawable_Element*> *origin_stack = nullptr;
        CRubyGraphicsStack* origin_ruby_stack = nullptr;
        CRect_Element* linkedRect = nullptr;
        unsigned long drawPriority = 0;
        bool disposeFromViewport_ = false;
    protected:
        VALUE self = Qnil;

    public:
        CDrawable_Element() = default;
        virtual ~CDrawable_Element();
        void setOriginStack(CRubyGraphicsStack& oRuby, vector_tracker<CDrawable_Element*>& o);
        void overrideOriginCppStack() {origin_stack = nullptr; };
        unsigned long getIndex();
        virtual void draw(sf::RenderTarget& target) const = 0;
        virtual void drawFast(sf::RenderTarget& target) const = 0;
        virtual bool isViewport() const = 0;
        virtual bool isPureSprite() const = 0;
		virtual bool isShape() const = 0;
        void setSelf(VALUE self) {
            this->self = self;
            onSelfSetted();
        }
        virtual void onSelfSetted() {}
        void disposeFromViewport() { RDATA(self)->data = nullptr; disposeFromViewport_ = true;}
        void bindRect(CRect_Element* rect);
        CRect_Element* getRect() const { return linkedRect; };

        /* Instance variable for Ruby */
        VALUE rViewport = Qnil;
        
};
#endif