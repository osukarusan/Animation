#include "viewer.h"
#include "GL/glut.h"

int Viewer::win_w = 640;
int Viewer::win_h = 640;

int Viewer::ox = 0;
int Viewer::oy = 0;
int Viewer::buttonState = 0;

float Viewer::tx =   0.0f;
float Viewer::ty =   0.0f;
float Viewer::tz = -15.0f;
float Viewer::rx =  90.0f;
float Viewer::ry =   0.0f;

bool Viewer::wire = false;

unsigned int Viewer::quadsTexId = 0;
unsigned long long Viewer::lastTime = 0;

AnimScene* Viewer::scene = 0;


Viewer::Viewer(void) {
}

Viewer::~Viewer(void) {
}

void Viewer::Init(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(win_w, win_h);
	glutInitWindowPosition(50,50);

	glutCreateWindow("Animation Viewer");
	glutDisplayFunc(Display);
	glutIdleFunc(Idle);
	glutKeyboardFunc(Keyboard);
	glutReshapeFunc (Reshape);
	glutMouseFunc   (MouseButtHandler);
    glutMotionFunc  (MouseMoveHandler);

	glClearColor(0.5f, 0.5f, 0.7f, 1.0f);
	Reshape(win_w, win_h); 

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	InitLight();
	InitTextures();

	if (scene) delete scene;
	scene = new AnimScene();
	scene->initScene();

	lastTime = getTime();
}

void Viewer::InitLight()
{
	static float ambient[]             = {0.2, 0.2, 0.2, 1.0};
	static float diffuse[]             = {0.6, 0.6, 0.6, 1.0};

	GLfloat position[] = {1.0,0.0,0.0,0.0};
	GLfloat position1[] = {-1.0,0.0,0.0,0.0};
	GLfloat position2[] = {0.0,0.0,1.0,0.0};

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glEnable( GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE,  diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, position2);

	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
}

void Viewer::InitTextures()
{
	static float texdata[3*4*4] = {	0.78f, 0.78f, 0.82f, 
									0.58f, 0.58f, 0.65f,
									0.58f, 0.58f, 0.65f,
									0.78f, 0.78f, 0.82f
	};

	glGenTextures(1, &quadsTexId);
	glBindTexture(GL_TEXTURE_2D, quadsTexId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, texdata);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Viewer::BeginLoop()
{
	glutMainLoop();
}

void Viewer::Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(tx, ty, tz);
	glRotatef(rx, 1.0, 0.0, 0.0);
	glRotatef(ry, 0.0, 1.0, 0.0);
	
	// Draw a floor surface rectangle
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, quadsTexId);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS); 
		glTexCoord2f( 0, 10);	glVertex3f(-10,0, 10); 
		glTexCoord2f(10, 10);	glVertex3f( 10,0, 10);
		glTexCoord2f(10,  0);	glVertex3f( 10,0,-10); 
		glTexCoord2f( 0,  0);	glVertex3f(-10,0,-10);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	// Draw the coordinate axis: x-->red,y-->green,z-->blue
	glBegin(GL_LINES);
		glColor3f(1,0,0); glVertex3f(0,0,0); glVertex3f(1,0,0); //x axis
		glColor3f(0,1,0); glVertex3f(0,0,0); glVertex3f(0,1,0); //y axis
		glColor3f(0,0,1); glVertex3f(0,0,0); glVertex3f(0,0,1); //z axis
	glEnd();
	glEnable(GL_LIGHTING);

	// Draw particle system
	scene->draw();

	glutSwapBuffers();
}


void Viewer::Idle() {

	unsigned long long fr = getTimerFrequency();
	unsigned long long t0 = lastTime;
	unsigned long long t1 = getTime();
	lastTime = t1;

	scene->update(double(t1 - t0)/fr);
	glutPostRedisplay();
}


void Viewer::Keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case  0x1B: // esc
		case  'q':
			exit(0);
			break; 
	}
	glutPostRedisplay();
}


void Viewer::MouseButtHandler(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)		buttonState |= 1<<button;
	else if (state == GLUT_UP)	buttonState = 0;

	int mods = glutGetModifiers();
	if (mods & GLUT_ACTIVE_SHIFT)		buttonState = 2;
	else if (mods & GLUT_ACTIVE_CTRL)	buttonState = 3;
  
	ox = x; oy = y;
	glutPostRedisplay();
}

void Viewer::MouseMoveHandler(int x, int y)
{
    
	float dx, dy;
	dx = float(x - ox);
	dy = float(y - oy);
	
	if (buttonState == 3) {
	  // left+middle = zoom
	  tz -= dx / 100.0f;
	} 
	else if (buttonState & 2) {
	  // middle = translate
	  tx += dx / 100.0f;
	  ty -= dy / 100.0f;
	}
	else if (buttonState & 1) {
      // left = rotate
      rx += dy / 5.0f;
      ry += dx / 5.0f;
	}
	ox = x; oy = y;
	glutPostRedisplay();
}

void Viewer::Reshape(int w, int h)
{
	win_w = w; win_h = h;

    if(w <= 0 || h <=0) return;

    glViewport(0,0,w,h);
    glutReshapeWindow(w,h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(80.0, (float) w / (float)h, 0.1, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h);
}

