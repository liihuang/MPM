#ifndef SIMCONSTANT_H
#define SIMCONSTANT_H

#include "Matrix2f.h"
#include "Vector2f.h"

#define MATRIX_EPSILON 1e-6
#define ENABLE_IMPLICIT true
#define IMPLICIT_RATIO 0.5
#define MAX_IMPLICT_ITER 30
#define MAX_RESIDUAL 1e5
#define MIN_RESIDUAL 1e-5


const float TimeStep = 0.00001;
const int WinWidth = 1000, WinHeight = 1000;
const int Sticky = 0.9;
const float Particle_Diameter = 0.0025;
const float Density = 100;
const float PI = 3.1415926;
const float WIN_METER_X = 1, WIN_METER_Y = 1;
const float YOUNGS_MODULUS = 4.8e4,		//Young's modulus (springiness) (1.4e5)
			POISSONS_RATIO = 0.2;
const float Hardening = 10;
const float FLIP_PERCENT = 0.95;
const float BSPLINE_RADIUS = 2;


const Vector2f Gravity = Vector2f(0, -9.8);
const float LAMBDA = YOUNGS_MODULUS * POISSONS_RATIO / ((1 + POISSONS_RATIO)*(1 - 2 * POISSONS_RATIO)),
			MU = YOUNGS_MODULUS / (2 + 2 * POISSONS_RATIO);
const float theta_c = 2.5e-2;//critical compression
const float theta_s = 5.0e-3;//critical stretch

#endif // !SIMCONSTANT_H
