#include "AnimScene.h"
#include "GL/glut.h"
#include "ModelLoader.h"
#include "defines.h"


AnimScene::AnimScene(void)
{
	m_system = 0;
}


AnimScene::~AnimScene(void)
{
	if (m_system)	
		delete m_system;

	for (unsigned int i = 0; i < m_models.size(); i++)
		delete m_models[i];
	m_models.clear();

	for (unsigned int i = 0; i < m_agents.size(); i++) 
		delete m_agents[i];
	m_agents.clear();
}


void loadModels(std::vector<ModelData*>& models)
{
	ModelData* data;

	// load 'skeleton' model
	std::cout << "Loading 'skeleton' model ..." << std::endl;
	data = ModelLoader::getModel("skeleton", "data/skeleton/", "data/skeleton.cfg");
	if (!data) {
		std::cerr << "Model initialization failed! (skeleton)" << std::endl;
		return;
	}
	models.push_back(data);
	std::cout << std::endl;

	// load 'paladin' model
	std::cout << "Loading 'paladin' model ..." << std::endl;
	data = ModelLoader::getModel("paladin", "data/paladin/", "data/paladin.cfg");
	if (!data) {
		std::cerr << "Model initialization failed! (paladin)" << std::endl;
		return;
	}
	models.push_back(data);
	std::cout << std::endl;
}



void AnimScene::initScene() 
{
	// preload models
	std::vector<ModelData*> modelsData;
	loadModels(modelsData);
	int NUM_MODELS = modelsData.size();

	// particles
	m_system = new ParticleSystem();
	for (int i = 0; i < NUM_AGENTS; i++) {
		Particle* p = new Particle();
		p->pos = p->prevPos = Vec3d(20.0*random01() - 10, 0, 20.0*random01() - 10);
		p->vel = p->prevVel = (1.5*random01() + 0.5) * norm(Vec3d(random01(), 0, random01()));
		m_system->addParticle(p);	

		int t = rand()%NUM_MODELS;
		float h = 0.25*random01() + 1.6;
		Vec3f blend = Vec3f(random01(), random01(), len(p->vel)*random01()).Normalise();

		Model* m = new Model();
		m->setCoreModel(modelsData[t]->coreModel);
		m->setAnimationIds(modelsData[t]->animationIds);
		m->setModelScale(modelsData[t]->renderScale);
		m->onInit();
		m->setMotionBlend(&blend[0], 0);
		m->onUpdate(10*random01());
		m_models.push_back(m);

		Agent* a = new Agent();
		a->init(t, m, p, h);
		m_agents.push_back(a);
	}

	// world limits (min_y is not at 0 since we do not want to detect collisions with it)
	m_boxContainer.setPosition(Vec3d(-10, -10, -10));
	m_boxContainer.setSize(Vec3d(20, 20, 20));
	m_boxContainer.useInnerSide(true);

}


void AnimScene::update(double dt) 
{
	int numParticles = m_system->getNumParticles();

	// physics integration
	m_integrator.doStep(m_system, dt);

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

	// agents
	for (int i = 0; i < m_agents.size(); i++)
		m_agents[i]->update(dt);

}

void AnimScene::draw() 
{
	glColor3f(1, 1, 1);
	for (int i = 0; i < m_agents.size(); i++) {
		m_agents[i]->draw();
	}
}
