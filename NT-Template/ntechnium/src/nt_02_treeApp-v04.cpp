#include "nt_02_treeApp-v04.h"

void treeApp04::init() {
	///////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////// OBJECTS
	bool fixL = true;
	float gravity = .007; //.0002;
	float rl = .6;
	float k = 0.01;
	float d = 0.5;
	float z = -.3;

	tree00 = ntTree(&Vec3(0, 0, 0), Vec3(-0.5, 0, .5), 10, 5, true, false);
	tree00.setMutations(3, 3, 12, 0.8); //45 & 90 degrees causes error
	tree00.setDynamics(.05, k, d);
	//tree00.setMutations(5, 3, 25, 0.20);
	//tree00.setRandomize(0, 10, 0, 45, 1.0, 5.5);
	//tree00.setFixedRoot(false);
	//tree01.setFixedBase(false);
	tree00.setFixedLeaves(fixL);
	tree00.setLeaf_Z(0.5);
	tree00.init();

	nodes = ntNodeGen(tree00.branches);
	nodes.set_Parameters(POLYPARAM, 5, 0.05);
	nodes.init();

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

	///////////////////////////////////////////////////////////////
	/////////////////////////////////////// APPLY FORCES TO NETWORK

	std::vector<Particle*>* particlesPtr00 =	tree00.getParticles();
	std::vector<Spring*>* springsPtr00 =		tree00.getSprings();

	force00 = ntBasePhysics(ntVec3(0, 0, 0),.08);
	force00.setGravity(-gravity);
	//force00.setForce(Vec3(0, .5, 0), .00001);
	force00.setParticles(particlesPtr00);
	force00.setSprings(springsPtr00);
}

void treeApp04::run(){
	//force00.run();
	display();
}

void treeApp04::display(){
	tree00.display();
	nodes.display_Edges();
}