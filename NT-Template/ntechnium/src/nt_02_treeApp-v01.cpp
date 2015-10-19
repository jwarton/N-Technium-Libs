#include "nt_02_treeApp-v01.h"

void treeApp01::init() {
	///////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////// OBJECTS
	float rl = .3;
	float d = 0.1;
	float k = 0.2;

	tree00 = ntTree(&Vec3(0, 0, 0), Vec3(0, 0, 1), .25, 5, true);
	tree00.setMutations(5, 3, 38, 0.75);
	tree00.setDynamics(rl, d, k);
	//tree00.setFixedRoot(true);
	//tree00.setFixedBase(false);
	tree00.setFixedLeaves(true);
	tree00.setLeaf_Z(0.5);
	tree00.init();

	tree01 = ntTree(&Vec3(0, 0, 0), Vec3(.48, 0, 1), .2, 5, true);
	tree01.setMutations(5, 3, 15, 0.75);
	tree01.setDynamics(rl, d, k);
	tree01.setFixedBase(false);
	tree01.setFixedLeaves(true);
	tree01.setLeaf_Z(0.25);
	tree01.init();

	///////////////////////////////////////////////////////////////
	/////////////////////////////////////// OBJECTS COLOR BY VERTEX
	int brCnt00 = tree00.branches.size();
	for (int i = 0; i < brCnt00; i++){
		float w = mapRange(.01,   2.5, 0, brCnt00, i, false);
		float r = mapRange(.2, 1, 0, brCnt00, i, false) + .02;
		float g = mapRange(.4 , .6 , 0, brCnt00, i, false);
		float b = mapRange(.55, .65, 0, brCnt00, i, false);
		float a = 1;
		tree00.branches.at(i)->w = w;
		tree00.branches.at(i)->colS = Col4(r, g, b, a);
		tree00.branches.at(i)->colE = Col4(.8, 0, .2, .5);
	}

	int brCnt01 = tree01.branches.size();
	for (int i = 0; i < brCnt01; i++){
		float w = mapRange(.01,   2.5, 0, brCnt01, i, false);
		float r = mapRange(.3, 1, 0, brCnt01, i, false) + .02;
		float g = mapRange(.3 , .55, 0, brCnt01, i, false);
		float b = mapRange(.55, .75, 0, brCnt01, i, false);
		float a = 1;
		tree01.branches.at(i)->w = w;
		tree01.branches.at(i)->colS = Col4(r, g, b, a);
		tree01.branches.at(i)->colE = Col4(.8, 0, .2, .5);
	}

	std::vector<Particle*>* particlesPtr00 = tree00.getParticles();
	std::vector<Spring*>* springsPtr00 = tree00.getSprings();

	force00 = ntBasePhysics(ntVec3(0, 0, 0), .05);
	force00.setGravity(.0005);
	force00.setParticles(particlesPtr00);
	force00.setSprings(springsPtr00);

	std::vector<Particle*>* particlesPtr01 = tree01.getParticles();
	std::vector<Spring*>* springsPtr01 = tree01.getSprings();

	force01 = ntBasePhysics(ntVec3(0, 0, 0), .05);
	force01.setGravity(-.0005);
	force01.setParticles(particlesPtr01);
	force01.setSprings(springsPtr01);

	//glRotatef(-90, 1.f, 0.f, 0.f);
	//glRotatef(90, 0.f, 1.f, 0.f);
}

void treeApp01::run(){
	force00.run();
	force01.run();
	display();
}

void treeApp01::display(){
	//glRotatef(-.03, 0.f, 1.f, 0.f);
	tree00.display();
	tree01.display();
}