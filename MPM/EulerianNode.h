#ifndef ENODE_H
#define ENODE_H

#include "Particle.h"
#include "Matrix2f.h"
#include "Vector2f.h"
#include "SimConstant.h"

#define VELOCITY_EPSILON = 1e-6;

class Grid;
class Object;

class EulerianNode : public Particle {
	friend class Grid;
	friend class Object;
	Vector2f explicitForce;
	bool active;
	Vector2f velocity_new;
#if ENABLE_IMPLICIT
	Vector2f r, Ar, p, Ap;
	Vector2f r_next, Ar_next;
	float alpha, beta;
	bool imp_active;
#endif // ENABLE_IMPLICIT


public:
	EulerianNode(Vector2f _position, float _volume) : Particle(0, _volume, 0, _position, Vector2f(0, 0)), explicitForce(Vector2f(0, 0)), active(false) { }

	Vector2f getExplicitForce() { return explicitForce; }

	void setExplicitForce(const Vector2f &_explicitForce) { explicitForce = _explicitForce; }
};

#endif // !ENODE_H
