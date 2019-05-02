#ifndef CTONE_ELEMENT_H
#define CTONE_ELEMENT_H
#include <SFML/Graphics.hpp>
#include "CViewport_Element.h"

class CTone_Element {
	private:
		sf::Glsl::Vec4 tonevalues;
		CViewport_Element* target_ = nullptr;
	public:
		CTone_Element() = default;
		~CTone_Element() { 
			if(target_ != nullptr) {
				target_->bindTone(nullptr);
			}
		}
		sf::Glsl::Vec4* getTone() { return &tonevalues; };
		void bindViewport(CViewport_Element* target) {
			if(target != target_) {
				auto lastLinkedElement = target_;
				target_ = nullptr;
				if(lastLinkedElement != nullptr) {
					lastLinkedElement->bindTone(nullptr);
				}
				target_ = target;
				if(target_ != nullptr) {
					target_->bindTone(this);
				}
			}
		}
		CViewport_Element* getViewport() { return target_; }
};

namespace meta {
	template<>
	struct Log<CTone_Element> {
		static constexpr auto classname = "Tone";
	};
}
#endif