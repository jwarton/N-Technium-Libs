#include "nt_05_cubeApp.h"

void cubeApp::init() {
	///////////////////////////////////////////////////////////////
	////////////////////////////////////////////// OBJECT POSITIONS
	Vec3 pt0 = Vec3(0, 0, 0);


	///////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////// OBJECTS
	//ntVertex vertex0 = ntVertex(&pt0, 2, Col4f(1, 1, 0, 1));
	cubeA = ntCube(pt0, 2); 

	///////////////////////////////////////////////////////////////
	/////////////////////////////////////// OBJECTS COLOR BY VERTEX
}

void cubeApp::run(){
	display();
}

void cubeApp::display(){
	///////////////////////////////////////////////////////////////
	//cubeA.display();
	//cubeA.display_verts(2);
	cubeA.display_edges(.1);
	//cubeA.display_norms(.12);

	///////////////////////////////////////////////////////////////
}