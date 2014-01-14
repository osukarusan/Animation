#ifndef _AGENT_H_
#define _AGENT_H_

#include "Model.h"
#include "Particle.h"
#include "CollisionSphere.h"

class Agent
{
public:
	Agent(void);
	~Agent(void);

	void init(int type, Model* m, Particle* p, float height);
	void update(float dt, const std::vector<CollisionSphere>& obs, const std::vector<Agent*>& agents);
	void draw(bool drawPath = false);

	void followPath(const std::vector<Vec3d>& path);
	bool finishedCurrentPath() const;
	bool isStuckOnPath() const;
	Vec3d getDestination() const;

	int		  getType()     const;
	Vec3d	  getPosition() const;
	Particle* getParticle() const;
	float     getRadius()   const;
	
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
	unsigned int		m_framesSinceLastWP;

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

inline Particle* Agent::getParticle() const {
	return m_particle;
}

inline float Agent::getRadius() const {
	return m_radius;
}

inline bool Agent::isStuckOnPath() const {
	return m_nextWaypoint > 0 && m_framesSinceLastWP > 250;
}

inline Vec3d Agent::getDestination() const {
	return m_path.back();
}

#endif