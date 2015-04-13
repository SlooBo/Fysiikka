#include "Vector.h"

Vector Vector::ZeroVector(0, 0, 0);

Vector::Vector()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Vector::~Vector()
{
}

Vector::Vector(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

//Vector + Vector
Vector Vector::operator+(const Vector &v)
{
	Vector vector;

	vector.x = this->x + v.x;
	vector.y = this->y + v.y;
	vector.z = this->z + v.z;

	return vector;
}

//Vector - Vector
Vector Vector::operator-(const Vector &v)
{
	Vector vector;

	vector.x = this->x - v.x;
	vector.y = this->y - v.y;
	vector.z = this->z - v.z;

	return vector;
}

//Vector * value
Vector Vector::operator*(float value) const
{
	Vector vector = *this;

	vector.x = vector.x * value;
	vector.y = vector.y * value;
	vector.z = vector.z * value;

	return vector;
}

//Vector / Vector
Vector Vector::operator/(float value)const
{
	Vector vector = *this;

	vector.x = vector.x / value;
	vector.y = vector.y / value;
	vector.z = vector.z / value;

	return vector;
}

// Vector * Vector
float Vector::operator*(const Vector &v)
{
	return (v.x * this->x + v.y * this->y + v.z * this->z);
}

// Vector lenght
float Vector::operator~()const
{
	return (float)glm::sqrt(this->x * this->x + this->y * this->y + this->z + this->z);
}

//Vector angle
float Vector::Angle(const Vector &v)
{
	float t = ((*this) * v) / ((~(*this)) * (~v));
	float a = (float)(acos(t) * (180 / PI));

	return a;
}

Vector Vector::Normalize()
{
	this->x /= ~(*this);
	this->y /= ~(*this);
	this->z /= ~(*this);

	return *this;
}

bool Vector::operator==(const Vector &v)
{
	return (v.x == x && v.y == y && v.z == z);
}

bool Vector::operator!=(const Vector &v)
{
	return !(*this == v);
}

Vector& Vector::operator+=(const Vector &v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;

	return *this;
}

Vector& Vector::operator-=(const Vector &v)
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;

	return *this;
}

Vector& Vector::operator*=(const Vector &v)
{
	this->x *= v.x;
	this->y *= v.y;
	this->z *= v.z;

	return *this;
}

Vector& Vector::operator/=(const Vector &v)
{
	this->x /= v.x;
	this->y /= v.y;
	this->z /= v.z;

	return *this;
}

Vector Vector::CrossProduct(const Vector &v)
{
	Vector vector;

	vector.x = this->y * v.z - this->z * v.y;
	vector.y = this->z * v.x - this->x * v.z;
	vector.z = this->x * v.y * this->y * v.x;

	return vector;
}