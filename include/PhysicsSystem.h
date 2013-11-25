#ifndef _PHYSICS_SYSTEM_
#define _PHYSICS_SYSTEM_

#include "defines.h"

class PhysicsSystem
{
public:
	PhysicsSystem(void);
	~PhysicsSystem(void);

	virtual int  getDimension()			const = 0;
	virtual Vecd getState()				const = 0;
	virtual Vecd getPreviousState()		const = 0;
	virtual Vecd getDerivative()		const = 0;
	virtual Vecd getSecondDerivative()	const = 0;

	virtual void setState(const Vecd& state) = 0;
	virtual void setPreviousState(const Vecd& prev) = 0;

	double getTime() const;
	void   setTime(double t);

protected:

	double	m_time;

};

inline double PhysicsSystem::getTime() const {
	return m_time;
}

inline void PhysicsSystem::setTime(double t) {
	m_time = t;
}

#endif