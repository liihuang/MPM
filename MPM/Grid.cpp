#include "Grid.h"

Grid::Grid(Vector2f _size, float _interval, std::shared_ptr<std::vector<LargrangianParticle>>_obj) : size(_size), interval(_interval), obj(_obj) {
	x_num = size[0] / interval + 2;
	y_num = size[1] / interval + 2;
	for (int y = 0; y < y_num; ++y) {
		for (int x = 0; x < x_num; ++x) {
			EulerGrid.push_back(EulerianNode(Vector2f(x * interval, y * interval), interval * interval));
		}
	}

	PtoG();
	/*-----compute particle volumes and densities-----*/
	//densities
	float cell_volume = pow(interval, 2);
	for (int i = 0, length = obj->size(); i < length; ++i) {
		LargrangianParticle &currentParticle = (*obj)[i];

		const Vector2f &x_p = currentParticle.getPotision();
		int node_x = x_p[0] / interval, node_y = x_p[1] / interval;

		for (int temp_node_y = node_y - 1, index = 0; temp_node_y <= node_y + 2; ++temp_node_y) {
			for (int temp_node_x = node_x - 1; temp_node_x <= node_x + 2; ++temp_node_x, ++index) {
				currentParticle.density += EulerGrid[temp_node_y * x_num + temp_node_x].mass * currentParticle.weight[index] / cell_volume;
			}
		}
	}
	//volumes
	for (int i = 0, length = obj->size(); i < length; ++i) {
		(*obj)[i].volume = (*obj)[i].mass / (*obj)[i].density;
	}
}

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

void Grid::PtoG() {
	reset();//reset Eulerian Grid
	for (int i = 0, length = obj->size(); i < length; ++i) {// for each Largrangian particle
		LargrangianParticle &currentParticle = (*obj)[i];
		currentParticle.computeSigma();

		const Vector2f &x_p = currentParticle.position;
		int node_x = x_p[0] / interval, node_y = x_p[1] / interval;

		for (int temp_node_y = node_y - 1, index = 0; temp_node_y <= node_y + 2; ++temp_node_y) {
			if (temp_node_y < 0 || temp_node_y > y_num) continue;
			
			float yp_yi = x_p[1] / interval - temp_node_y;
			float weight_y = cubic_B_Spline(yp_yi);//N (1/h(yp - yi))

			float weight_gradient0_y = weight_y,   //N (1/h(yp - yi))
				weight_gradient1_y = cubic_B_Spline_grad(x_p[1] / interval - temp_node_y);//N'(1/h(yp - yi));

			for (int temp_node_x = node_x - 1; temp_node_x <= node_x + 2; ++temp_node_x, ++index) {
				if (temp_node_x < 0 || temp_node_x > x_num) continue;
				
				int node_index = temp_node_y * x_num + temp_node_x;
				float xp_xi = x_p[0] / interval - temp_node_x;
				
				float weight_x = cubic_B_Spline(xp_xi);//N (1/h(xp - xi))

				float weight_gradient0_x = cubic_B_Spline_grad(xp_xi),//N'(1/h(xp - xi))
					weight_gradient1_x = weight_x;       //N (1/h(xp - xi))

				/*-----compute w_ip, delta_w_ip-----*/
				currentParticle.weight[index] = weight_x * weight_y;
				currentParticle.weight_gradient[index] = Vector2f(weight_gradient0_x * weight_gradient0_y / interval,
					weight_gradient1_x * weight_gradient1_y / interval);

				/*-----active grid node-----*/
				EulerGrid[node_index].active = true;
				/*-----transfer mass-----*/
				EulerGrid[node_index].mass += currentParticle.mass * currentParticle.weight[index];
				/*-----compute grid forces-----*/
				if (currentParticle.weight[index] > BSPLINE_EPSILON) {
					EulerGrid[node_index].explicitForce -= currentParticle.Sigma * currentParticle.weight_gradient[index];
				}
				/*-----debug-----*/
				if (EulerGrid[node_index].explicitForce.length() > 1e-7) {
					int stop = 1;
				}
				/*-----debug-----*/
			}
		}
	}
	/*-----transfer velocity from particles to grid-----*/
	for (int i = 0, length = obj->size(); i < length; ++i) {
		LargrangianParticle &currentParticle = (*obj)[i];

		const Vector2f &x_p = currentParticle.position;
		int node_x = x_p[0] / interval, node_y = x_p[1] / interval;

		for (int temp_node_y = node_y - 1, index = 0; temp_node_y <= node_y + 2; ++temp_node_y) {
			for (int temp_node_x = node_x - 1; temp_node_x <= node_x + 2; ++temp_node_x, ++index) {
				float weight = currentParticle.weight[index];
				if (weight > BSPLINE_EPSILON) {
					int node_index = temp_node_y * x_num + temp_node_x;
					EulerGrid[node_index].velocity += currentParticle.mass * currentParticle.velocity * weight;//v_i
				}
				//int node_index = temp_node_y * x_num + temp_node_x;
				//EulerGrid[node_index].velocity += currentParticle.mass * currentParticle.velocity * currentParticle.weight[index];//v_i
			}
		}
	}
}

void Grid::updateGridVelocity() {
	for (int i = 0, length = EulerGrid.size(); i < length; ++i) {
		if (EulerGrid[i].active && EulerGrid[i].mass > 1e-7) {
			EulerGrid[i].velocity /= EulerGrid[i].mass;
			EulerGrid[i].velocity_new = EulerGrid[i].velocity + TimeStep * (Gravity + EulerGrid[i].explicitForce / EulerGrid[i].mass);
		}
	}
}

void Grid::collision() {
	for (int i = 0, length = EulerGrid.size(); i < length; ++i) {
		if (EulerGrid[i].active) {
			Vector2f newPos = EulerGrid[i].position + EulerGrid[i].velocity_new * TimeStep;
			if (newPos[0] < BSPLINE_RADIUS * interval || newPos[0] > size[0] - BSPLINE_RADIUS * interval) {
				//EulerGrid[i].velocity[0] = -Sticky * EulerGrid[i].velocity[0];
				EulerGrid[i].velocity_new[0] = 0;
				EulerGrid[i].velocity_new[1] *= Sticky;
			}
			if (newPos[1] < BSPLINE_RADIUS * interval || newPos[1] > size[1] - BSPLINE_RADIUS * interval) {
				EulerGrid[i].velocity_new[0] *= Sticky;
				//EulerGrid[i].velocity[1] = -Sticky * EulerGrid[i].velocity[1];
				EulerGrid[i].velocity_new[1] = 0;
			}
		}
	}
}

void Grid::reset() {
	for (int i = 0, length = EulerGrid.size(); i < length; ++i) {
		if (EulerGrid[i].active) {
			EulerGrid[i].active = false;
			EulerGrid[i].mass = EulerGrid[i].density = 0;
			EulerGrid[i].velocity = Vector2f(0.0, 0.0);
			EulerGrid[i].velocity_new = Vector2f(0.0, 0.0);
			EulerGrid[i].explicitForce = Vector2f(0.0, 0.0);
		}
	}
}