#include "EulerIntegrator.h"

EulerIntegrator::EulerIntegrator(void)
{
}

EulerIntegrator::~EulerIntegrator(void)
{
}

void EulerIntegrator::doStep(PhysicsSystem *system, float h) {
	
	// x_{t+1} = x_t + t * d(x)/dt
	Vecd x0 = system->getState();
	Vecd dx = system->getDerivative();	
	Vecd x1 = x0 + h*dx;
	system->setPreviousState(x0);
	system->setState(x1);
	system->setTime(system->getTime() + h);

}
