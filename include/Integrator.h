#ifndef _INTEGRATOR_
#define _INTEGRATOR_

#include "PhysicsSystem.h"

class Integrator
{
public:

	Integrator();
	~Integrator();

	virtual void doStep(PhysicsSystem* system, float time) = 0;

};

#endif