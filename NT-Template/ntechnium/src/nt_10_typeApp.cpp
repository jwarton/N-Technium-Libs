#include "nt_10_typeApp.h"
#include <GL/glu.h>

void typeApp::init() {
	///////////////////////////////////////////////////////////////
	ntVec3 pt0 = Vec3(0, 0, 0);
	cubeA = ntCube(pt0, 2);

	font = "AGENCYB";
	ntVec3* pt1 = new ntVec3(5, 535, 0);
	title = ntType(pt1, 36, font, "N-TECHNIUM");
}

void typeApp::run(){
	display();
}

void typeApp::display(){
	///////////////////////////////////////////////////////////////
	//cubeA.display();
	//cubeA.displayVerts(2);
	cubeA.displayEdges(.1);
	//cubeA.displayNorms(.12);

	///////////////////////////////////////////////////////////////
	////////////////////////////////////////////  2D DISPLAY CONENT
	//view_Orth();  ///REFACTOR THIS FUNCTION TO BASE CONTENT CLASS
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, appWidth, 0, appHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	title.display();
}