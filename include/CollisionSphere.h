#ifndef _COLLISION_SPHERE_
#define _COLLISION_SPHERE_

#include "CollisionTest.h"
#include "defines.h"

class CollisionSphere : public CollisionTest
{
public:
	CollisionSphere(void);
	~CollisionSphere(void);

	virtual void getBoundingBox(Vec3d& min, Vec3d& max);
	virtual bool testCollision(const Particle* particle, double eps, Vec3d& pos, Vec3d& nor);

	Vec3d getPosition() const;
	double getRadius() const;

	void setPosition(const Vec3d& p);
	void setRadius(double r);
	void useInnerSide(bool inner);

	virtual void move(const Vec3d& v);

private:

	Vec3d	m_center;
	double  m_radius;
	bool	m_inner;

};

inline Vec3d CollisionSphere::getPosition() const {
	return m_center;
}

inline double CollisionSphere::getRadius() const {
	return m_radius;
}

inline void CollisionSphere::setPosition(const Vec3d &p) {
	m_center = p;
}

inline void CollisionSphere::setRadius(double r) {
	m_radius = r;
}

inline void CollisionSphere::move(const Vec3d &v) {
	m_center += v;
}

inline void CollisionSphere::useInnerSide(bool inner) {
	m_inner = inner;
}

#endif