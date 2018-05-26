#include "Matrix2f.h"

Matrix2f::Matrix2f() {
	setData(0.0);
}

Matrix2f::Matrix2f(const float &i00, const float &i01, const float &i10, const float &i11) {
	setData(i00, i01, i10, i11);
}

Matrix2f::Matrix2f(const Matrix2f &m) {
	setData(m);
}

Matrix2f::Matrix2f(float _data[2][2]) {
	setData(_data);
}

void Matrix2f::loadIdentity() {
	setData(1, 0, 0, 1);
}

void Matrix2f::setData(const Matrix2f &m) {
	setData(m.data);
}

void Matrix2f::setData(float _data[2][2]){
	memcpy(data, _data, sizeof(float) * 4);
}

void Matrix2f::setData(const float _data[2][2]) {
	memcpy(data, _data, sizeof(float) * 4);
}

void Matrix2f::setData(const float &val){
	setData(val, val, val, val);
}

void Matrix2f::setData(const float &i00, const float &i01, const float &i10, const float &i11) {
	data[0][0] = i00;
	data[0][1] = i10;
	data[1][0] = i01;
	data[1][1] = i11;
}

void Matrix2f::normalize() {
	for (int i = 0; i < 2; ++i) {
		float l = 0;
		for (int j = 0; j < 2; ++j) {
			l += data[i][j] * data[i][j];
		}
		l = sqrt(l);
		for (int j = 0; j < 2; ++j) {
			data[i][j] /= l;
		}
	}
}

const float Matrix2f::determinant() const {
	return data[0][0] * data[1][1] - data[0][1] * data[1][0];
}

const Matrix2f Matrix2f::transpose() const {
	return Matrix2f(data[0][0], data[0][1], data[1][0], data[1][1]);
}

const Matrix2f Matrix2f::inverse() const {
	float det = determinant();
	return Matrix2f(
		data[1][1] / det,
		-data[1][0] / det,
		-data[0][1] / det,
		data[0][0] / det
	);
}

const Matrix2f Matrix2f::cofactor() const {
	return Matrix2f(
		data[1][1], -data[0][1],
		-data[1][0], data[0][0]
	);
}

float Matrix2f::frobeniusInnerProduct(const Matrix2f& c) const {
	float prod = 0;
	for (int i = 0; i<2; i++) {
		for (int j = 0; j<2; j++) {
			prod += data[i][j] * c.data[i][j];
		}
	}
	return prod;
}

void Matrix2f::svd_decomposition(Matrix2f &U, Matrix2f &sigma, Matrix2f &V) const {
	if (fabs(data[0][1] - data[1][0]) < MATRIX_EPSILON && fabs(data[0][1]) < MATRIX_EPSILON) {
		U.setData(data[0][0] < 0 ? -1 : 1, 0, 0, data[1][1] < 0 ? -1 : 1);
		sigma.setData(fabs(data[0][0]), 0, 0, fabs(data[1][1]));
		V.loadIdentity();
	}
	else {
		float j = data[0][0] * data[0][0] + data[0][1] * data[0][1],
			k = data[1][0] * data[1][0] + data[1][1] * data[1][1],
			v_c = data[0][0] * data[1][0] + data[0][1] * data[1][1];
		//Check to see if A^T*A is diagonal
		if (fabs(v_c) < MATRIX_EPSILON) {
			float s1 = sqrt(j),
				s2 = fabs(j - k) < MATRIX_EPSILON ? s1 : sqrt(k);
			sigma.setData(s1, 0, 0, s2);
			V.loadIdentity();
			U.setData(
				data[0][0] / s1, data[1][0] / s2,
				data[0][1] / s1, data[1][1] / s2
			);
		}
		//Otherwise, solve quadratic for eigenvalues
		else {
			float jmk = j - k,
				jpk = j + k,
				root = sqrt(jmk * jmk + 4 * v_c * v_c),
				eig = (jpk + root) / 2,
				s1 = sqrt(eig),
				s2 = fabs(root) < MATRIX_EPSILON ? s1 : sqrt((jpk - root) / 2);
			sigma.setData(s1, 0, 0, s2);
			//Use eigenvectors of A^T*A as V
			float v_s = eig - j,
				len = sqrt(v_s * v_s + v_c * v_c);
			v_c /= len;
			v_s /= len;
			V.setData(v_c, -v_s, v_s, v_c);
			//Compute w matrix as Av/s
			U.setData(
				(data[0][0] * v_c + data[1][0] * v_s) / s1,
				(data[1][0] * v_c - data[0][0] * v_s) / s2,
				(data[0][1] * v_c + data[1][1] * v_s) / s1,
				(data[1][1] * v_c - data[0][1] * v_s) / s2
			);
		}
	}
}
//DIAGONAL MATRIX OPERATIONS
//Matrix * Matrix
void Matrix2f::diag_product(const Vector2f &v) {
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			data[i][j] *= v[i];
		}
	}
}
//Matrix * Matrix^-1
void Matrix2f::diag_product_inv(const Vector2f& v) {
	for (int i = 0; i<2; i++) {
		for (int j = 0; j<2; j++)
			data[i][j] /= v[i];
	}
}
//Matrix - Matrix
void Matrix2f::diag_difference(const float& c) {
	for (int i = 0; i<2; i++)
		data[i][i] -= c;
}

