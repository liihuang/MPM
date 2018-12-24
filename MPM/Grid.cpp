#include "Grid.h"


Grid::Grid(float _interval_x, float _interval_y, Vector2f _size) : size(_size), interval_x(_interval_x), interval_y(_interval_y) {
	x_num = size[0] / interval_x + 2 + 2 * BSPLINE_RADIUS;
	y_num = size[1] / interval_y + 2 + 2 * BSPLINE_RADIUS;
	float area = interval_x * interval_y;
	for (int y = -BSPLINE_RADIUS; y < y_num - BSPLINE_RADIUS; ++y) {
		for (int x = -BSPLINE_RADIUS; x < x_num - BSPLINE_RADIUS; ++x) {
			eulerianNodes.push_back(EulerianNode(Vector2f(x * interval_x, y * interval_y), area));
		}
	}
}

Grid::Grid(int resolution_x, int resolution_y, Vector2f _size) : x_num(resolution_y), y_num(resolution_y), size(_size) {
	interval_x = size[0] / (x_num - 1) + 2 * BSPLINE_RADIUS;
	interval_y = size[1] / (y_num - 1) + 2 * BSPLINE_RADIUS;
	float area = interval_x * interval_y;
	for (int y = -BSPLINE_RADIUS; y < y_num - BSPLINE_RADIUS; ++y) {
		for (int x = -BSPLINE_RADIUS; x < x_num - BSPLINE_RADIUS; ++x) {
			eulerianNodes.push_back(EulerianNode(Vector2f(x * interval_x, y * interval_y), area));
		}
	}
}
/*Grid::Grid(float _interval, Vector2f _size) : size(_size), interval(_interval) {
	x_num = size[0] / interval + 2;
	y_num = size[1] / interval + 2;
	for (int y = 0; y < y_num; ++y) {
		for (int x = 0; x < x_num; ++x) {
			EulerGrid.push_back(EulerianNode(Vector2f(x * interval, y * interval), interval * interval));
		}
	}

	PtoG();
	//densities
	float cell_volume = pow(interval, 2);
	for (int i = 0, length = obj.size(); i < length; ++i) {
		LargrangianParticle &currentParticle = (*obj)[i];

		const Vector2f &x_p = currentParticle.getPotision();
		int node_x = x_p[0] / interval, node_y = x_p[1] / interval;

		for (int temp_node_y = node_y - 1, index = 0; temp_node_y <= node_y + 2; ++temp_node_y) {
			for (int temp_node_x = node_x - 1;temp_node_x <= node_x + 2; ++temp_node_x, ++index) {
				currentParticle.density += EulerGrid[temp_node_y * x_num + temp_node_x].mass * currentParticle.weight[index] / cell_volume;
			}
		}
	}
	//volumes
	for (int i = 0, length = obj.size(); i < length; ++i) {
		(*obj)[i].volume = (*obj)[i].mass / (*obj)[i].density;
	}
}*/

#if ENABLE_IMPLICIT
Matrix2f Grid::getHessian(const int &i, const int &j) const {
	int index;
	if (j > i)//upper triangle
		index = j * (j + 1) / 2 + i;
	else//lower triangle 
		index = i * (i + 1) / 2 + j;
	int nodes_num = x_num * y_num;

	if (index < 0 || index >= nodes_num * (nodes_num + 1) / 2) return Matrix2f::Zero();
	else return Hessian[index];
}

void Grid::setHessian(const int &i, const int &j, const Matrix2f &m) {
	int index;
	if (j > i)//upper triangle
		index = j * (j + 1) / 2 + i;
	else//lower triangle 
		index = i * (i + 1) / 2 + j;
	int nodes_num = x_num * y_num;
	if (index < 0 || index >= nodes_num * (nodes_num + 1) / 2) return;
	else {
		Hessian[index].setData(m);
	}
}
#endif

