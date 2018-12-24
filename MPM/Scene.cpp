#include "Scene.h"
#include <limits.h>
#include <time.h>
#include <algorithm>

Scene::Scene() {
	srand((unsigned)time(NULL));
	minTemperature = INT_MAX;
	maxTemperature = INT_MIN;
}
Scene::Scene(std::vector<LargrangianParticle> _MaterialPoints) : materialPoints(_MaterialPoints) {
	Scene();
	for (auto mpoint : materialPoints) {
		minTemperature = std::min(minTemperature, (int)mpoint.temperature);
		maxTemperature = std::max(maxTemperature, (int)mpoint.temperature);
	}
}

void Scene::reset() {
	for (int i = 0, length = materialPoints.size(); i < length; ++i) {
		LargrangianParticle &currentParticle = materialPoints[i];
		//currentParticle.setVelocity(Vector2f(0.0, 0.0));
		currentParticle.velocity_gradient = Matrix2f::Zero();
		currentParticle.temperature = 0;
	}
	//if(!materialPoints.empty()) memset(&materialPoints[0], 0, sizeof(LargrangianParticle) * materialPoints.size());
}

void Scene::GtoP(Grid &grid) {
	reset();
	for (int i = 0, length = materialPoints.size(); i < length; ++i) {
		LargrangianParticle &currentParticle = materialPoints[i];


		const Vector2f &x_p = currentParticle.position;
		Vector2f pic, flip = currentParticle.velocity;
		
		float interval_x = grid.getIntervalX(), interval_y =grid.getIntervalY();
		int node_x = x_p[0] / interval_x, node_y = x_p[1] / interval_y;


		for (int temp_node_y = node_y - 1, index = 0; temp_node_y <= node_y + 2; ++temp_node_y) {
			for (int temp_node_x = node_x - 1; temp_node_x <= node_x + 2; ++temp_node_x, ++index) {
				EulerianNode &currentNode = grid.getNode(temp_node_y * grid.getX_num() + temp_node_x);
				/*-----transfer velocity from grid to particles-----*/
				//currentParticle.velocity += currentNode.velocity * currentParticle.weight[index];
				/*-----compute velocity gradient(delta_v_p)-----*/
				currentParticle.velocity_gradient += currentNode.velocity_new.outer_product(currentParticle.weight_gradient[index]);

				float w = currentParticle.weight[index];
				currentParticle.temperature += currentNode.temperature * w;
				pic += currentNode.velocity_new * w;
				flip += (currentNode.velocity_new - currentNode.velocity) * w;
			}
		}
		currentParticle.velocity = flip * FLIP_PERCENT + pic * (1 - FLIP_PERCENT);

		minTemperature = std::min(minTemperature, (int)materialPoints[i].temperature);
		maxTemperature = std::max(maxTemperature, (int)materialPoints[i].temperature);
	}

	/*for (int i = 0, length = materialPoints.size(); i < length; i++) {
		materialPoints[i].temperature /= materialPoints[i].mass;
		minTemperature = std::min(minTemperature, (int)materialPoints[i].temperature);
		maxTemperature = std::max(maxTemperature, (int)materialPoints[i].temperature);
	}*/

}

void Scene::updateF() {
	for (int i = 0, length = materialPoints.size(); i < length; ++i) {
		materialPoints[i].updateF();
	}
}

void Scene::collision() {
	for (int i = 0, length = materialPoints.size(); i < length; ++i) {
		Vector2f newPos = materialPoints[i].position + materialPoints[i].velocity * TIMESTEP;
		
		if (newPos[0] <= 0.0 || newPos[0] >= 1.0) {
			materialPoints[i].velocity[0] = -STICKY * materialPoints[i].velocity[0];
		}
		if (newPos[1] <= 0.0 || newPos[1] >= 1.0) {
			materialPoints[i].velocity[1] = -STICKY * materialPoints[i].velocity[1];
		}
	}
}

void Scene::advection() {
	for (int i = 0, length = materialPoints.size(); i < length; ++i) {
		LargrangianParticle &currentParticle = materialPoints[i];
		currentParticle.position += TIMESTEP * currentParticle.velocity;//advection

	}
}

void Scene::createVAO(std::vector<float> &v) {
	int temperatureRange = maxTemperature - minTemperature;
	//int temperatureRange = 100;

	for (int i = 0, particleNum = materialPoints.size(); i < particleNum; ++i) {
		v[4 * i] = materialPoints[i].position[0];
		v[4 * i + 1] = materialPoints[i].position[1];
		v[4 * i + 2] = (materialPoints[i].temperature - minTemperature) * 1.0 / temperatureRange;
		v[4 * i + 3] = (maxTemperature - materialPoints[i].temperature) * 1.0 / temperatureRange;
	}
}

LargrangianParticle& Scene::getMaterialPointAt(int index) {
	return materialPoints[index];
}

const LargrangianParticle& Scene::getMaterialPointAt(int index) const {
	return materialPoints[index];
}

void Scene::addSphere(Vector2f origin, float radius, Vector2f velocity, float temperature) {
	float particle_area = PARTICLE_DIAMETER * PARTICLE_DIAMETER;
	float particle_mass = particle_area * DENSITY;
	int total_particle_number = PI * radius * radius / particle_area;

	int current_particle_number = 0;
	while (current_particle_number < total_particle_number) {
		float x = UniformRandom(-radius, radius);
		float y = UniformRandom(-radius, radius);
		temperature = UniformRandom(0, 100);
		minTemperature = std::min(minTemperature, (int)temperature);
		maxTemperature = std::max(maxTemperature, (int)temperature);
		if (sqrt(x * x + y * y) >= radius) continue;
		materialPoints.push_back(LargrangianParticle(particle_mass, origin + Vector2f(x, y), LAMBDA, MU, velocity, temperature));
		current_particle_number++;
	}
}

void Scene::addRectangle(Vector2f bottom_left, Vector2f top_right, Vector2f velocity, float temperature) {
	float particle_area = PARTICLE_DIAMETER * PARTICLE_DIAMETER;
	float particle_mass = particle_area * DENSITY;
	int total_particle_number = (top_right[0] - bottom_left[0]) * (top_right[1] - bottom_left[1]) / particle_area;

	int current_particle_number = 0;
	while (current_particle_number < total_particle_number) {
		float x = UniformRandom(bottom_left[0], top_right[0]);
		float y = UniformRandom(bottom_left[1], top_right[1]);

		materialPoints.push_back(LargrangianParticle(particle_mass, Vector2f(x, y), LAMBDA, MU, velocity, temperature));
		current_particle_number++;
	}
}