#include "nt_01_meshApp.h"

void meshApp::init() {
	std::cout << "\ninit meshApp" << endl;

	glRotatef( 60, 1.f, 0.f, 0.f);
	//glRotatef( -60, 0.f, 1.f, 0.f);
	glRotatef(-30, 0.f, 0.f, 1.f);
	///////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////// OBJECTS
	mesh00 = ntPlane(Vec3(0,0,0),1,1,50,50);

	///////////////////////////////////////////////////////////////
	/////////////////////////////////////// OBJECTS COLOR BY VERTEX

	for (int i = 0; i < mesh00.verts.size(); i++){
		int val = mesh00.faces.size();
		float r = mapRange(.5,  1, 0, val, i, true);
		float g = mapRange(.25, .4, 0, val, i,false);
		float b = mapRange( 0, .3, 0, val, i, true);
		float a = mapRange(-.25, 1, 0, val, i, true);
		float s = mapRange(1, 5, 0, val, i, true);
		mesh00.verts.at(i)->setSize(s);
		mesh00.verts.at(i)->setColor(ntColor4f(r, g, b, 1));
	}
	
	for (int i = 0; i < mesh00.faces.size(); i++){
		int val = mesh00.faces.size();
		float n = mapRange(.01,.25, 0, val, i, false);
		float w = mapRange(.01, 3, 0, val, i, false);
		float r = mapRange(.25, .5, 0, val, i, false);
		float g = mapRange(0, .25, 0, val, i, true);
		float b = mapRange(0, .8, 0, val, i, true);
		float a = mapRange(.5, 1, 0, val, i, false);
		mesh00.faces.at(i).normal.setLength(n);
		mesh00.faces.at(i).normal.colS = ntColor4f(.55, .65, .50, a);
	}

	mesh00.addPhysics(true);
	mesh00.fixCorners();

	std::vector<Particle*>* particlesPtr00 = mesh00.getParticles();
	std::vector<Spring*>* springsPtr00 = mesh00.getSprings();

	force00 = ntBasePhysics(ntVec3(0, 0, 0), .01);
	force00.setGravity(-.0001);
	//force00.setForce(Vec3(5, 0, 0), .1);
	std::cout << springsPtr00->size() << endl;

	force00.setParticles(particlesPtr00);
	force00.setSprings(springsPtr00);

	int ps = particlesPtr00->size();

	for (int i = 0; i < 3; i++){
		int pFixed = 465+i;// (rand() % ps);
		//particlesPtr00->at(pFixed)->fixed = true;
	}
}

void meshApp::run(){
	//force00.setForce(Vec3((float)glfwGetTime() *.001, .5, 0), .00001);
	force00.run();
	display();
}

void meshApp::display(){
	//glRotatef((float)glfwGetTime() * .001, 1.f, 1.f, 0.f);
	//glRotatef(.05, 1.f, 0.f, 0.f);
	//glRotatef(.05, 0.f, 0.f, 1.f);  //ROTATION ABOUT Z
	///////////////////////////////////////////////////////////////
	
	//mesh00.display();
	//mesh00.displayNorms();
	//mesh00.displayVerts();
	mesh00.displayEdges(.1);
	///////////////////////////////////////////////////////////////
}