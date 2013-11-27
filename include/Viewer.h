#ifndef _VIEWER_H_
#define _VIEWER_H_

#include "AnimScene.h"

class Viewer
{
public:
	Viewer(void);
	~Viewer(void);

	void Init(int argc, char* argv[]);
	void BeginLoop();

	static void Display(void);
	static void Idle();

	static void Reshape(int w, int h);

	static void Keyboard(unsigned char key, int x, int y);
	static void MouseButtHandler(int button, int state, int x, int y);
	static void MouseMoveHandler(int x, int y);

	static int win_w, win_h;
	static float tx, ty, tz;
	static float rx, ry;
	static int ox, oy, buttonState;
	static bool wire;

private:

	static AnimScene* scene;
	static unsigned long long lastTime;
	static unsigned int quadsTexId;

	void InitLight();
	void InitTextures();

};


#endif