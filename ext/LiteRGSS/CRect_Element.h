#ifndef CRECT_ELEMENT_H
#define CRECT_ELEMENT_H
#include <iostream>
#include <SFML/Graphics.hpp>
#include "CDrawable_Element.h"
#include "metadata.h"

class CRect_Element {
private:
	sf::IntRect rect;
	CDrawable_Element* target_ = nullptr;
public:
	CRect_Element() = default;
	~CRect_Element() {
		if(target_ != nullptr) {
			target_->bindRect(nullptr);
		}
	}
	
	sf::IntRect& getRect() { return rect; };
	void bindElement(CDrawable_Element* target) { 
		if(target != target_) {
			auto lastLinkedElement = target_;
			target_ = nullptr;
			if(lastLinkedElement != nullptr) {
				lastLinkedElement->bindRect(nullptr);
			}
			target_ = target;
			if(target_ != nullptr) {
				target_->bindRect(this);
			}
		}
	}
	CDrawable_Element* getElement() { return target_; }
};

namespace meta {
	template<>
	struct Log<CRect_Element> {
		static constexpr auto classname = "Rect";
	};
}


#endif