#ifndef SCENE_H
#define SCENE_H

#include <time.h>
#include "Particle.h"
#include "Grid.h"
#include "Util.h"

class Scene {
	std::vector<LargrangianParticle> materialPoints;
	int minTemperature, maxTemperature;
public:
	Scene();
	Scene(std::vector<LargrangianParticle> _MaterialPoints);

	void reset();
	void GtoP(Grid &);//Update velocity
	void updateF();
	void advection();//Update position
	void collision();

	void createVAO(std::vector<float> &v);

	int getParticleNum() { return materialPoints.size(); }
	LargrangianParticle& getMaterialPointAt(int index);
	const LargrangianParticle& getMaterialPointAt(int index) const;

	// Add basic geometries
	void addRectangle(Vector2f bottom_left, Vector2f top_right,Vector2f velocity = Vector2f::Zero(), float temperature = 0.0);
	void addSphere(Vector2f origin, float radius, Vector2f velocity = Vector2f::Zero(), float temperature = 0.0);
	void addTriangle(Vector2f);
};

#endif // !SCENE_H