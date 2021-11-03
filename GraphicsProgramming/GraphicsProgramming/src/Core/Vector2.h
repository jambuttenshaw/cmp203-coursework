#include <math.h>

#ifndef _VECTOR2_H_
#define _VECTOR2_H_

class Vector2 {

public:
	Vector2(float x = 0, float y = 0);
	Vector2 copy();


	void set(float x, float y);
	void setX(float x);
	void setY(float y);

	float getX();
	float getY();

	void add(const Vector2& v1, float scale = 1.0);
	void subtract(const Vector2& v1, float scale = 1.0);
	void scale(float scale);

	float dot(const Vector2& v2);

	void normalise();
	Vector2 normalised();
	float length();
	float lengthSquared();

	bool equals(const Vector2& v2, float epsilon);
	bool equals(const Vector2& v2);

	Vector2 operator+(const Vector2& v2);
	Vector2 operator-(const Vector2& v2);
	Vector2 operator*(float s);
	Vector2 operator/(float s);

	Vector2& operator+=(const Vector2& v2);
	Vector2& operator-=(const Vector2& v2);
	Vector2& operator*=(float s);
	Vector2& operator/=(float s);

	inline float const* ptr() const { return reinterpret_cast<float const*>(this); }

	float x;
	float y;

public:
	static const Vector2 zero;
	static const Vector2 one;
	static const Vector2 forward;
	static const Vector2 back;
	static const Vector2 up;
	static const Vector2 down;
	static const Vector2 left;
	static const Vector2 right;
};

#endif