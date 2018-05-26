#ifndef VECTOR2F_H
#define VECTOR2F_H

#include "Matrix2f.h"

class Matrix2f;

class Vector2f {
public:
	float data[2];

	Vector2f();
	Vector2f(const float &val);
	Vector2f(const float &x, const float &y);
	Vector2f(const Vector2f &orig);
	virtual ~Vector2f() = default;

	void setData(const float &val);
	void setData(const float _data[2]);
	void setData(const float &x, const float &y);
	void setData(const Vector2f &v);

	static Vector2f Zero() {
		return Vector2f(0.0, 0.0);
	}

	void normalize();
	const float dot(const Vector2f &v) const;
	const float sum() const;
	const float product() const;
	const float length() const;
	const float length_squared() const;
	
	const Matrix2f outer_product(const Vector2f &v) const;

	const Vector2f operator-() const;

	float& operator[](int index);
	const float& operator[](int index) const;

	//SCALAR OVERLOADS
	//Vector * Scalar
	const Vector2f operator*(const float &c) const;
	Vector2f& operator*=(const float &c);
	//Vector / Scalar
	const Vector2f operator/(const float &c) const;
	Vector2f& operator/=(const float &c);
	//Vector + Scalar
	const Vector2f operator+(const float &c) const;
	Vector2f& operator+=(const float &c);
	//Vector - Scalar
	const Vector2f operator-(const float &c) const;
	Vector2f& operator-=(const float &c);

	//VECTOR OVERLOADS
	//Vector / Vector (piecewise division)
	const Vector2f operator/(const Vector2f &v) const;
	Vector2f& operator/=(const Vector2f &v);
	//Vector * Vector (dot product)
	const Vector2f operator*(const Vector2f& v) const;
	Vector2f& operator*=(const Vector2f& v);
	//Vector ^ Vector (cross product)
	const Vector2f operator^(const Vector2f& v) const;
	Vector2f& operator^=(const Vector2f& v);
	//Vector + Vector
	const Vector2f operator+(const Vector2f& v) const;
	Vector2f& operator+=(const Vector2f& v);
	//Vector - Vector
	const Vector2f operator-(const Vector2f& v) const;
	Vector2f& operator-=(const Vector2f& v);
};

//Scalar operations
const Vector2f operator*(const float& c, const Vector2f& v);
const Vector2f operator/(const float& c, const Vector2f& v);
const Vector2f operator-(const float& c, const Vector2f& v);
const Vector2f operator+(const float& c, const Vector2f& v);

#endif // !VECTOR2F_H

