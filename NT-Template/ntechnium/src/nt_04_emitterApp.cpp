#include "nt_04_emitterApp.h"
#include <iostream>


emitterApp::emitterApp(int p, float dimR) :
particleCnt(p), dimR(dimR){
	damping = .8;		///explicit overwrite for testing

	posE = new Vec3(0, 0, 0);

	W = 2;
	D = 2;
	H = 2;
	//init();
}

void emitterApp::init(){
	glRotatef(-45, 1.f, 0.f, 0.f);

	int pCnt = particleCnt;  
	float radius;							//drives initial magnitude

	//float rMax = 0.980;
	//float gMax = 0.470;
	//float bMax = 0.314;

	//float rMin = 0.745;
	//float gMin = 0.157;
	//float bMin = 0.078;

	float rMax = .2;
	float gMax = 0.7;
	float bMax = 0.7;

	float rMin = .1;
	float gMin = .6;
	float bMin = .4;


	float angle;
	float theta;
	float phi;

	float velX;
	float velY;
	float velZ;
	Col4f col;

	///////////////////////////////////////////////////////////////////
	/////////////////////////////////////////// CALCULATE INITIAL COLOR
	for (int i = 0; i<pCnt; i++) {
		float r = mapRange(rMin, rMax, 0, pCnt, i);
		float g = mapRange(gMin, gMax, 0, pCnt, i);
		float b = mapRange(bMin, bMax, 0, pCnt, i);
		float a = 1;
		
		float pRange = pCnt - (pCnt / 5);

		if (i < pRange){
			col = Col4(r, g, b, a);
		}
		if (i > pRange){
			col = Col4(.8, 0.7, 0.1, 1.0f);
			/*col = Col4(0.882, 0.886, 0.705, 1.0f);*/
		}
		///////////////////////////////////////////////////////////////
		//////////////////////////////////// CALCULATE INITIAL VELOCITY
		int g1 = pCnt * .2;
		int g2 = pCnt * .4;
		int g3 = pCnt * .5;
		int g4 = pCnt * .6;
		int g5 = pCnt * .8;

		if (i < g1){
			float min = -0.1f;
			float max =  0.1f;
			radius = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (max - min));
		}
		if ((i > g1) & (i < g2)){
			float min = -.02f;
			float max =  .02f;
			radius = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (max - min));
		}
		if ((i > g2) & (i < g3)){
			radius = .001;
		}
		if ((i > g3) & (i < g4)){
			radius = .16;
		}
		if ((i > g4) & (i < g5)){
			float min = -1.0f;
			float max = -.1f;
			radius = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (max - min)); 
		}
		if (i > g5){
			radius = 1.2;
		}
		///////////////////////////////////////////////////////////////////
		///////////////////////////////////// SET INITIAL PARTICLE VELOCITY
		angle = rand() % 360;
		theta = rand() % 360;
		phi = rand() % 180;

		///////////////////////////////////////////////// CIRCULAR EMMITTER
		velX = ((radius)*(cos(angle)));
		velY = ((radius)*(sin(angle)));
		velZ = (0);

		///////////////////////////////////////////////// SHERICAL EMMITTER
		velX = (cos(theta) * sin(phi) * radius);
		velY = (sin(theta) * sin(phi) * radius);
		velZ = (cos(phi) * radius);
		

		Vec3* pos = new Vec3(0, 0, 0);
		Vec3* vel = new Vec3(velX, velY, velZ);
		vel->mult(.001); //initial velocity adjusted to relative scene scale

		pPos->push_back(pos);
		pVel->push_back(vel);
		particles->push_back(new ntParticle(pos, vel, col));
	}
}

void emitterApp::run(){
	///////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////// ROTATE
	glRotatef(.2, 0.f, 0.f, 1.f);

	for (int i = 0; i < particles->size(); ++i) {
		///////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////// GRAVITY
		//particles->at(i)->velocity->z -= .0000001;
		//particles->at(i)->velocity->y -= .00001;
		///////////////////////////////////////////////////////////////////
		////////////////////////////////////////// STORE TEMPORARY VELOCITY
		Vec3* vel = new Vec3 (
			particles->at(i)->velocity->x,
			particles->at(i)->velocity->y,
			particles->at(i)->velocity->z);

		///////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////// ATTRACTOR RANGE
		float range = dimR + (dimR*.25); 
		float rangSq = range * range;
		vel->mult(.05);	//attraction strength

		///////////////////////////////////////////////////////////////////
		//////////////////////////////////// APPLY ATTRACTION TOWARD CENTER
		float distSq = particles->at(i)->pos->distSqrd(posE);
		if (distSq >= rangSq){
			particles->at(i)->velocity->sub(vel); ///decelerate particles exceeding range
		}
		else {
			particles->at(i)->velocity->add(vel); ///accelerate particle within range
		}
		///////////////////////////////////////////////////////////////////
		////////////////////////////////////// REACT TO BOUNDARY CONDITIONS
		if ((particles->at(i)->pos->x >= W / 2) || (particles->at(i)->pos->x <= -W / 2)){
			particles->at(i)->velocity->x = -particles->at(i)->velocity->x;
			particles->at(i)->velocity->x *= damping;
		}
		if ((particles->at(i)->pos->y >= D / 2) || (particles->at(i)->pos->y <= -D / 2)){
			particles->at(i)->velocity->y = -particles->at(i)->velocity->y;
			particles->at(i)->velocity->y *= damping;
		}
		if ((particles->at(i)->pos->z >= H / 2) || (particles->at(i)->pos->z <= -H / 2)){
			particles->at(i)->velocity->z = -particles->at(i)->velocity->z;
			particles->at(i)->velocity->z *= damping;
		}
		///////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////// APPLY FORCE
		particles->at(i)->pos->add(particles->at(i)->velocity);

		delete vel;
		///////////////// EMITTER OF LOST SOULS, THE 500,000,000 YEAR REIGN
		if ((particles->at(i)->pos->x > W) || (particles->at(i)->pos->x < -W) || (particles->at(i)->pos->y < -D) || (particles->at(i)->pos->y > D)){
			///TODO
			//reset position near center 
			float x = rand() % 1;
			float y = rand() % 1;
			float z = rand() % 1;
			particles->at(i)->pos->set(new Vec3(x,y,z));
			//reset initial velocity
			float min = -.02f;
			float max =  .02f;
			float magnitude = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (max - min));
			magnitude *= .0001;
		
			//reset color
			particles->at(i)->col = Col4( 1, .1, .3, 1);
			///
		}
	}
	display();
}

void emitterApp::display(){
	for (int i = 0; i<particles->size(); ++i){
		particles->at(i)->display();
	}
}
