#ifndef L_UTILS_COMMON_HEADER
#define L_UTILS_COMMON_HEADER

#include <SFML/Graphics.hpp>

inline long normalize_long(long value, long min, long max) {
	if(value < min)
		return min;
	if(value > max)
		return max;
	return value;
}
inline double normalize_double(double value, double min, double max) {
	if(value < min)
		return min;
	if(value > max)
		return max;
	return value;
}

inline void rect_copy(sf::IntRect* dest, const sf::IntRect* src) {
	dest->left = src->left;
	dest->top = src->top;
	dest->width = src->width;
	dest->height = src->height;
}

inline void tone_copy(sf::Glsl::Vec4* dest, const sf::Glsl::Vec4* src)
{
	dest->x = src->x;
	dest->y = src->y;
	dest->z = src->z;
	dest->w = src->w;
}
#endif