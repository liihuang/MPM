#ifndef UTIL_H
#define UTIL_H

#include <math.h>
#include "Matrix2f.h"
#include "Vector2f.h"
#include "SimConstant.h"

const float BSPLINE_EPSILON = 1e-4;

inline float cubic_B_Spline(float x) {
	float abs_x = fabs(x), result;
	if (abs_x < 1) {
		result = abs_x * abs_x * abs_x / 2 - abs_x * abs_x + 2.0 / 3;
	}
	else if (abs_x < 2) {
		result = pow(2 - abs_x, 3) / 6.0;
	}
	else return 0;
	if (result < BSPLINE_EPSILON) return 0;
	else return result;
}

inline float cubic_B_Spline_grad(float x) {
	float abs_x = fabs(x), result;
	if (abs_x < 1) {
		result = 1.5 * x * abs_x - 2 * x;
	}
	else if (x < 2) {
		result = -x * abs_x / 2 + 2 * x - 2 * x / abs_x;
	}
	else return 0;
	if (fabs(result) < BSPLINE_EPSILON) return 0;
	else return result;
}

inline void clamp(float &number, const float &lower_bound, const float &upper_bound) {
	if (number < lower_bound) {
		number = lower_bound;
	}
	if (number > upper_bound) {
		number = upper_bound;
	}
}

float UniformRandom(const float &min, const float &max);
#endif