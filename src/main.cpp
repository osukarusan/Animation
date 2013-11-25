#include "GL/glut.h"
#include "Viewer.h"

void main(int argc, char** argv)
{
	Viewer v; //init graphical window
	v.Init(argc, argv);
	v.BeginLoop(); //init execution loop
}

