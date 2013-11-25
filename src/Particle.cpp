#include "Particle.h"

Particle::Particle(void)
{
	pos		= Vec3d(0.0, 0.0, 0.0);
	prevPos = Vec3d(0.0, 0.0, 0.0);
	vel		= Vec3d(0.0, 0.0, 0.0);
	prevVel = Vec3d(0.0, 0.0, 0.0);
	force	= Vec3d(0.0, 0.0, 0.0);
	mass	= 1.0;
	color	= Vec3f(1.0f, 1.0f, 1.0f);
}

Particle::~Particle(void)
{
}

Particle::Particle(const Vec3d& p, const Vec3d& v, float m) {
	pos		= p;
	prevPos = p;
	vel		= v;
	prevVel = v;
	force	= Vec3d(0.0, 0.0, 0.0);
	mass	= m;
	color	= Vec3f(1.0f, 1.0f, 1.0f);
}

Particle::Particle(const Particle& p) {
	pos   = p.pos;
	vel   = p.vel;
	force = p.force;
	mass  = p.mass;
	color = p.color;
}