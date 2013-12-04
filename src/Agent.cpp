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

	float sx = m_scale*0.5f*(bmax[0] - bmin[0]);
	float sz = m_scale*0.5f*(bmax[2] - bmin[2]);
	m_radius = std::sqrt(sx*sx + sz*sz);

	m_nextWaypoint = 0;
}

void Agent::followPath(const std::vector<Vec3d>& path) {
	m_path = path;
	m_nextWaypoint = 0;
}

void Agent::update(float dt) {
	
	// follow path
	if (m_nextWaypoint < m_path.size()) {

		Vec3d& p  = m_path[m_nextWaypoint];

		Vec3d dir = p - m_particle->pos;
		float dist = len(dir);
		normalise(dir);
		Vec3d vel = m_particle->vel;
		float speed = len(vel);
		normalise(vel);

		m_particle->vel = speed*norm(vel + 0.5*dir);

		if (len(m_path[m_nextWaypoint] - m_particle->pos) < m_radius)
			m_nextWaypoint++;
	}

	m_model->onUpdate(dt);
	m_rotation = std::atan2(m_particle->vel[0], m_particle->vel[2])*RAD2DEG;
}

void Agent::draw(bool drawPath) {
	glPushMatrix();
	glTranslatef(m_particle->pos[0], m_particle->pos[1] + m_offVert, m_particle->pos[2]);
	glScalef(m_scale, m_scale, m_scale);
	glRotatef(m_rotation, 0, 1, 0);
	glRotatef(-90, 1, 0, 0);
	m_model->onRender();
	glPopMatrix();

	if (drawPath) {
		glLineWidth(2.0f);
		glColor3f(0, 1, 0);
		glBegin(GL_LINE_STRIP);
		glVertex3f(m_particle->pos[0], m_particle->pos[1], m_particle->pos[2]);
		for (unsigned int i = m_nextWaypoint; i < m_path.size(); i++) {
			Vec3d& p = m_path[i];
			glVertex3f(p[0], p[1], p[2]);
		}
		glEnd();
		glColor3f(1, 0, 0);
		glPointSize(4.0f);
		glBegin(GL_POINTS);
		for (unsigned int i = 0; i < m_path.size(); i++) {
			Vec3d& p = m_path[i];
			glVertex3f(p[0], p[1], p[2]);
		}
		glEnd();
	}
}
