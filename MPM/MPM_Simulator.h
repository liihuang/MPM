#ifndef MPM_SIMULATOR_H
#define MPM_SIMULATOR_H

#include "Grid.h"
#include "Object.h"

class MPM_Simulator {
	std::shared_ptr<Grid> grid;
	std::shared_ptr<Object> object;
public:
	MPM_Simulator(std::shared_ptr<Object> _object) : object(_object), grid(new Grid(Vector2f(WIN_METER_X, WIN_METER_Y), 0.005, _object->getMP())) {	}

	void update();
	void createVAO(std::vector<float> &v);
};

#endif // !MPM_SIMULATOR_H