#ifndef _COLLISION_TEST_
#define _COLLISION_TEST_

#include "Particle.h"

class CollisionTest
{
public:
	CollisionTest(void);
	~CollisionTest(void);

	virtual void getBoundingBox(Vec3d& min, Vec3d& max) = 0;
	virtual bool testCollision(const Particle* particle, double eps, Vec3d& pos, Vec3d& nor) = 0;

	virtual void move(const Vec3d& v) {};	
};

#endif