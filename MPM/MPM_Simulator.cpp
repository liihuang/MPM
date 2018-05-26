#include "MPM_Simulator.h"

void MPM_Simulator::createVAO(std::vector<float> &v) {
	object->createVAO(v);
}

void MPM_Simulator::update() {
	grid->PtoG();//700ms->562ms
	grid->updateGridVelocity();//9ms
	grid->collision();//9ms

	object->GtoP(*grid);//550ms->170ms
	object->updateF();//250ms->200ms
	object->collision();//10ms
	object->advection();//5ms
}