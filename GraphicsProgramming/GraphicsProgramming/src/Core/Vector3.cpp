#include "Vector3.h"


const Vector3 Vector3::zero    = {  0,  0,  0 };
const Vector3 Vector3::one     = {  1,  1,  1 };
const Vector3 Vector3::forward = {  0,  0, -1 };
const Vector3 Vector3::back    = {  0,  0,  1 };
const Vector3 Vector3::up      = {  0,  1,  0 };
const Vector3 Vector3::down    = {  0, -1,  0 };
const Vector3 Vector3::left    = { -1,  0,  0 };
const Vector3 Vector3::right   = {  1,  0,  0 };


Vector3::Vector3(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3::Vector3(float xyz)
	: x(xyz), y(xyz), z(xyz)
{
}

Vector3 Vector3::copy() {
	Vector3 copy(
		this->x,
		this->y,
		this->z);
	return copy;
}

bool Vector3::equals(const Vector3& v2, float epsilon) const {
	return ((fabsf(this->x - v2.x) < epsilon) &&
		(fabsf(this->y - v2.y) < epsilon) &&
		(fabsf(this->z - v2.z) < epsilon));
}

bool Vector3::equals(const Vector3& v2) const
{
	return equals(v2, 0.00001f);
}


float Vector3::length() const {
	return (float)sqrt(this->lengthSquared());
}

float Vector3::lengthSquared() const {
	return (
		this->x*this->x +
		this->y*this->y +
		this->z*this->z
		);
}

void Vector3::normalise() {
	float mag = this->length();
	if (mag) {
		float multiplier = 1.0f / mag;
		this->x *= multiplier;
		this->y *= multiplier;
		this->z *= multiplier;
	}
}

Vector3 Vector3::normalised() const
{
	Vector3 norm(x, y, z);
	norm.normalise();
	return norm;
}

Vector3 Vector3::cross(const Vector3& v2) const {
	Vector3 cross(
		(this->y * v2.z - this->z * v2.y),
		(this->z * v2.x - this->x * v2.z),
		(this->x * v2.y - this->y * v2.x)
		);
	return cross;
}

void Vector3::subtract(const Vector3& v1, float scale) {
	this->x -= (v1.x*scale);
	this->y -= (v1.y*scale);
	this->z -= (v1.z*scale);
}

void Vector3::set(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

void Vector3::setX(float x) {
	this->x = x;
}

void Vector3::setY(float y) {
	this->y = y;
}

void Vector3::setZ(float z) {
	this->z = z;
}

float Vector3::getX() const {
	return this->x;
}

float Vector3::getY() const {
	return this->y;
}

float Vector3::getZ() const {
	return this->z;
}

float Vector3::dot(const Vector3& v2) const {
	return (this->x*v2.x +
		this->y*v2.y +
		this->z*v2.z
		);
}

void Vector3::scale(float scale) {
	this->x *= scale;
	this->y *= scale;
	this->z *= scale;
}

void Vector3::add(const Vector3& v1, float scale) {
	this->x += (v1.x*scale);
	this->y += (v1.y*scale);
	this->z += (v1.z*scale);
}

Vector3 Vector3::operator+(const Vector3& v2) {
	return Vector3(this->x + v2.x, this->y + v2.y, this->z + v2.z);
}

Vector3 Vector3::operator-(const Vector3& v2) {
	return Vector3(this->x - v2.x, this->y - v2.y, this->z - v2.z);
}

Vector3 Vector3::operator*(float s) {
	return Vector3(this->x * s, this->y * s, this->z * s);
}
Vector3 Vector3::operator/(float s) {
	return Vector3(this->x / s, this->y / s, this->z / s);
}

Vector3& Vector3::operator+=(const Vector3& v2) {
	this->x += v2.x;
	this->y += v2.y;
	this->z += v2.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& v2) {
	this->x -= v2.x;
	this->y -= v2.y;
	this->z -= v2.z;
	return *this;
}

Vector3& Vector3::operator*=(float s) {
	this->x *= s;
	this->y *= s;
	this->z *= s;
	return *this;
}

Vector3& Vector3::operator/=(float s) {
	this->x *= s;
	this->y *= s;
	this->z *= s;
	return *this;
}
