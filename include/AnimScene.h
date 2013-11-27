#ifndef _ANIM_SCENE_H_
#define _ANIM_SCENE_H_

#include "ParticleSystem.h"
#include "EulerIntegrator.h"
#include "CollisionAABB.h"
#include "Agent.h"
#include <vector>

#define NUM_AGENTS	5


class AnimScene
{
public:
	AnimScene(void);
	~AnimScene(void);

	void initScene();
	void update(double time);
	void draw();

private:

	ParticleSystem* m_system;
	EulerIntegrator m_integrator;
	CollisionAABB   m_boxContainer;

	std::vector<Model*> m_models;
	std::vector<Agent*> m_agents;

};

#endif