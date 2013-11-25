#ifndef _ANIM_SCENE_H_
#define _ANIM_SCENE_H_

#include "ParticleSystem.h"
#include "EulerIntegrator.h"
#include "CollisionAABB.h"


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

};

#endif