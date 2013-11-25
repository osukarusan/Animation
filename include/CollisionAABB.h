#ifndef _COLLISION_AABB_
#define _COLLISION_AABB_

#include "CollisionTest.h"
#include "defines.h"

class CollisionAABB : public CollisionTest
{
public:
	CollisionAABB(void);
	~CollisionAABB(void);

	virtual void getBoundingBox(Vec3d& min, Vec3d& max);
	virtual bool testCollision(const Particle* particle, double eps, Vec3d& pos, Vec3d& nor);

	Vec3d getPosition() const;
	Vec3d getSize() const;

	void setPosition(const Vec3d& p);
	void setSize(const Vec3d& s);
	void useInnerSide(bool inner);

	virtual void move(const Vec3d &v);

private:

	Vec3d	m_position;
	Vec3d	m_size;
	bool	m_inner;
	
};

inline Vec3d CollisionAABB::getPosition() const {
	return m_position;
}

inline Vec3d CollisionAABB::getSize() const {
	return m_size;
}

inline void CollisionAABB::setPosition(const Vec3d &p) {
	m_position = p;
}

inline void CollisionAABB::setSize(const Vec3d &s) {
	m_size = s;
}

inline void CollisionAABB::move(const Vec3d &v) {
	m_position += v;
}

inline void CollisionAABB::useInnerSide(bool inner) {
	m_inner = inner;
}

#endif