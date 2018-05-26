#include "Object.h"

void Object::reset() {
	for (int i = 0, length = MaterialPoints->size(); i < length; ++i) {
		LargrangianParticle &currentParticle = (*MaterialPoints)[i];
		//currentParticle.setVelocity(Vector2f(0.0, 0.0));
		currentParticle.velocity_gradient = Matrix2f::Zero();
	}
}

void Object::GtoP(Grid &grid) {
	reset();
	for (int i = 0, length = MaterialPoints->size(); i < length; ++i) {
		LargrangianParticle &currentParticle = (*MaterialPoints)[i];


		const Vector2f &x_p = currentParticle.position;
		Vector2f pic, flip = currentParticle.velocity;
		
		float interval = grid.getInterval();
		int node_x = x_p[0] / interval, node_y = x_p[1] / interval;


		for (int temp_node_y = node_y - 1, index = 0; temp_node_y <= node_y + 2; ++temp_node_y) {
			for (int temp_node_x = node_x - 1; temp_node_x <= node_x + 2; ++temp_node_x, ++index) {
				EulerianNode &currentNode = grid.getNode(temp_node_y * grid.getX_num() + temp_node_x);
				/*-----transfer velocity from grid to particles-----*/
				//currentParticle.velocity += currentNode.velocity * currentParticle.weight[index];
				/*-----compute velocity gradient(delta_v_p)-----*/
				currentParticle.velocity_gradient += currentNode.velocity_new.outer_product(currentParticle.weight_gradient[index]);

				float w = currentParticle.weight[index];
				pic += currentNode.velocity_new * w;
				flip += (currentNode.velocity_new - currentNode.velocity)*w;
			}
		}

		currentParticle.velocity = flip * FLIP_PERCENT + pic * (1 - FLIP_PERCENT);
	}

	//apply plasticity

}

void Object::updateF() {
	for (int i = 0, length = MaterialPoints->size(); i < length; ++i) {
		(*MaterialPoints)[i].updateF();
	}
}

void Object::collision() {
	for (int i = 0, length = MaterialPoints->size(); i < length; ++i) {
		Vector2f newPos = (*MaterialPoints)[i].position + (*MaterialPoints)[i].velocity * TimeStep;
		if (newPos[0] < 0.05 || newPos[0] > 1.0 - 0.05) {
			(*MaterialPoints)[i].velocity[0] = -Sticky * (*MaterialPoints)[i].velocity[0];
		}
		if (newPos[1] < 0.05 || newPos[1] > 1.0 - 0.05) {
			(*MaterialPoints)[i].velocity[1] = -Sticky * (*MaterialPoints)[i].velocity[1];
		}
	}
}

void Object::advection() {
	for (int i = 0, length = MaterialPoints->size(); i < length; ++i) {
		LargrangianParticle &currentParticle = (*MaterialPoints)[i];
		currentParticle.position += TimeStep * currentParticle.velocity;//advection

	}
}

void Object::createVAO(std::vector<float> &v) {
	for (int i = 0, particleNum = MaterialPoints->size(); i < particleNum; ++i) {
		v[2 * i] = (*MaterialPoints)[i].position[0];
		v[2 * i + 1] = (*MaterialPoints)[i].position[1];
	}
}