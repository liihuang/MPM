#ifndef SIMCONSTANT_H
#define SIMCONSTANT_H

#include "Matrix2f.h"
#include "Vector2f.h"

#define MATRIX_EPSILON 1e-6
#define ENABLE_IMPLICIT false
#define IMPLICIT_RATIO 0.5
#define MAX_IMPLICT_ITER 30
#define MAX_RESIDUAL 1e5
#define MIN_RESIDUAL 1e-5
#define VELOCITY_EPSILON = 1e-6;

#define PI 3.1415926
#define DENSITY 100
#define TIMESTEP 1e-5
#define PARTICLE_DIAMETER 0.0025
#define WIN_WIDTH 600
#define WIN_HEIGHT 600
#define STICKY 0.9
#define WIN_METER_X 1
#define WIN_METER_Y 1
#define YOUNGS_MODULUS 1.5e6
#define POISSONS_RATIO 0.2
#define HARDENING 5
#define FLIP_PERCENT 0.95
#define BSPLINE_RADIUS 2
#define BSPLINE_EPSILON 1e-4
#define THETA_C 2.5e-2
#define THETA_S 5.0e-3


const Vector2f GRAVITY = Vector2f(0, -9.8);
const float LAMBDA = YOUNGS_MODULUS * POISSONS_RATIO / ((1 + POISSONS_RATIO)*(1 - 2 * POISSONS_RATIO)),
			MU = YOUNGS_MODULUS / (2 + 2 * POISSONS_RATIO);

#endif // !SIMCONSTANT_H
