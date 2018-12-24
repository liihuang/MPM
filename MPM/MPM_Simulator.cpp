#include "MPM_Simulator.h"

void MPM_Simulator::init() {
	grid.PtoG(scene);
	/*-----compute particle volumes and densities-----*/
	//densities

	float interval_x = grid.getIntervalX(), interval_y = grid.getIntervalY(), area = interval_x * interval_y;
	int x_num = grid.getX_num(), y_num = grid.getY_num();
	for (int i = 0, length = scene.getParticleNum(); i < length; ++i) {
		LargrangianParticle &currentParticle = scene.getMaterialPointAt(i);

		const Vector2f &x_p = currentParticle.getPotision();
		int node_x = x_p[0] / interval_x, node_y = x_p[1] / interval_y;

		for (int temp_node_y = node_y - 1, index = 0; temp_node_y <= node_y + 2; ++temp_node_y) {
			for (int temp_node_x = node_x - 1;temp_node_x <= node_x + 2; ++temp_node_x, ++index) {
				currentParticle.density += grid.getNode(temp_node_y * x_num + temp_node_x).mass * currentParticle.weight[index] / area;
			}
		}
	}
	//volumes
	for (int i = 0, length = scene.getParticleNum(); i < length; ++i) {
		scene.getMaterialPointAt(i).volume = scene.getMaterialPointAt(i).mass / scene.getMaterialPointAt(i).density;
	}
}

void MPM_Simulator::createVAO(std::vector<float> &v) {
	scene.createVAO(v);
}

void MPM_Simulator::advance() {
	grid.PtoG(scene);//700ms->562ms
	grid.updateGridVelocity();//9ms
	grid.collision();//9ms

	scene.GtoP(grid);//550ms->170ms
	scene.updateF();//250ms->200ms
	scene.collision();//10ms
	scene.advection();//5ms
}