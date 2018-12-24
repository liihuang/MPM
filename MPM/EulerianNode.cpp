#include "Particle.h"

Vector2f EulerianNode::getExplicitForce() { return explicitForce; }

void EulerianNode::setExplicitForce(const Vector2f &_explicitForce) { explicitForce = _explicitForce; }