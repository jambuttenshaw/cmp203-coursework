#pragma once

#include <glm/glm.hpp>


struct Color
{
	Color() = default;
	Color(float red, float green, float blue)
		: r(red), g(green), b(blue) {}
	Color(float red, float green, float blue, float alpha)
		: r(red), g(green), b(blue), a(alpha) {}
	Color(float v)
		: r(v), g(v), b(v) {}
	Color(const glm::vec3& c)
		: r(c.r), g(c.g), b(c.b) {}
	Color(const glm::vec4& c)
		: r(c.r), g(c.g), b(c.b), a(c.a) {}

	float const* ptr() const { return reinterpret_cast<float const*>(this); }

	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	float a = 1.0f;


	// color helper functions
	static Color Lerp(const Color& color1, const Color& color2, float t);

	// default colors
	static const Color White;
	static const Color Black;
	static const Color Red;
	static const Color Green;
	static const Color Blue;
	static const Color Yellow;
	static const Color Cyan;
	static const Color Magenta;

};

Color operator+(const Color& lhs, const Color& rhs);
Color operator*(const Color& lhs, float rhs);
