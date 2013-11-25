#include "AnimScene.h"
#include "configs.h"
#include "GL/glut.h"


AnimScene::AnimScene(void)
{
	m_system = 0;
}


AnimScene::~AnimScene(void)
{
}

void AnimScene::initScene() 
{
	if (m_system) delete m_system;
	m_system = new ParticleSystem();

	for (int i = 0; i < NUM_AGENTS; i++) {
		Particle* p = new Particle();
		p->pos = p->prevPos = Vec3d(20.0*random01() - 10, 0, 20.0*random01() - 10);
		p->vel = p->prevVel = 0.1 * (5.0*random01() + 2.0) * norm(Vec3d(random01(), 0, random01()));
		m_system->addParticle(p);	
	}

	m_boxContainer.setPosition(Vec3d(-10, -10, -10));
	m_boxContainer.setSize(Vec3d(20, 20, 20));
	m_boxContainer.useInnerSide(true);

}

void AnimScene::update(double timeStep) 
{
	int numParticles = m_system->getNumParticles();

	// physics integration
	m_integrator.doStep(m_system, SIM_TIMESTEP);

	// collision with the container
	double kr  = 1.0;
	double kt  = 1.0;
	double eps = 0.016;
	Vec3d pos, nor;
	for (int i = 0; i < numParticles; i++) {
		Particle *p = m_system->getParticle(i);
		if (m_boxContainer.testCollision(p, eps, pos, nor)) {
			Vec3d velN = dot(nor, p->vel)*nor;
			Vec3d velT = p->vel - velN;
			p->vel = kt*velT - kr*velN;
			p->pos = p->pos - (1 + kr)*(dot(nor, p->pos) - dot(nor, pos))*nor + eps*nor;
		}
	}

}

void AnimScene::draw() 
{
	int numParticles = m_system->getNumParticles();
	glColor3f(1, 0, 0);
	for (int i = 0; i < numParticles; i++) {
		Particle *p = m_system->getParticle(i);
		glPushMatrix();
		glTranslatef(p->pos[0], p->pos[1] + 0.25, p->pos[2]);
		glutSolidSphere(0.25f, 8, 8);
		glPopMatrix();
	}

}