#include "AnimScene.h"
#include "configs.h"
#include "GL/glut.h"
#include "ModelLoader.h"


AnimScene::AnimScene(void)
{
	m_system = 0;
}


AnimScene::~AnimScene(void)
{
	if (m_system)	delete m_system;
}

void AnimScene::initScene() 
{
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

	loadModels();
}

void AnimScene::loadModels()
{
	Model *pModel;
	ModelData* data;

	// load 'skeleton' model
	std::cout << "Loading 'skeleton' model ..." << std::endl;
	data = ModelLoader::getModel("skeleton", "data/skeleton/", "data/skeleton.cfg");
	if (data) {
		pModel = new Model();
		pModel->setCoreModel(data->coreModel);
		pModel->setAnimationIds(data->animationIds);
		pModel->onInit();
		m_models.push_back(pModel);
	}
	else {
		std::cerr << "Model initialization failed! (skeleton)" << std::endl;
		return;
	}
	std::cout << std::endl;

	// load 'paladin' model
	std::cout << "Loading 'paladin' model ..." << std::endl;
	data = ModelLoader::getModel("paladin", "data/paladin/", "data/paladin.cfg");
	if (data) {
		pModel = new Model();
		pModel->setCoreModel(data->coreModel);
		pModel->setAnimationIds(data->animationIds);
		pModel->onInit();
		m_models.push_back(pModel);
	}
	else {
		std::cerr << "Model initialization failed! (paladin)" << std::endl;
		return;
	}
	std::cout << std::endl;

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

	// models
	for (int i = 0; i < m_models.size(); i++)
		m_models[i]->onUpdate(timeStep);

}

void AnimScene::draw() 
{
	int numParticles = m_system->getNumParticles();
	glColor3f(1, 1, 1);
	for (int i = 0; i < numParticles; i++) {
		Particle *p = m_system->getParticle(i);
		glPushMatrix();
		glTranslatef(p->pos[0], p->pos[1] + 0.25, p->pos[2]);
		glScalef(0.01, 0.01, 0.01);
		glRotatef(-90, 1, 0, 0);
		m_models[1]->onRender();

		glPopMatrix();
	}

}