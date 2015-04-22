#include <GLM\glm.hpp>

#define PI 3.1415926

#pragma once
class Vector
{
public:
	Vector();
	Vector(float x, float y, float z);

	static Vector ZeroVector;

	Vector operator+(const Vector &v);
	Vector operator-(const Vector &v);
	Vector operator*(float value) const;
	Vector operator/(float value) const;
	float operator*(const Vector &v);
	float operator~() const;
	float Angle(const Vector &v);
	Vector Normalize();
	bool operator==(const Vector &v);
	bool operator!=(const Vector &v);
	Vector& operator+=(const Vector &v);
	Vector& operator-=(const Vector &v);
	Vector& operator*=(const Vector &v);
	Vector& operator/=(const Vector &v);
	Vector CrossProduct(const Vector &v);

	float x, y, z;
};

