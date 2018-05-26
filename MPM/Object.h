#ifndef OBJECT_H
#define OBJECT_H

#include  <memory>
#include "LargrangianParticle.h"
#include "Grid.h"
#include "Util.h"

class Object {
	std::shared_ptr<std::vector<LargrangianParticle>> MaterialPoints;
public:
	Object(std::shared_ptr<std::vector<LargrangianParticle>> _MaterialPoints) : MaterialPoints(_MaterialPoints) { }

	void reset();
	void GtoP(Grid &);//Update velocity
	void updateF();
	void advection();//Update position
	void collision();

	void createVAO(std::vector<float> &v);

	int getParticleNum() { return MaterialPoints->size(); }
	std::shared_ptr<std::vector<LargrangianParticle>> getMP() { return MaterialPoints; }
};

#endif // !OBJECT_H