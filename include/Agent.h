#ifndef _AGENT_H_
#define _AGENT_H_

#include "Model.h"
#include "Particle.h"

class Agent
{
public:
	Agent(void);
	~Agent(void);

	void init(int type, Model* m, Particle* p, float height);
	void update(float dt);
	void draw(bool drawPath = false);

	void followPath(const std::vector<Vec3d>& path);
	bool finishedCurrentPath() const;

	int		getType() const;
	Vec3d	getPosition() const;
	
private:

	int			m_type;
	Model*		m_model;
	float		m_rotation;
	float		m_scale;
	float		m_height;
	float		m_offVert;
	float		m_radius;
	float		m_velocity;
	
	Particle*	m_particle;

	std::vector<Vec3d>  m_path;
	unsigned int		m_nextWaypoint;

};

inline int Agent::getType() const {
	return m_type;
}

inline Vec3d Agent::getPosition() const {
	return m_particle->pos;
}

inline bool Agent::finishedCurrentPath() const {
	return m_nextWaypoint >= m_path.size();
}


#endif