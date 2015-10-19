#include "nt_03_swarmApp.h"

void swarmApp::init() {
	///////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////// OBJECTS
	for(int i = 0; i < 20; i++){
		for (int j = 0; j< 20; j++){
			ntVec3* v0 = new ntVec3(i*.01, j*.01, 0 );
			ntVertex* vert = new ntVertex(v0);
			Particle* p0 = new Particle(v0);
			ntAgent* a0 = new ntAgent(v0, Vec3(.2, .5, .1),.2);
			vert->setSize(2);
			verts.push_back(vert);
			particlesPtr00->push_back(p0);
			agentsPtr00->push_back(a0);
			a0->setVel(Vec3(.01, .02, .01));
		}
	}
	force00 = ntBasePhysics(ntVec3(0, 0, 0),.05);
	force00.setAttractor(ntVec3(0, 0, 0),.001,2);
	//force00.setGravity(.001);
	force00.setForce(ntVec3(1, 1, 1), .1);
	force00.setParticles(particlesPtr00);
	swarm00 = ntSwarm(agentsPtr00, .15, .25, .3);
}
void swarmApp::run(){
	//swarm00.run();
	force00.run();
	//std::cout << "run lola run\n";
	display();
}

void swarmApp::display(){
	///////////////////////////////////////////////////////////////
	for (int i = 0; i < verts.size(); i++){
		verts.at(i)->display();
	}
	///////////////////////////////////////////////////////////////
}
///////////////////////////////////////////////////footnote *01
//reference: http://www.opengl.org/discussion_boards/showthread.php/171184-GLM-to-create-gl_NormalMatrix
