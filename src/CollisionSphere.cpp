#include "CollisionSphere.h"

CollisionSphere::CollisionSphere(void)
{
	m_center = Vec3d(0.0, 0.0, 0.0);
	m_radius   = 1.0;
	m_inner    = false;
}

CollisionSphere::~CollisionSphere(void)
{
}

void CollisionSphere::getBoundingBox(Vec3d& min, Vec3d& max) {
	min = m_center - m_radius*Vec3d(1,1,1);
	max = m_center + m_radius*Vec3d(1,1,1);
}

bool CollisionSphere::testCollision(const Particle *p, double eps, Vec3d& pos, Vec3d& nor) 
{
	bool   collision = false;
	Vec3d  toCenter = m_center - p->pos;
	double dist = len(toCenter);
	
	if (m_inner) {
		collision = (m_radius - dist < eps);
	}
	else {
		collision = (dist - m_radius < eps);
	}

	if (collision) {
		Vec3d  v = p->pos - p->prevPos;
		double a = dot(v, v);
		double b = 2*dot(v, p->prevPos - m_center);
		double c = dot(m_center, m_center) + dot(p->prevPos, p->prevPos) 
				   - 2*dot(p->prevPos, m_center) - m_radius*m_radius;
		double det2 = b*b - 4*a*c;
		double lambda = 0;
		if (det2 >= 0) {
			double det = std::sqrt(det2);
			double lambda1 = (-b + det)/(2*a);
			double lambda2 = (-b - det)/(2*a);
			if (lambda1 > 0 && lambda2 > 0) lambda = min(lambda1, lambda2);
			else if (lambda1 > 0)			lambda = lambda1;
			else if (lambda2 > 0)			lambda = lambda2;
		}

		if (lambda > 0 && lambda <= 1) {
			pos = p->prevPos + lambda*v;
		}
		else {
			v = p->prevPos - m_center;
			normalise(v);
			pos = m_center + m_radius*v;
		}

		if (m_inner) nor = m_center - pos;
		else		 nor = pos - m_center;
		normalise(nor);
	}

	return collision;
}
