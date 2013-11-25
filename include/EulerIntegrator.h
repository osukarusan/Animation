#ifndef _EULER_INTEGRATOR_
#define _EULER_INTEGRATOR_

#include "Integrator.h"

class EulerIntegrator : public Integrator
{
public:

	EulerIntegrator();
	~EulerIntegrator();

	virtual void doStep(PhysicsSystem* system, float time);

};

#endif