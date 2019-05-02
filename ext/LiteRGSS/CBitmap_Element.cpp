#include <iostream>
#include "CBitmap_Element.h"

sf::Texture& CBitmap_Element::getTexture() {
	return text;
}

sf::Image& CBitmap_Element::getImage() {
	if(!has_image()) {
		img = text.copyToImage();
	}
	return img;
}
