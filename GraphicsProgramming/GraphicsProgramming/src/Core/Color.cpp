#include "Color.h"

const Color Color::White   = { 1, 1, 1, 1 };
const Color Color::Black   = { 0, 0, 0, 1 };
const Color Color::Red     = { 1, 0, 0, 1 };
const Color Color::Green   = { 0, 1, 0, 1 };
const Color Color::Blue    = { 0, 0, 1, 1 };
const Color Color::Yellow  = { 1, 1, 0, 1 };
const Color Color::Cyan    = { 0, 1, 1, 1 };
const Color Color::Magenta = { 1, 0, 1, 1 };

Color Color::Lerp(const Color& color1, const Color& color2, float t)
{
	return color1 * (1 - t) + color2 * t;
}

Color operator+(const Color& lhs, const Color& rhs)
{
	return Color(lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b, lhs.a + rhs.a);
}

Color operator*(const Color& lhs, float rhs)
{
	return Color(lhs.r * rhs, lhs.g * rhs, lhs.b * rhs, lhs.a * rhs);
}
