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
	void draw();

	int	 getType();
	
private:

	int			m_type;
	Model*		m_model;
	float		m_rotation;
	float		m_scale;
	float		m_height;
	float		m_offVert;
	Particle*	m_particle;

};

inline int Agent::getType() {
	return m_type;
}

#endif