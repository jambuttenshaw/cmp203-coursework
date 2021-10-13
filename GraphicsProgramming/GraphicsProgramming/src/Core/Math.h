#pragma once


class Math
{
public:
	static float radians(float deg)
	{
		static constexpr float degToRad = 0.01745329252f;
		return deg * degToRad;
	}
	static float degrees(float rad)
	{
		static constexpr float radToDeg = 1 / 0.01745329252f;
		return rad * radToDeg;
	}

};