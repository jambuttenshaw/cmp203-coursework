// Vector3 class
// Repesents a vector 3 object, storing x, y and z. Provided functions for vector maths and manipulation
// @author Paul Robertson

#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include <math.h>

class Vector3 {

public:
	Vector3(float x = 0, float y = 0, float z = 0);
	Vector3 copy();


	void set(float x, float y, float z);
	void setX(float x);
	void setY(float y);
	void setZ(float z);

	float getX();
	float getY();
	float getZ();

	void add(const Vector3& v1, float scale = 1.0);
	void subtract(const Vector3& v1, float scale = 1.0);
	void scale(float scale);

	float dot(const Vector3& v2);
	Vector3 cross(const Vector3& v2);

	void normalise();
	Vector3 normalised();
	float length();
	float lengthSquared();

	bool equals(const Vector3& v2, float epsilon);
	bool equals(const Vector3& v2);

	Vector3 operator+(const Vector3& v2);
	Vector3 operator-(const Vector3& v2);
	Vector3 operator*(float s);
	Vector3 operator/(float s);

	Vector3& operator+=(const Vector3& v2);
	Vector3& operator-=(const Vector3& v2);
	Vector3& operator*=(float s);
	Vector3& operator/=(float s);

	inline float const* ptr() const { return reinterpret_cast<float const*>(this); }

	float x;
	float y;
	float z;


public:
	static const Vector3 zero;
	static const Vector3 one;
	static const Vector3 forward;
	static const Vector3 back;
	static const Vector3 up;
	static const Vector3 down;
	static const Vector3 left;
	static const Vector3 right;
};

#endif