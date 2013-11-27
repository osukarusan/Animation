#include "Agent.h"

Agent::Agent(void)
{
}

Agent::~Agent(void)
{
}

void Agent::init(int t, Model* m, Particle* p, float h)
{
	m_type	   = t;
	m_model    = m;
	m_particle = p;
	m_rotation = 0.0f;
	m_height   = h;

	Vec3f bmin, bmax;
	m_model->getBoundingBox(bmin, bmax);
	m_scale    = h/(bmax[1] - bmin[1]);
	m_offVert  = 0; //-bmin[1];
}

void Agent::update(float dt) {
	m_model->onUpdate(dt);
	m_rotation = atan2(m_particle->vel[0], m_particle->vel[2])*RAD2DEG;
}

void Agent::draw() {
	glPushMatrix();
	glTranslatef(m_particle->pos[0], m_particle->pos[1] + m_offVert, m_particle->pos[2]);
	glScalef(m_scale, m_scale, m_scale);
	glRotatef(m_rotation, 0, 1, 0);
	glRotatef(-90, 1, 0, 0);
	m_model->onRender();
	glPopMatrix();
}
