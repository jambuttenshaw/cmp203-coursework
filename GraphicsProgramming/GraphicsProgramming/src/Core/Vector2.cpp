#include "Vector2.h"


const Vector2 Vector2::zero = { 0,  0 };
const Vector2 Vector2::one = { 1,  1 };
const Vector2 Vector2::up = { 0,  1 };
const Vector2 Vector2::down = { 0, -1 };
const Vector2 Vector2::left = { -1,  0 };
const Vector2 Vector2::right = { 1,  0 };


Vector2::Vector2(float x, float y) {
	this->x = x;
	this->y = y;
}

Vector2 Vector2::copy() {
	Vector2 copy(
		this->x,
		this->y);
	return copy;
}

bool Vector2::equals(const Vector2& v2, float epsilon) {
	return ((fabsf(this->x - v2.x) < epsilon) &&
		(fabsf(this->y - v2.y) < epsilon));
}

bool Vector2::equals(const Vector2& v2)
{
	return equals(v2, 0.00001f);
}


float Vector2::length() {
	return (float)sqrt(this->lengthSquared());
}

float Vector2::lengthSquared() {
	return (
		this->x * this->x +
		this->y * this->y
		);
}

void Vector2::normalise() {
	float mag = this->length();
	if (mag) {
		float multiplier = 1.0f / mag;
		this->x *= multiplier;
		this->y *= multiplier;
	}
}

Vector2 Vector2::normalised()
{
	Vector2 norm(x, y);
	norm.normalise();
	return norm;
}

void Vector2::subtract(const Vector2& v1, float scale) {
	this->x -= (v1.x * scale);
	this->y -= (v1.y * scale);
}

void Vector2::set(float x, float y) {
	this->x = x;
	this->y = y;
}

void Vector2::setX(float x) {
	this->x = x;
}

void Vector2::setY(float y) {
	this->y = y;
}

float Vector2::getX() {
	return this->x;
}

float Vector2::getY() {
	return this->y;
}

float Vector2::dot(const Vector2& v2) {
	return (this->x * v2.x +
		this->y * v2.y
		);
}

void Vector2::scale(float scale) {
	this->x *= scale;
	this->y *= scale;
}

void Vector2::add(const Vector2& v1, float scale) {
	this->x += (v1.x * scale);
	this->y += (v1.y * scale);
}

Vector2 Vector2::operator+(const Vector2& v2) {
	return Vector2(this->x + v2.x, this->y + v2.y);
}

Vector2 Vector2::operator-(const Vector2& v2) {
	return Vector2(this->x - v2.x, this->y - v2.y);
}

Vector2 Vector2::operator*(float s) {
	return Vector2(this->x * s, this->y * s);
}
Vector2 Vector2::operator/(float s) {
	return Vector2(this->x / s, this->y / s);
}

Vector2& Vector2::operator+=(const Vector2& v2) {
	this->x += v2.x;
	this->y += v2.y;
	return *this;
}

Vector2& Vector2::operator-=(const Vector2& v2) {
	this->x -= v2.x;
	this->y -= v2.y;
	return *this;
}

Vector2& Vector2::operator*=(float s) {
	this->x *= s;
	this->y *= s;
	return *this;
}

Vector2& Vector2::operator/=(float s) {
	this->x *= s;
	this->y *= s;
	return *this;
}
