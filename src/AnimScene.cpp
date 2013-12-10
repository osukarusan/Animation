#include "AnimScene.h"
#include "GL/glut.h"
#include "ModelLoader.h"
#include "defines.h"
#include <fstream>


unsigned int AnimScene::NUM_AGENTS = 0;

AnimScene::AnimScene(void)
{
	m_system = 0;
	m_grid = 0;
	m_drawPathId = -1;
}


AnimScene::~AnimScene(void)
{
	if (m_system)	
		delete m_system;

	if (m_grid)
		delete m_grid;

	for (unsigned int i = 0; i < m_models.size(); i++)
		delete m_models[i];

	for (unsigned int i = 0; i < m_agents.size(); i++) 
		delete m_agents[i];
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

NavigationGrid* loadGrid() 
{
	int nrows, ncols;
	float width, height;
	std::vector<std::vector<int> > cells;

	std::fstream fin;
	fin.open("data/scene.txt", std::fstream::in);
	fin >> AnimScene::NUM_AGENTS;
	fin >> nrows >> ncols;
	fin >> width >> height;
	cells.resize(nrows);
	for (int i = 0; i < nrows; i++) {
		cells[i].resize(ncols);
		for (int j = 0; j < ncols; j++) {
			fin >> cells[i][j];
		}
	}
	fin.close();

	NavigationGrid* grid = new NavigationGrid();
	grid->init(cells, -0.5f*width, -0.5f*height, 0.5f*width, 0.5f*height);
	return grid;
}

void AnimScene::initScene() 
{
	// preload models
	std::vector<ModelData*> modelsData;
	loadModels(modelsData);
	int NUM_MODELS = modelsData.size();

	// load scene
	std::cout << "Loading scene description ..." << std::endl;
	m_grid = loadGrid();

	// agents
	std::cout << std::endl;
	std::cout << "Initializing agents..." << std::endl;
	m_system = new ParticleSystem();
	for (int i = 0; i < NUM_AGENTS; i++) {
		std::cout << "    Agent " << i << std::endl;

		Particle* p = new Particle();
		Vec3d pos;
		do {
			pos = Vec3d(20.0*random01() - 10, 0, 20.0*random01() - 10);
		} while (!m_grid->walkable(pos));
		p->pos = p->prevPos = pos;
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

		std::vector<Vec3d> apath;
		do {
			Vec3d dest;
			do {
				dest = Vec3d(20.0*random01() - 10, 0, 20.0*random01() - 10);
			} while (!m_grid->walkable(dest));
			m_grid->findPath(pos, dest, apath);
		} while (apath.size() <= 0);
		m_agents.back()->followPath(apath);
	}

	// world limits (min_y is not at 0 since we do not want to detect collisions with it)
	m_boxContainer.setPosition(Vec3d(-10, -10, -10));
	m_boxContainer.setSize(Vec3d(20, 20, 20));
	m_boxContainer.useInnerSide(true);

}


void AnimScene::update(double dt) 
{
	if (dt > 0.20) dt = 0.20;

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
	for (int i = 0; i < m_agents.size(); i++) {
		m_agents[i]->update(dt);
		if (m_agents[i]->finishedCurrentPath()) {
			std::vector<Vec3d> apath;
			do {
				Vec3d dest;
				do {
					dest = Vec3d(20.0*random01() - 10, 0, 20.0*random01() - 10);
				} while (!m_grid->walkable(dest));
				m_grid->findPath(m_agents[i]->getPosition(), dest, apath);
			} while (apath.size() <= 0);
			m_agents[i]->followPath(apath);
		}
	}

}

void AnimScene::draw() 
{
	// create obstacle stencil mask
	std::vector<Vec4d> obstacles;
	m_grid->getObstacleAreas(obstacles);

	glEnable(GL_STENCIL_TEST);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    glStencilFunc(GL_NEVER, 1, 0xFF);
    glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);
	glStencilMask(0xFF);
	glClear(GL_STENCIL_BUFFER_BIT); 
	glBegin(GL_QUADS);
	for (unsigned int i = 0; i < obstacles.size(); i++) {
		Vec4d& q = obstacles[i];
		glVertex3f(q[0], 0.0f, q[1]);
		glVertex3f(q[0], 0.0f, q[3]);
		glVertex3f(q[2], 0.0f, q[3]);
		glVertex3f(q[2], 0.0f, q[1]);
	}
	glEnd();

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);
	glStencilMask(0x00);
	glStencilFunc(GL_EQUAL, 0, 0xFF);

	// Draw a floor surface rectangle
	int nrows, ncols;
	m_grid->getCells(nrows, ncols);
	Vec4d a = m_grid->getArea();
	
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_quadsTexId);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS); 
		glTexCoord2f( 0,    nrows);	glVertex3f(a[0],0, a[3]); 
		glTexCoord2f(ncols, nrows);	glVertex3f(a[2],0, a[3]);
		glTexCoord2f(ncols,  0);	glVertex3f(a[2],0, a[1]); 
		glTexCoord2f( 0,     0);	glVertex3f(a[0],0, a[1]);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glLineWidth(2.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	for (unsigned int i = 0; i < obstacles.size(); i++) {
		Vec4d& q = obstacles[i];
		glBegin(GL_LINE_LOOP);
			glVertex3f(q[0], 0.0f, q[1]);
			glVertex3f(q[0], 0.0f, q[3]);
			glVertex3f(q[2], 0.0f, q[3]);
			glVertex3f(q[2], 0.0f, q[1]);
		glEnd();
	}
	glLineWidth(1.0f);
	glBegin(GL_LINE_LOOP); 
		glVertex3f(a[0],0, a[3]); 
		glVertex3f(a[2],0, a[3]);
		glVertex3f(a[2],0, a[1]); 
		glVertex3f(a[0],0, a[1]);
	glEnd();

	glDisable(GL_STENCIL_TEST);

	// Draw the coordinate axis: x-->red,y-->green,z-->blue
	/*glBegin(GL_LINES);
		glColor3f(1,0,0); glVertex3f(0,0,0); glVertex3f(1,0,0); //x axis
		glColor3f(0,1,0); glVertex3f(0,0,0); glVertex3f(0,1,0); //y axis
		glColor3f(0,0,1); glVertex3f(0,0,0); glVertex3f(0,0,1); //z axis
	glEnd();*/

	// Draw agents
	glEnable(GL_LIGHTING);
	glColor3f(1, 1, 1);
	for (int i = 0; i < m_agents.size(); i++) {
		m_agents[i]->draw(i == m_drawPathId);
	}
}
