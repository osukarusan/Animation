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
		collision = true;

		// X planes
		if (((p->pos[0] - bmax[0]) < eps) && ((bmin[0] - p->pos[0]) < eps)) {
			if (p->prevPos[0] <= bmin[0]) {
				pos[0] = bmin[0];
				nor[0] = -1;
			}
			else if (p->prevPos[0] >= bmax[0]) {
				pos[0] = bmax[0];
				nor[0] = 1;
			}
			else {
				pos[0] = p->pos[0];
				nor[0] = 0;
			}
		}
		else collision = false;
		// Y planes
		if (((p->pos[1] - bmax[1]) < eps) && ((bmin[1] - p->pos[1]) < eps)) {
			if (p->prevPos[1] <= bmin[1]) {
				pos[1] = bmin[1];
				nor[1] = -1;
			}
			else if (p->prevPos[1] >= bmax[1]) {
				pos[1] = bmax[1];
				nor[1] = 1;
			}
			else {
				pos[1] = p->pos[1];
				nor[1] = 0;
			}
		}
		else collision = false;
		// Z planes
		if (((p->pos[2] - bmax[2]) < eps) && ((bmin[2] - p->pos[2]) < eps)) {
			if (p->prevPos[2] <= bmin[2]) {
				pos[2] = bmin[2];
				nor[2] = -1;
			}
			else if (p->prevPos[2] >= bmax[2]) {
				pos[2] = bmax[2];
				nor[2] = 1;
			}
			else {
				pos[2] = p->pos[2];
				nor[2] = 0;
			}
		}
		else collision = false;
	}

	if (collision) nor.Normalise();
	
	return collision;
}