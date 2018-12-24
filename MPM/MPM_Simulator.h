#ifndef MPM_SIMULATOR_H
#define MPM_SIMULATOR_H

#include "Grid.h"
#include "Scene.h"

class MPM_Simulator {
	Grid grid;
	Scene scene;
	float delta_t;
public:
	MPM_Simulator() = default;
	MPM_Simulator(Scene _scene, float _delta_t) : scene(_scene), delta_t(_delta_t), grid((float)0.005, (float)0.005, Vector2f(WIN_METER_X, WIN_METER_Y)) { init(); }

	void init();
	void setTimeStep(float _delta_t) { delta_t = _delta_t; }
	void advance();
	void createVAO(std::vector<float> &v);
};

#endif // !MPM_SIMULATOR_H