void Grid::PtoG(Scene &scene) {
	reset();//reset Eulerian Grid
	for (int i = 0, length = scene.getParticleNum(); i < length; ++i) {// for each Largrangian particle
		LargrangianParticle &currentParticle = scene.getMaterialPointAt(i);
		currentParticle.computeSigma();

		const Vector2f &x_p = currentParticle.position;
		//float inv_interval_x = 1.0 / interval_x, inv_interval_y = 1.0 / interval_x;
		int node_x = x_p[0] / interval_x, node_y = x_p[1] / interval_y;

		for (int temp_node_y = node_y - 1, index = 0; temp_node_y <= node_y + 2; ++temp_node_y) {
			
			float yp_yi = x_p[1] / interval_y - temp_node_y;// 1/h(yp - yi)
			float weight_y = cubic_B_Spline(yp_yi);//N (1/h(yp - yi))

			float weight_gradient0_y = weight_y,   //N (1/h(yp - yi))
				weight_gradient1_y = cubic_B_Spline_grad(x_p[1] / interval_y - temp_node_y);//N'(1/h(yp - yi));

			for (int temp_node_x = node_x - 1; temp_node_x <= node_x + 2; ++temp_node_x, ++index) {
				
				int node_index = temp_node_y * x_num + temp_node_x;
				float xp_xi = x_p[0] / interval_x - temp_node_x;

				float weight_x = cubic_B_Spline(xp_xi);//N (1/h(xp - xi))

				float weight_gradient0_x = cubic_B_Spline_grad(xp_xi),//N'(1/h(xp - xi))
					weight_gradient1_x = weight_x;       //N (1/h(xp - xi))

				/*-----compute w_ip, delta_w_ip-----*/
				currentParticle.weight[index] = weight_x * weight_y;
				currentParticle.weight_gradient[index] = Vector2f(weight_gradient0_x * weight_gradient0_y / interval_y,
					weight_gradient1_x * weight_gradient1_y / interval_y);

				/*-----active grid node-----*/
				eulerianNodes[node_index].active = true;
				
				/*-----transfer mass-----*/
				eulerianNodes[node_index].mass += currentParticle.mass * currentParticle.weight[index];

				/*-----transfer momentum-----*/
				eulerianNodes[node_index].velocity += currentParticle.mass * currentParticle.velocity * currentParticle.weight[index];

				/*-----transfer temperature multiplied by mass-----*/
				eulerianNodes[node_index].temperature += currentParticle.mass * currentParticle.temperature * currentParticle.weight[index];

				/*-----compute grid forces-----*/
				if (currentParticle.weight[index] > BSPLINE_EPSILON) {
					eulerianNodes[node_index].explicitForce -= currentParticle.Sigma * currentParticle.weight_gradient[index];
				}
			}
		}
	}
}

void Grid::updateGridVelocity() {
	for (int i = 0, length = eulerianNodes.size(); i < length; ++i) {
		if (eulerianNodes[i].active && eulerianNodes[i].mass > 1e-7) {
			eulerianNodes[i].velocity /= eulerianNodes[i].mass;
			eulerianNodes[i].velocity_new = eulerianNodes[i].velocity + TIMESTEP * (GRAVITY + eulerianNodes[i].explicitForce / eulerianNodes[i].mass);

			eulerianNodes[i].temperature /= eulerianNodes[i].mass;
		}
	}
}

void Grid::collision() {
	for (int i = 0, length = eulerianNodes.size(); i < length; ++i) {
		if (eulerianNodes[i].active) {
			Vector2f newPos = eulerianNodes[i].position + eulerianNodes[i].velocity_new * TIMESTEP;
			if (newPos[0] < 0.0 || newPos[0] > size[0]) {
				//EulerGrid[i].velocity[0] = -Sticky * EulerGrid[i].velocity[0];
				eulerianNodes[i].velocity_new[0] = -0.1 * eulerianNodes[i].velocity_new[0];
				eulerianNodes[i].velocity_new[1] *= STICKY;
			}
			if (newPos[1] < 0.0 || newPos[1] > size[1]) {
				eulerianNodes[i].velocity_new[0] *= STICKY;
				//EulerGrid[i].velocity[1] = -Sticky * EulerGrid[i].velocity[1];
				eulerianNodes[i].velocity_new[1] = -0.1 * eulerianNodes[i].velocity_new[1];
			}
		}
	}
}

void Grid::reset() {
	for (int i = 0, length = eulerianNodes.size(); i < length; ++i) {
		if (eulerianNodes[i].active) {
			eulerianNodes[i].active = false;
			eulerianNodes[i].mass = eulerianNodes[i].density = eulerianNodes[i].temperature = 0;
			eulerianNodes[i].velocity = Vector2f(0.0, 0.0);
			eulerianNodes[i].velocity_new = Vector2f(0.0, 0.0);
			eulerianNodes[i].explicitForce = Vector2f(0.0, 0.0);
		}
	}
	//if (!eulerianNodes.empty()) memset(&eulerianNodes[0], 0, sizeof(EulerianNode) * eulerianNodes.size());
}