#include "nt_05_shapeApp.h"

void shapeApp::init() {
	//glRotatef( 90, 1.f, 0.f, 0.f);
	//glRotatef( -10, 0.f, 1.f, 0.f);
	glRotatef(-90, 0.f, 0.f, 1.f);

	///////////////////////////////////////////////////////////////
	////////////////////////////////////////////// OBJECT POSITIONS
	Vec3 pt0 = Vec3(-1.25, 0, 0);
	Vec3 pt1 = Vec3(-.375, 0, 0);
	Vec3 pt2 = Vec3(.375, 0, 0);
	Vec3 pt3 = Vec3(0, 0, 0); //Vec3 pt3 = Vec3(1.25, 0, 0);

	Mat4(&pt2).translate(Vec3(0, 0, .5));
	Mat4(&pt1).translate(Vec3(0, 0, -.5));

	///////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////// OBJECTS
	ntVertex vertex0 = ntVertex(&pt1, 2, Col4f(1, 0, 0, 1));
	ntVertex vertex1 = ntVertex(&pt2, 2, Col4f(1, 0, 0, 1));
	ntVertex vertex2 = ntVertex(&pt0, 2, Col4f(1, 1, 0, 1));

	sphereA = ntSphere(pt1, Vec3(0, 0, 0), Vec3(.25, .5, .5), 50, 50);
	sphereB = ntSphere(pt2, Vec3(0, 0, 0), Vec3(.25, .5, .5), 50, 50);
	sphereC = ntSphere(pt2, Vec3(0, 0, 0), Vec3(.25, .5, .5), 35, 35);
	sphereD = ntSphere(pt3, Vec3(0, 0, 0), Vec3(.25, .5, .5), 25, 25);

	///////////////////////////////////////////////////////////////
	/////////////////////////////////////// OBJECTS COLOR BY VERTEX
	int val = sphereA.verts.size();
	for (int i = 0; i<val; i++){
		float r = 1 / (val / ((float)i + 1));
		float g = 0;
		float b = .5 - (.25 / (val / ((float)i + 1)));
		float a = 1 / (val / ((float)i + 1));
		sphereA.verts.at(i)->setColor(Col4f(r, g, b, a));
	}
	val = sphereC.verts.size();
	for (int i = 0; i<val; i++){
		float r = 1 / (val / ((float)i + 1));
		float g = 0;
		float b = .5 - (.25 / (val / ((float)i + 1)));
		float a = 1 / (val / ((float)i + 1));
		sphereC.verts.at(i)->setColor(Col4f(r, g, b, a));
	}
	val = sphereD.verts.size();
	for (int i = 0; i<val; i++){
		float r = 1 / (val / ((float)i + 1));
		float g = .5 + (.5 / (val / ((float)i + 1)));
		float b = 1 - (1 / (val / ((float)i + 1)));
		float a = 1 / (val / ((float)i + 1));
		sphereD.verts.at(i)->setColor(Col4f(r, g, b, a));
	}
}

void shapeApp::run(){
	display();
}

void shapeApp::display(){
	//glRotatef((float)glfwGetTime() * .001, 1.f, 1.f, 0.f);
	//glRotatef(.05, 1.f, 0.f, 0.f);
	glRotatef(.01, 0.f, 1.f, 0.f);  //ROTATION ABOUT Z
	///////////////////////////////////////////////////////////////
	
	sphereA.displayVerts(2);
	//sphereB.displayNorms(1);
	//sphereC.display();
	sphereC.displayEdges(.1);
	//sphereD.display();
	sphereD.displayNorms(.12);

	///////////////////////////////////////////////////////////////
}