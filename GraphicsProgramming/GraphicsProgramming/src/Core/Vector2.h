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

	float getX() const;
	float getY() const;

	void add(const Vector2& v1, float scale = 1.0);
	void subtract(const Vector2& v1, float scale = 1.0);
	void scale(float scale);

	float dot(const Vector2& v2) const;

	void normalise();
	Vector2 normalised() const;
	float length() const;
	float lengthSquared() const;

	bool equals(const Vector2& v2, float epsilon) const;
	bool equals(const Vector2& v2) const;

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