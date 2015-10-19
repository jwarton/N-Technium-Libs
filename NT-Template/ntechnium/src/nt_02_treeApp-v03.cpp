#include "nt_02_treeApp-v03.h"

void treeApp03::init() {
	///////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////// OBJECTS
	bool fixL = true;
	float gravity = .007; //.0002;
	float rl = .6;
	float k = 0.01;
	float d = 0.5;
	float z = -.3;

	tree00 = ntTree(&Vec3(0, 0, 0), Vec3(-0.5, 0, .5), .25, 7, false, true);
	tree00.setMutations(1, 3, 12, 0.8); //45 & 90 degrees causes error
	tree00.setDynamics(.05, k, d);
	//tree00.setMutations(5, 3, 25, 0.20);
	//tree00.setRandomize(0, 10, 0, 45, 1.0, 5.5);
	//tree00.setFixedRoot(false);
	//tree01.setFixedBase(false);
	tree00.setFixedLeaves(fixL);
	tree00.setLeaf_Z(0.5);
	tree00.init();

	tree01 = ntTree(&Vec3(0, 0, 0), Vec3(.5, -.25, -0.25), .2, 5, false, true);
	tree01.setMutations(5, 3, 20, 0.55);
	tree01.setDynamics(rl, k, d);
	//tree01.setRandomize(0, 5, 0, 45, 1, 1);
	tree01.setFixedLeaves(fixL);
	tree01.setFixedBase(false);
	tree01.setLeaf_Z(z - 0.1);
	tree01.init();

	tree02 = ntTree(&Vec3(0, 0, 0), Vec3(.5, .25, -0.25), .2, 5, false, true);
	tree02.setMutations(5, 3, 20, 0.55);
	tree02.setDynamics(rl, k, d);
	//tree02.setRandomize(0, 5, 0, 45, 1, 1);
	tree02.setFixedLeaves(fixL);
	tree02.setFixedBase(false);
	tree02.setLeaf_Z(z - 0.1);
	tree02.init();

	tree03 = ntTree(&Vec3(0, 0, 0), Vec3(-.36, -.3, 0.35), .18, 6, false, true);
	tree03.setMutations(1, 3, -15, 0.84);
	tree03.setDynamics(.1, .5, d);
	//tree03.setRandomize(0, 5, 0, 45, 1, 1);
	tree03.setFixedLeaves(fixL);
	tree03.setFixedBase(false);
	tree03.setLeaf_Z(.5);
	tree03.init();

	//tree04 = ntTree(&Vec3(0, 0, 0), Vec3(-.36, .3, 0.35), .24, 5, false, true);
	//tree04.setMutations(5, 3, -65, 0.75);
	tree04 = ntTree(&Vec3(0, 0, 0), Vec3(-.36, .3, 0.35), .18, 6, false, true);
	tree04.setMutations(1, 3, -15, 0.84);
	tree04.setDynamics(.1, .5, d);
	//tree04.setRandomize(0, 5, 0, 45, 1, 1);
	tree04.setFixedLeaves(fixL);
	tree04.setFixedBase(false);
	tree04.setLeaf_Z(.5);
	tree04.init();

	///////////////////////////////////////////////////////////////
	/////////////////////////////////////// OBJECTS COLOR BY VERTEX
	///VISUALIZATION PARAMETERS
	int brCnt00 = tree00.branches.size();
	for (int i = 0; i < brCnt00; i++){
		float w = mapRange(.01,  1.5 , 0, brCnt00, i, false);
		float r = 1 + .02;//mapRange(.2 ,  .5 , 0, brCnt00, i, false);
		float g = 1;//mapRange(.4 ,  .5 , 0, brCnt00, i, false);
		float b = 1;//mapRange(.55,  .7, 0, brCnt00, i, false);
		float a = 1;
		tree00.branches.at(i)->w = w;
		tree00.branches.at(i)->colS = Col4(r, g, b, a);
		//tree00.branches.at(i)->colE = Col4(.8, 0, .2, .5);
	}

	int brCnt01 = tree01.branches.size();
	for (int i = 0; i < brCnt01; i++){
		float w = mapRange(.01, 1.5, 0, brCnt01, i, false);
		float r = 1 + .02;//mapRange(.3 ,   1, 0, brCnt01, i, false);
		float g = 1;//mapRange(.3 , .55, 0, brCnt01, i, false);
		float b = 1;//mapRange(.55, .75, 0, brCnt01, i, false);
		float a = mapRange(.2, 1, 0, brCnt01, i, false); //1;
		tree01.branches.at(i)->w = w;
		tree01.branches.at(i)->colS = Col4(r, g, b, a);
		tree01.branches.at(i)->colE = Col4(.5, .5, .5, .5);
		//tree01.branches.at(i)->colE = Col4(.8, 0, .2, .5);

		tree02.branches.at(i)->w = w;
		tree02.branches.at(i)->colS = Col4(r, g, b, a);
		tree02.branches.at(i)->colE = Col4(.5, .5, .5, .5);
		//tree02.branches.at(i)->colE = Col4(.8, 0, .2, .5);

		tree03.branches.at(i)->w = w;
		tree03.branches.at(i)->colS = Col4(r, g, b, a);
		//tree03.branches.at(i)->colE = Col4(.5, .5, .5, .5);
		//tree03.branches.at(i)->colE = Col4(.8, 0, .2, .5);

		tree04.branches.at(i)->w = w;
		tree04.branches.at(i)->colS = Col4(r, g, b, a);
		//tree04.branches.at(i)->colE = Col4(.5, .5, .5, .5);
		//tree04.branches.at(i)->colE = Col4(.8, 0, .2, .5);
	}

	///////////////////////////////////////////////////////////////
	/////////////////////////////////////// APPLY FORCES TO NETWORK

	std::vector<Particle*>* particlesPtr00 =	tree00.getParticles();
	std::vector<Spring*>* springsPtr00 =		tree00.getSprings();

	force00 = ntBasePhysics(ntVec3(0, 0, 0),.08);
	force00.setGravity(-gravity);
	//force00.setForce(Vec3(0, .5, 0), .00001);
	force00.setParticles(particlesPtr00);
	force00.setSprings(springsPtr00);

	std::vector<Particle*>* particlesPtr01 = tree01.getParticles();
	std::vector<Spring*>* springsPtr01 = tree01.getSprings();

	force01 = ntBasePhysics(ntVec3(0, 0, 0), .05);
	force01.setGravity(.0002);
	force01.setParticles(particlesPtr01);
	force01.setSprings(springsPtr01);

	std::vector<Particle*>* particlesPtr02 = tree02.getParticles();
	std::vector<Spring*>* springsPtr02 = tree02.getSprings();

	force02 = ntBasePhysics(ntVec3(0, 0, 0), .05);
	force02.setGravity(.0002);
	force02.setParticles(particlesPtr02);
	force02.setSprings(springsPtr02);

	std::vector<Particle*>* particlesPtr03 = tree03.getParticles();
	std::vector<Spring*>* springsPtr03 = tree03.getSprings();

	force03 = ntBasePhysics(ntVec3(0, 0, 0), .1);
	force03.setGravity(-gravity);
	force03.setParticles(particlesPtr03);
	force03.setSprings(springsPtr03);

	std::vector<Particle*>* particlesPtr04 = tree04.getParticles();
	std::vector<Spring*>* springsPtr04 = tree04.getSprings();

	force04 = ntBasePhysics(ntVec3(0, 0, 0), .1);
	force04.setGravity(-gravity);
	force04.setParticles(particlesPtr04);
	force04.setSprings(springsPtr04);

	glRotatef(-90, 1.f, 0.f, 0.f);
	//glRotatef(90, 0.f, 1.f, 0.f);
	//glRotatef(-90, 0.f, 0.f, 1.f);
	//glRotatef(10, 0.f, 0.f, 1.f);
	//glRotatef(-90, 1.f, 0.f, 0.f);
	//glRotatef(180, 0.f, 0.f, 1.f);
	//glRotatef(15, 1.f,0.f, 0.f);
}

void treeApp03::run(){
	///force00.setForce(Vec3((float)glfwGetTime() *.001, .5, 0), .00001);
	force00.run();
	force01.run();
	force02.run();
	force03.run();
	force04.run();
	display();
}

void treeApp03::display(){
	//glRotatef(-.05, 0.f, 0.f, 1.f);
	//glRotatef(-.05, 0.f, 0.f, 1.f);

	tree00.display();
	//tree01.display();
	tree02.display();
	tree03.display();
	//tree04.display();
}