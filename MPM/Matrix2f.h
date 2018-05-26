#ifndef MATRIX2F_H
#define MATRIX2F_H

#include "Vector2f.h"
#include <cstring>
#include <cmath>
#include <iostream>

#define MATRIX_EPSILON 1e-6

class Vector2f;

class Matrix2f {
public:
	float data[2][2];//[cloumn][row]

	Matrix2f();
	Matrix2f(const float &i00, const float &i01, const float &i10, const float &i11);
	Matrix2f(const Matrix2f &m);
	Matrix2f(float _data[2][2]);
	virtual ~Matrix2f() = default;

	static Matrix2f Identity(){//inline static
		return Matrix2f(1, 0, 0, 1);
	}

	static Matrix2f Zero() {//inline static
		return Matrix2f(0, 0, 0, 0);
	}

	void loadIdentity();
	void setData(const Matrix2f &m);
	void setData(float data[2][2]);
	void setData(const float data[2][2]);
	void setData(const float &val);
	void setData(const float &i00, const float &i01, const float &i10, const float &i11);

	void normalize();
	const float determinant() const;
	const Matrix2f transpose() const;
	const Matrix2f inverse() const;
	const Matrix2f cofactor() const;

	float frobeniusInnerProduct(const Matrix2f &c) const;

	void svd_decomposition(Matrix2f &U, Matrix2f &sigma, Matrix2f &V) const;

	void diag_product(const Vector2f &v);

	void diag_product_inv(const Vector2f &v);

	void diag_difference(const float &c);
	void diag_difference(const Vector2f &v);
	
	void diag_sum(const float &c);
	void diag_sum(const Vector2f &v);

	float* operator[](const int &index);
	const float* operator[](const int &index) const;

	const Matrix2f operator/(const float &c) const;
	Matrix2f& operator/=(const float &c);

	const Matrix2f operator*(const float &c) const;
	Matrix2f& operator*=(const float &c);

	const Matrix2f operator-(const float &c) const;
	Matrix2f& operator-=(const float &c);
	
	const Matrix2f operator+(const float &c) const;
	Matrix2f& operator+=(const float &c);

	const Matrix2f operator+(const Matrix2f &m) const;
	Matrix2f& operator+=(const Matrix2f &m);

	const Matrix2f operator-(const Matrix2f &m) const;
	Matrix2f& operator-=(const Matrix2f &m);
	//Matrix * Matrix
	const Matrix2f operator*(const Matrix2f &m) const;
	//Matrix * Vector
	const Vector2f operator*(const Vector2f &v) const;

	void print() const;
};

const Matrix2f operator+(const float &c, const Matrix2f &m);
const Matrix2f operator-(const float &c, const Matrix2f &m);
const Matrix2f operator*(const float &c, const Matrix2f &m);
const Matrix2f operator/(const float &c, const Matrix2f &m);

#endif
