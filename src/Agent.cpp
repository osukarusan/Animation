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
	m_velocity = len(p->vel);
	m_rotation = std::atan2(m_particle->vel[0], m_particle->vel[2])*RAD2DEG;

	Vec3f bmin, bmax;
	m_model->getBoundingBox(bmin, bmax);
	m_scale    = h/(bmax[1] - bmin[1]);
	m_offVert  = 0; //-bmin[1];

	float sx = m_scale*0.5f*(bmax[0] - bmin[0]);
	float sz = m_scale*0.5f*(bmax[2] - bmin[2]);
	m_radius = 0.5*std::sqrt(sx*sx + sz*sz);

	m_nextWaypoint = 0;
}

void Agent::followPath(const std::vector<Vec3d>& path) {
	m_path = path;
	m_nextWaypoint = 0;
}

void Agent::update(float dt, const std::vector<CollisionSphere>& obs, const std::vector<Agent*>& agents) {
	
	// current state
	float speed    = len(m_particle->vel);
	Vec3d vcurrent = m_particle->vel;

	// follow path
	Vec3d seekSteering(0,0,0);
	if (m_nextWaypoint < m_path.size()) {
		Vec3d& wp = m_path[m_nextWaypoint];
		// Steering: seek
		Vec3d vdesired = speed*norm(wp - m_particle->pos);
		seekSteering   = vdesired - vcurrent;
		// check if reached waypoint
		if (len(wp - m_particle->pos) < 1.5*m_radius)
			m_nextWaypoint++;
	}

	// Steering: obstacle avoidance
	Vec3d obsSteering(0,0,0);
	Vec3d  veldir  = norm(vcurrent);
	double minDist = 5*m_radius;
	for (unsigned int i = 0; i < obs.size(); i++) {
		Vec3d  toObs   = obs[i].getPosition() - m_particle->pos;
		double obsDist = dot(veldir, toObs);
		if (obsDist > 0 && obsDist < minDist) {
			Vec3d obsLateral = toObs - obsDist*veldir;
			double sideDist = len(obsLateral);
			if (sideDist < obs[i].getRadius() + m_radius) {
				Vec3d steerDir    = -norm(obsLateral);
				double dirEffect  = max(dot(veldir, norm(toObs)), 0);
				float steerWeight = min(0.5*m_radius/obsDist, 1.0);
				obsSteering       = dirEffect*steerWeight*steerDir;
				minDist = obsDist;
			}
		}
	}

	// Steering: unaligned collision avoidance
	Vec3d collSteering(0,0,0);
	Vec3d pfpos = m_particle->pos + 2*dt*m_particle->vel;
	minDist = 4*m_radius;
	for (unsigned int i = 0; i < agents.size(); i++) {
		Vec3d apos = agents[i]->getParticle()->pos;
		Vec3d avel = agents[i]->getParticle()->vel;
		if (dot(norm(avel), veldir) < 0.8) {
			Vec3d afpos     = apos + 2*dt*avel;
			double collDist = len(afpos - pfpos);
			if (collDist < minDist) {
				double collWeight = (0.4*random01()+0.8)*min(0.5*m_radius/collDist, 1.0);
				collSteering = collWeight*norm(pfpos - afpos);
				minDist = collDist;
			}
		}
	}


	// Steering sum
	const double SW = 0.6;
	const double OW = 1.2;
	const double CW = 0.7;
	speed = min(speed + 0.1, m_velocity);
	Vec3d vresult   = speed*norm(vcurrent + SW*seekSteering + OW*obsSteering + CW*collSteering);
	m_particle->vel = vresult;

	m_model->onUpdate(dt);

	// update rotation
	double curRot = m_rotation;
	double desRot = std::atan2(m_particle->vel[0], m_particle->vel[2])*RAD2DEG;
	double difRot = desRot - curRot;
	if (abs(difRot) > 180) difRot = curRot - desRot;
	m_rotation = curRot + max(min(difRot, 360*dt), -360*dt);
	if (m_rotation > 180)   m_rotation -= 360;
	if (m_rotation < -180)  m_rotation += 360;
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
