#ifndef _ANIM_SCENE_H_
#define _ANIM_SCENE_H_

#include "ParticleSystem.h"
#include "EulerIntegrator.h"
#include "CollisionAABB.h"
#include "Agent.h"
#include "NavigationGrid.h"
#include <vector>

class AnimScene
{
public:
	AnimScene(void);
	~AnimScene(void);

	void initScene();
	void update(double time);
	void draw();

	int  getDrawPathId() const;
	void setDrawPathId(int id);

	void setFloorTextureId(unsigned int id);

	static unsigned int NUM_AGENTS;

private:

	ParticleSystem* m_system;
	EulerIntegrator m_integrator;

	// TODO: change this for a vector of containers
	CollisionAABB   m_boxContainer;

	std::vector<Model*> m_models;
	std::vector<Agent*> m_agents;
	NavigationGrid*		m_grid;

	int m_drawPathId;

	unsigned int m_quadsTexId;

};

inline void AnimScene::setDrawPathId(int id) {
	m_drawPathId = id;
}

inline int AnimScene::getDrawPathId() const {
	return m_drawPathId;
}

inline void AnimScene::setFloorTextureId(unsigned int id) {
	m_quadsTexId = id;
}

#endif