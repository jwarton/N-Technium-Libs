#include "nt_05_convexhullApp.h"

void convexApp::init() {
	///////////////////////////////////////////////////////////////
	////////////////////////////////////////////// OBJECT POSITIONS
	/// /////////////////////////////
	std::vector <ntVec3*> pt_cloud;

	for (int i = 0; i < 4; i++) {
		Vec3* pt0 = new Vec3(-i * 1, i * 1, -i * 1);
		pt_cloud.push_back(pt0);
	}
	/// /////////////////////////////
	ntVec3* vA = new ntVec3(-15, -15,  15);

	/// /////////////////////////////
	ntVec3* vB = new ntVec3( 15,  15,  15);
	ntVec3* vC = new ntVec3( 15, -15,  15);
	ntVec3* vD = new ntVec3(-15, -15, -15);
	ntVec3* vE = new ntVec3( 15,  15, -15);

	std::vector <ntVec3*> vs;
	vs.push_back(vB);
	vs.push_back(vC);
	vs.push_back(vD);
	vs.push_back(vE);
	///////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////// OBJECTS
	polyhedron = ntConvexHull(&vs);

	polyhedron.add_point(vA);
	polyhedron.add_points(&pt_cloud);
}

void convexApp::run(){
	display();
}

void convexApp::display(){
	///////////////////////////////////////////////////////////////
	polyhedron.display();

	///////////////////////////////////////////////////////////////
}