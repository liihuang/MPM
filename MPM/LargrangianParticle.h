#ifndef LPARTICLE_H
#define LPARTICLE_H

#include "Matrix2f.h"
#include "Vector2f.h"
#include "Particle.h"
#include "Util.h"
#include "SimConstant.h"

class Grid;
class Object;

class LargrangianParticle : public Particle {
	friend class Grid;
	friend class Object;
	Vector2f initialPosition;//X
	Matrix2f F_Elastic, F_Plastic;//deformation gradient
	//Matrix2f Dp, Bp;
	float lambda, mu;
	Matrix2f Sigma;//the Cauchy stress
	Matrix2f P;//the first Piola-Kirchoff stress

	Matrix2f SVD_U, SVD_Singular, SVD_V;//SVD decomposition of F_Elastic

	float weight[16];
	Vector2f weight_gradient[16];
	Matrix2f velocity_gradient;
public:

	LargrangianParticle(float mass, Vector2f _initialPosition, float _lambda, float _mu, Vector2f _vel) :
		Particle(mass, 0, 0, _initialPosition, _vel), initialPosition(_initialPosition), lambda(_lambda), mu(_mu) {
		Sigma = Matrix2f::Zero();
		P = Matrix2f::Zero();
		F_Elastic = Matrix2f::Identity();
		F_Plastic = Matrix2f::Identity();
		SVD_U = Matrix2f::Identity();
		SVD_V = Matrix2f::Identity();
		SVD_Singular = Matrix2f::Identity();
		velocity_gradient = Matrix2f::Zero();
		
	}


	//void SVD_Decomposition();
	void computeSigma();
	void updateF();

	Vector2f getInitialPosition() { return initialPosition; }
	Matrix2f getF_E() { return F_Elastic; }
	Matrix2f getF_P() { return F_Plastic; }
	Matrix2f getSigma() { return Sigma; }
	Matrix2f getP() { return P; }
	float getLambda() { return lambda; }
	float getMu() { return mu; }
	float getWeight(int index) { if (index < 16 && index >= 0) return weight[index]; }
	Vector2f getWeight_grad(int index){ if (index < 16 && index >= 0) return weight_gradient[index]; }

	void setInitialPosition(Vector2f _initialPosition){ initialPosition = _initialPosition; }
	void setF_E(Matrix2f F_E) { F_Elastic = F_E; }
	void setF_P(Matrix2f F_P) { F_Plastic = F_P; }
	void setSigma(Matrix2f _Sigma) { Sigma = _Sigma; }
	void setP(Matrix2f _P) { P = _P; }
	/*void setDp(Matrix2f _Dp) { Dp = _Dp; }
	void setBp(Matrix2f _Bp) { Bp = _Bp; }*/
	void setLambda(float _lambda) { lambda = _lambda; }
	void setMu(float _mu) { mu = _mu; }
	void setWeight(int index, float value) { if (index < 16 && index >= 0) weight[index] = value; }
	void setWeight_grad(int index, Vector2f value){ if (index < 16 && index >= 0) weight_gradient[index] = value; }

};

#endif // !LPARTICLE_H