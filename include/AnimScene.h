#ifndef _ANIM_SCENE_H_
#define _ANIM_SCENE_H_

#include "ParticleSystem.h"
#include "EulerIntegrator.h"
#include "CollisionAABB.h"
#include "model.h"
#include <vector>

class AnimScene
{
public:
	AnimScene(void);
	~AnimScene(void);

	void initScene();
	void update(double time);
	void draw();

private:

	void loadModels();

	ParticleSystem* m_system;
	EulerIntegrator m_integrator;
	CollisionAABB   m_boxContainer;

	std::vector<Model*> m_models;

};

#endif