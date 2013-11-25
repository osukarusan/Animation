#include "CollisionAABB.h"

CollisionAABB::CollisionAABB(void)
{
	m_position = Vec3d(-1.0, -1.0, -1.0);
	m_size     = Vec3d( 2.0,  2.0,  2.0);
	m_inner    = false;
}

CollisionAABB::~CollisionAABB(void)
{
}

void CollisionAABB::getBoundingBox(Vec3d& min, Vec3d& max) {
	min = m_position;
	max = m_position + m_size;
}

bool CollisionAABB::testCollision(const Particle *p, double eps, Vec3d& pos, Vec3d& nor) 
{
	Vec3d bmin = m_position;
	Vec3d bmax = m_position + m_size;

	bool collision = false;
	nor = Vec3d(0,0,0);
	pos = p->pos;

	if (m_inner) {
		// X planes
		if ((p->pos[0] - bmin[0]) < eps) {
			pos[0] = bmin[0]; 
			nor[0] = 1;
			collision = true;
		}
		else if ((bmax[0] - p->pos[0]) < eps) {
			pos[0] = bmax[0];
			nor[0] = -1;
			collision = true;
		}
		// Y planes
		if ((p->pos[1] - bmin[1]) < eps) {
			pos[1] = bmin[1]; 
			nor[1] = 1;
			collision = true;
		}
		else if ((bmax[1] - p->pos[1]) < eps) {
			pos[1] = bmax[1];
			nor[1] = -1;
			collision = true;
		}
		// Z planes
		if ((p->pos[2] - bmin[2]) < eps) {
			pos[2] = bmin[2]; 
			nor[2] = 1;
			collision = true;
		}
		else if ((bmax[2] - p->pos[2]) < eps) {
			pos[2] = bmax[2];
			nor[2] = -1;
			collision = true;
		}
	}
	else {
		// X planes
		if ((p->pos[0] - bmax[0]) < eps) {
			pos[0] = bmax[0]; 
			nor[0] = 1;
			collision = true;
		}
		else if ((bmin[0] - p->pos[0]) < eps) {
			pos[0] = bmin[0];
			nor[0] = -1;
			collision = true;
		}
		// Y planes
		if ((p->pos[1] - bmax[1]) < eps) {
			pos[1] = bmax[1]; 
			nor[1] = 1;
			collision = true;
		}
		else if ((bmin[1] - p->pos[1]) < eps) {
			pos[1] = bmin[1];
			nor[1] = -1;
			collision = true;
		}
		// Z planes
		if ((p->pos[2] - bmax[2]) < eps) {
			pos[2] = bmax[2]; 
			nor[2] = 1;
			collision = true;
		}
		else if ((bmin[2] - p->pos[2]) < eps) {
			pos[2] = bmin[2];
			nor[2] = -1;
			collision = true;
		}
	}

	if (collision) nor.Normalise();
	
	return collision;
}