#include "nt_02_treeApp-v02.h"

void treeApp02::init() {
	float gravity00 = 0.045;//.06;
	float gravity01 = 0.04;//.06;
	float rl = .001;
	float k = -5;
	float d = .5;
	float z = 0.5;

	float drag = -.01; //.05
	///////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////// OBJECTS
	tree00 = ntTree(&Vec3(-.25, .25, 0), Vec3(0, 0, 1), .45, 6, false, true);
	tree00.setMutations(4, 3, 60, 0.5);
	//tree00.setDynamics(rl, d, k);
	tree00.setFixedRoot(false);
	tree00.setFixedBase(false);
	tree00.setFixedLeaves(true);
	tree00.setLeaf_Z(z);
	tree00.init();

	tree01 = ntTree(&Vec3(.35, -.25, 0), Vec3(0, 0, 1), .2, 6, true, true);
	tree01.setMutations(5, 3, 5, 0.75);
	//tree01.setDynamics(rl, d, k);
	tree01.setRandomize(0, 5, 0, 45, 1, 1);
	tree01.setFixedRoot(false);
	tree01.setFixedBase(false);
	tree01.setFixedLeaves(true);
	tree01.setLeaf_Z(z);
	tree01.init();

	///////////////////////////////////////////////////////////////
	/////////////////////////////////////// OBJECTS COLOR BY VERTEX
	int brCnt00 = tree00.branches.size();
	for (int i = 0; i < brCnt00; i++){
		float w = mapRange(.01,   2.5, 0, brCnt00, i, false);
		float r = mapRange(.2 ,  1 , 0, brCnt00, i, false);
		float g = mapRange(.4 , .6 , 0, brCnt00, i, false);
		float b = mapRange(.55, .65, 0, brCnt00, i, false);
		float a = 1;
		tree00.branches.at(i)->w = w;
		tree00.branches.at(i)->colS = Col4(r + .02, g, b, a);
		tree00.branches.at(i)->colE = Col4(.8, 0, .2, .5);
	}

	//int brCnt01 = tree01.branches.size();
	//for (int i = 0; i < brCnt01; i++){
	//	float w = mapRange(.01,   2.5, 0, brCnt01, i, false);
	//	float r = mapRange(.3 ,   1, 0, brCnt01, i, false);
	//	float g = mapRange(.3 , .55, 0, brCnt01, i, false);
	//	float b = mapRange(.55, .75, 0, brCnt01, i, false);
	//	float a = 1;
	//	tree01.branches.at(i)->w = w;
	//	tree01.branches.at(i)->colS = Col4(r+.02, g, b, a);
	//	tree01.branches.at(i)->colE = Col4(.8, 0, .2, .5);
	//}

	///////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////// PHYSICS
	std::vector<Particle*>* particlesPtr00 =	tree00.getParticles();
	std::vector<Spring*>* springsPtr00 =		tree00.getSprings();

	int ps = particlesPtr00->size();

	force00 = ntBasePhysics(ntVec3(0, 0, 0), drag);
	force00.setGravity(-gravity00);
	force00.setForce(Vec3(.5, .5, 0), .00001);
	force00.setParticles(particlesPtr00);
	force00.setSprings(springsPtr00);
	///add spring parameters

	std::vector<Particle*>* particlesPtr01 = tree01.getParticles();
	std::vector<Spring*>* springsPtr01 = tree01.getSprings();

	force01 = ntBasePhysics(ntVec3(0, 0, 0), drag);
	force01.setGravity(-gravity01);
	force01.setParticles(particlesPtr01);
	force01.setSprings(springsPtr01);

	glRotatef(-60, 1.f, 0.f, 0.f);
	//glRotatef(-90, 1.f, 0.f, 0.f);
	//glRotatef(-30, 0.f, 0.f, 1.f);
	//glRotatef(-30, 1.f, 1.f, 0.f);
}

void treeApp02::run(){
	///force00.setForce(Vec3((float)glfwGetTime() *.001, .5, 0), .00001);
	force00.run();
	force01.run();
	display();
}

void treeApp02::display(){
	glRotatef(-.01, 0.f, 0.f, 1.f);
	//glRotatef(-.01, 1.f, 0.f, 0.f);
	//glRotatef(-.03, 0.f, 0.f, 1.f);

	tree00.display();
	tree01.display();
}