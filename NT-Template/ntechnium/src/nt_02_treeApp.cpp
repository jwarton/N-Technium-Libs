#include "nt_02_treeApp.h"

void treeApp::init() {
	///////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////// OBJECTS

	///////////////////////////////////////////////////////////////
	/////////////////////////////////////// OBJECTS COLOR BY VERTEX
	//tree00 = ntTree(&Vec3(0, 0, 0), Vec3(  0,  5,   0), .25, 6, true);
	//tree01 = ntTree(&Vec3(0, 0, 0), Vec3(-.2, -.2, .2), .15, 6, true);
	tree00 = ntTree(&Vec3(0, 0, 0), .25, 6, true);
	tree01 = ntTree(&Vec3(0, 0, 0), .15, 6, true);

	//tree00.vecs->at(12)->x = 0;
	//tree00.vecs->at(12)->y = 0;
	//tree00.vecs->at(12)->z = 0;

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

		//cout << "branch Cnt:" << i+1 << " in transform"<<endl;
		//cout << "branch: S " << tree00.branches.at(i)->v0->x << ", " << tree00.branches.at(i)->v0->y << ", " << tree00.branches.at(i)->v0->z;
		//cout << " : E " << tree00.branches.at(i)->v1->x << ", " << tree00.branches.at(i)->v1->y << ", " << tree00.branches.at(i)->v1->z << endl;
	}

	int brCnt01 = tree01.branches.size();
	for (int i = 0; i < brCnt01; i++){
		float w = mapRange(.01,   2.5, 0, brCnt01, i, false);
		float r = mapRange(.3 ,   1, 0, brCnt01, i, false);
		float g = mapRange(.3 , .55, 0, brCnt01, i, false);
		float b = mapRange(.55, .75, 0, brCnt01, i, false);
		float a = 1;
		tree01.branches.at(i)->w = w;
		tree01.branches.at(i)->colS = Col4(r+.02, g, b, a);
		tree01.branches.at(i)->colE = Col4(.8, 0, .2, .5);
	}

	std::vector<Particle*>* particlesPtr00 =	tree00.getParticles();
	std::vector<Spring*>* springsPtr00 =		tree00.getSprings();

	int ps = particlesPtr00->size();
	particlesPtr00->at(10)->fixed = true;
	particlesPtr00->at(125)->fixed = true;
	particlesPtr00->at(ps - 1)->fixed = true;

	for (int i = 0; i < 10; i++){
		int pFixed = (rand() % ps);
		particlesPtr00->at(pFixed)->fixed = true;
	}

	force00 = ntBasePhysics(ntVec3(0, 0, 0),.05);
	force00.setGravity(.0005);
	force00.setForce(Vec3(.5, .5, 0), .00001);
	force00.setParticles(particlesPtr00);
	force00.setSprings(springsPtr00);
	///add spring parameters

	std::vector<Particle*>* particlesPtr01 = tree01.getParticles();
	std::vector<Spring*>* springsPtr01 = tree01.getSprings();

	ps = particlesPtr01->size();
	particlesPtr01->at(10)->fixed = true;
	particlesPtr01->at(125)->fixed = true;
	particlesPtr01->at(ps-1)->fixed = true;

	for (int i = 0; i < 10; i++){
		int pFixed = (rand() % ps);
		particlesPtr01->at(pFixed)->fixed = true;
	}

	force01 = ntBasePhysics(ntVec3(0, 0, 0), .05);
	force01.setGravity(-.0005);
	force01.setParticles(particlesPtr01);
	force01.setSprings(springsPtr01);

}

void treeApp::run(){
	///force00.setForce(Vec3((float)glfwGetTime() *.001, .5, 0), .00001);
	force00.run();
	force01.run();
	display();
}

void treeApp::display(){
	glRotatef(-.03, 0.f, 1.f, 0.f);

	tree00.display();
	tree01.display();
}
///////////////////////////////////////////////////footnote *01
//reference: http://www.opengl.org/discussion_boards/showthread.php/171184-GLM-to-create-gl_NormalMatrix