void Matrix2f::diag_difference(const Vector2f& v) {
	for (int i = 0; i<2; i++)
		data[i][i] -= v[i];
}

//Matrix + Matrix
void Matrix2f::diag_sum(const float& c) {
	for (int i = 0; i<2; i++)
		data[i][i] += c;
}

void Matrix2f::diag_sum(const Vector2f& v) {
	for (int i = 0; i<2; i++)
		data[i][i] += v[i];
}
//OVERLOADS
//Array subscripts (Warning! these use [column][row])
float* Matrix2f::operator[](const int &index) {
	return data[index];
}

const float* Matrix2f::operator[](const int &index) const {
	return data[index];
}

//SCALAR OVERLOADS
//Matrix / Scalar
const Matrix2f operator/(const float& c, const Matrix2f& m) {
	return Matrix2f(m) / c;
}

const Matrix2f Matrix2f::operator/(const float &c) const {
	return Matrix2f(*this) /= c;
}

Matrix2f& Matrix2f::operator/=(const float &c) {
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			data[i][j] /= c;
		}
	}
	return *this;
}
//Matrix * Scalar
const Matrix2f operator*(const float &c, const Matrix2f &m) {
	return Matrix2f(m) * c;
}

const Matrix2f Matrix2f::operator*(const float &c) const {
	return Matrix2f(*this) *= c;
}

Matrix2f& Matrix2f::operator*=(const float &c) {
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			data[i][j] *= c;
		}
	}
	return *this;
}
//Matrix - Scalar
const Matrix2f operator-(const float& c, const Matrix2f& m) {
	return Matrix2f(m) - c;
}
const Matrix2f Matrix2f::operator-(const float& c) const {
	return Matrix2f(*this) -= c;
}
Matrix2f& Matrix2f::operator-=(const float& c) {
	for (int i = 0; i<2; i++) {
		for (int j = 0; j<2; j++)
			data[i][j] -= c;
	}
	return *this;
}
//Matrix + Scalar
const Matrix2f operator+(const float& c, const Matrix2f& m) {
	return Matrix2f(m) + c;
}
const Matrix2f Matrix2f::operator+(const float& c) const {
	return Matrix2f(*this) += c;
}
Matrix2f& Matrix2f::operator+=(const float& c) {
	for (int i = 0; i<2; i++) {
		for (int j = 0; j<2; j++)
			data[i][j] += c;
	}
	return *this;
}

//VECTOR OVERLOAD
//Matrix + Matrix
const Matrix2f Matrix2f::operator+(const Matrix2f &m) const {
	return Matrix2f(*this) += m;
}
Matrix2f& Matrix2f::operator+=(const Matrix2f &m) {
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			data[i][j] += m.data[i][j];
		}
	}
	return *this;
}
//Matrix - Matrix
const Matrix2f Matrix2f::operator-(const Matrix2f &m) const {
	return Matrix2f(*this) -= m;
}
Matrix2f& Matrix2f::operator-=(const Matrix2f &m) {
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			data[i][j] -= m.data[i][j];
		}
	}
	return *this;
}
//Matrix * Matrix
const Matrix2f Matrix2f::operator*(const Matrix2f &rhs) const {
	Matrix2f result = Matrix2f(*this);
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			result.data[i][j] = data[i][0] * rhs[0][j] + data[i][1] * rhs[1][j];
		}
	}
	return result;
}
//Matrix * Vector
const Vector2f Matrix2f::operator*(const Vector2f &v) const {
	return Vector2f(
		data[0][0] * v[0] + data[1][0] * v[1],
		data[0][1] * v[0] + data[1][1] * v[1]
	);
}

void Matrix2f::print() const {
	std::cout << data[0][0] << ",\t" << data[1][0] << std::endl <<
		data[0][1] << ",\t" << data[1][1] << std::endl;
}