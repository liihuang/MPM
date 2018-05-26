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

#endif // !PARTICLE_H

