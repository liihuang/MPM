#ifndef PARTICLE_H
#define PARTICLE_H

#include "Matrix2f.h"
#include "Vector2f.h"

class Particle {
protected:
	float mass, volume, density;
	Vector2f position, velocity;
public:
	Particle() = default;
	Particle(float _mass, float _volume, float _density, Vector2f _position, Vector2f _velocity) 
		: mass(_mass), volume(_volume), density(_density), position(_position), velocity(_velocity){}
	virtual ~Particle() = default;

	float getVolume() { return volume; }
	float getMass() { return mass; }
	float getDensity() { return density; }
	Vector2f getPotision() { return position; }
	Vector2f getVelocity() { return velocity; }

	void setVolume(const float &_volume) { volume = _volume; }
	void setMass(const float &_mass) { mass = _mass; }
	void setDensity(const float &_density) { density = _density; }
	void setPosition(const Vector2f &_position) { position = _position; }
	void setVelocity(const Vector2f &_velocity) { velocity = _velocity; }

	//virtual void setVolume(float _volume) { volume = _volume; }
};


class LargrangianParticle : public Particle {
	friend class Grid;
	friend class Scene;
	friend class MPM_Simulator;

	Vector2f initialPosition;//X
	Matrix2f F_Elastic, F_Plastic;//deformation gradient
	//Matrix2f Dp, Bp;
	float lambda, mu;
	float temperature, heatConductivity;
	Matrix2f Sigma;//the Cauchy stress
	Matrix2f P;//the first Piola-Kirchoff stress

	Matrix2f SVD_U, SVD_Singular, SVD_V;//SVD decomposition of F_Elastic

	float weight[16];
	Vector2f weight_gradient[16];
	Matrix2f velocity_gradient;

public:

	LargrangianParticle() = default;
	LargrangianParticle(float mass, Vector2f _initialPosition, float _lambda, float _mu, Vector2f _vel, float _temprature) :
		Particle(mass, 0, 0, _initialPosition, _vel), initialPosition(_initialPosition), lambda(_lambda), mu(_mu), temperature(_temprature) {
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
	float getTemprature() { return temperature; }
	Vector2f getWeight_grad(int index) { if (index < 16 && index >= 0) return weight_gradient[index]; }

	void setInitialPosition(Vector2f _initialPosition) { initialPosition = _initialPosition; }
	void setF_E(Matrix2f F_E) { F_Elastic = F_E; }
	void setF_P(Matrix2f F_P) { F_Plastic = F_P; }
	void setSigma(Matrix2f _Sigma) { Sigma = _Sigma; }
	void setP(Matrix2f _P) { P = _P; }
	/*void setDp(Matrix2f _Dp) { Dp = _Dp; }
	void setBp(Matrix2f _Bp) { Bp = _Bp; }*/
	void setLambda(float _lambda) { lambda = _lambda; }
	void setMu(float _mu) { mu = _mu; }
	void setTemprature(float _temprature) { temperature = _temprature; }
	void setWeight(int index, float value) { if (index < 16 && index >= 0) weight[index] = value; }
	void setWeight_grad(int index, Vector2f value) { if (index < 16 && index >= 0) weight_gradient[index] = value; }

};


class EulerianNode : public Particle {
	friend class Grid;
	friend class Scene;
	friend class MPM_Simulator;

	Vector2f explicitForce;
	bool active;
	Vector2f velocity_new;
	float temperature;

#if ENABLE_IMPLICIT
	Vector2f r, Ar, p, Ap;
	Vector2f r_next, Ar_next;
	float alpha, beta;
	bool imp_active;
#endif // ENABLE_IMPLICIT

public:
	EulerianNode() = default;
	EulerianNode(Vector2f _position, float _volume, float _temperature = 0.0) : Particle(0, _volume, 0, _position, Vector2f(0, 0)), temperature(_temperature), explicitForce(Vector2f(0, 0)), active(false) { }

	Vector2f getExplicitForce();
	float getTemprature() { return temperature; }
	void setTemprature(float _temprature) { temperature = _temprature; }
	void setExplicitForce(const Vector2f &_explicitForce);
};

#endif // !PARTICLE_H

