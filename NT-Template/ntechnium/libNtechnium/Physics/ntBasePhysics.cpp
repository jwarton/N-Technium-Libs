
#include "ntBasePhysics.h"

ntBasePhysics::ntBasePhysics(){}

ntBasePhysics::ntBasePhysics(ntVec3 force, float drag):
force(force),drag(drag){
	gravity = Vec3(0, 0, 0);
	init();
}

ntBasePhysics::ntBasePhysics(std::vector<ntVec3*>* vecsPtr):
vecsPtr(vecsPtr){
	gravity = Vec3(0, 0, 0);
	drag = 0.001;
	addParticles(vecsPtr);
	init();
}

ntBasePhysics::ntBasePhysics(std::vector<ntVec3*>* vecsPtr, ntVec3 gravity):
vecsPtr(vecsPtr), gravity(gravity){
	drag = 0.001;
	addParticles(vecsPtr);
	init();
}

ntBasePhysics::ntBasePhysics(std::vector<ntVec3*>* vecsPtr, ntVec3 force, float drag):
vecsPtr(vecsPtr), force(force), drag(drag){
	gravity = Vec3(0, 0, 0);
	addParticles(vecsPtr);
	init();
}

ntBasePhysics::ntBasePhysics(std::vector<ntEdge*>* edgePtrs, float ks, float d, ntVec3 force):
edgePtrs(edgePtrs), force(force){
	drag = 0.001;
	gravity = Vec3(0, 0, 0);
	addSprings(edgePtrs, ks, d);
	init();
}

void ntBasePhysics::init(){	
	tick = 1;
}

void ntBasePhysics::run(){
	if (particles->size() < 1000){
		rungeKuttaIntegration();
	} else {
		rungeKuttaIntegration();
		//modEulerIntegration();	///NEEDS WORK
		//eulerIntegration();		///ALSO NEEDS WORK
	}
}
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
void ntBasePhysics::setParticles(std::vector<Particle*>* particles){
	this->particles = particles;
}
void ntBasePhysics::setSprings(std::vector <Spring*>* springs){
	this->springs = springs;
}
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
void ntBasePhysics::addParticles(std::vector<ntVec3*>* vecsPtr){
	///requires exception based on constructor usage
	///this->vecsPtr = vecsPtr;  
	for (int i = 0; i < vecsPtr->size(); i++){
		addParticle(vecsPtr->at(i));
	}
}
void ntBasePhysics::addParticle(Vec3* pos){
	Particle* p = new Particle(pos);
	particles->push_back(p);
}
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
void ntBasePhysics::addSprings(std::vector<ntEdge*>* edgePtrs, float ks, float d){
	///requires exception based on constructor usage
	///this->edgePtrs = edgePtrs;

	///function creates duplicate particles at common end points
	///vec3* may be moved twice/ cycle durring integration
	///may be problematic when fixed/free field is inconsistent
	for (int i = 0; i < edgePtrs->size(); i++){
		Vec3* v0 = edgePtrs->at(i)->v0;
		Vec3* v1 = edgePtrs->at(i)->v1;
		float r = edgePtrs->at(i)->getLength();
		addSpring(v0, v1, ks, d, r);
	}
}
void ntBasePhysics::addSpring(Vec3* v0, Vec3* v1, float ks, float d, float r){
	Particle* p0 = new ntParticle(v0);
	particles->push_back(p0);
	Particle* p1 = new ntParticle(v1);
	particles->push_back(p1);
	Spring* spring = new ntSpring(*p0, *p1, ks, d, r);
	springs->push_back(spring);
}
///////////////////////////////////////////////////////////////////
//////////////////////////// APPLIED TO ALL PARTICLES IN COLLECTION
void ntBasePhysics::setAttractor(Vec3 pos, float k, float minDist){
	Attractor* a0 = new Attractor(pos, k, minDist);
	attractors->push_back(a0);
}
void ntBasePhysics::setRepeller(Vec3 pos, float k, float maxDist){
	///TODO
}
void ntBasePhysics::addAttractor(Attractor* a){
	attractors->push_back(a);
}
void ntBasePhysics::addRepeller(Attractor* r){
	attractors->push_back(r);
}

void ntBasePhysics::setGravity(float magnitude){
	gravity.z = magnitude;
}
void ntBasePhysics::setForce(Vec3 dir0, float magnitude){
	///REPLACE WITH ACCESS TO MULTIPLE FORCES
	///private variable force can only allow
	///application of single force
	force.x = dir0.x;
	force.y = dir0.y;
	force.z = dir0.z;
	force.unitize();
	force.mult(magnitude);
}
void ntBasePhysics::setDrag(float drag){
	this->drag = drag;
}
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
void ntBasePhysics::eulerIntegration(){
	int t = tick;
	clearForces();
	applyForces();

	for (int i = 0; i < particles->size(); i++){
		if (particles->at(i)->isFree()){
			ntVec3 deltaV = ntVec3(
				particles->at(i)->getForce()->x / (particles->at(i)->getMass() * t),
				particles->at(i)->getForce()->y / (particles->at(i)->getMass() * t),
				particles->at(i)->getForce()->z / (particles->at(i)->getMass() * t));

			particles->at(i)->getVel()->add(&deltaV);

			ntVec3 deltaD = ntVec3(
				particles->at(i)->getVel()->x / t,
				particles->at(i)->getVel()->y / t,
				particles->at(i)->getVel()->z / t);

			particles->at(i)->getPos()->add(&deltaD);
		}
	}
}
void ntBasePhysics::modEulerIntegration(){
	int t = tick;
	clearForces();
	applyForces();

	float halftt = .5f*t*t;

	for (int i = 0; i < particles->size(); i++){
		if (particles->at(i)->isFree()){
			
			float fX = particles->at(i)->getForce()->x / (particles->at(i)->getMass());
			float fY = particles->at(i)->getForce()->y / (particles->at(i)->getMass());
			float fZ = particles->at(i)->getForce()->z / (particles->at(i)->getMass());
			
			float vX = particles->at(i)->getVel()->x / t;
			float vY = particles->at(i)->getVel()->y / t;
			float vZ = particles->at(i)->getVel()->z / t;

			particles->at(i)->getPos()->add(&Vec3(vX, vY, vZ));
			particles->at(i)->getPos()->add(&Vec3(fX * halftt, fY * halftt, fZ * halftt));
			particles->at(i)->getVel()->add(&Vec3(fX/t, fY/t, fZ/t));
		}
	}
}
void ntBasePhysics::rungeKuttaIntegration(){
	int deltaT = tick;

	int size = particles->size();
	std::vector <Vec3> pos (size);
	std::vector <Vec3> vel (size);
	std::vector <Vec3> k1F (size);
	std::vector <Vec3> k1V (size);
	std::vector <Vec3> k2F (size);
	std::vector <Vec3> k2V (size);
	std::vector <Vec3> k3F (size);
	std::vector <Vec3> k3V (size);
	std::vector <Vec3> k4F (size);
	std::vector <Vec3> k4V (size);

	///////////////////////////////////////////////////////////////
	//////////////////////////// SAVE ORGINAL POSITION AND VELOCITY
	for (int i = 0; i < particles->size(); i++){
		if (particles->at(i)->isFree()){
			Vec3 pos0 = Vec3(0, 0, 0);
			pos0.set(particles->at(i)->getPos());
			pos.at(i) = pos0;
			Vec3 vel0 = Vec3(0, 0, 0);
			vel0.set(particles->at(i)->getVel());
			vel.at(i) = vel0;
		}
		particles->at(i)->getForce()->clear();
	}
	applyForces();
	///////////////////////////////////////////////////////////////
	//////////////////////////////////////////////// SAVE K1 VALUES
	for (int i = 0; i < particles->size(); i++){
		if (particles->at(i)->isFree()){
			Vec3 kF = Vec3(0, 0, 0);
			kF.set(particles->at(i)->getForce());
			k1F.at(i) = kF;
			Vec3 kV = Vec3(0, 0, 0);
			kV.set(particles->at(i)->getVel());
			k1V.at(i) = kV;
		}
		particles->at(i)->getForce()->clear();
	}
	///////////////////////////////////////////////////////////////
	///////////////////////////////////////////////// GET K2 VALUES
	for (int i = 0; i < particles->size(); i++){
		if (particles->at(i)->isFree()){

			particles->at(i)->getPos()->x = pos.at(i).x + k1V.at(i).x * 0.5f * deltaT;
			particles->at(i)->getPos()->y = pos.at(i).y + k1V.at(i).y * 0.5f * deltaT;
			particles->at(i)->getPos()->z = pos.at(i).z + k1V.at(i).z * 0.5f * deltaT;

			particles->at(i)->velocity->x = vel.at(i).x + k1F.at(i).x * 0.5f * deltaT / particles->at(i)->getMass();
			particles->at(i)->velocity->y = vel.at(i).y + k1F.at(i).y * 0.5f * deltaT / particles->at(i)->getMass();
			particles->at(i)->velocity->z = vel.at(i).z + k1F.at(i).z * 0.5f * deltaT / particles->at(i)->getMass();
		}
	}
	applyForces();
	///////////////////////////////////////////////////////////////
	//////////////////////////////////////////////// SAVE K2 VALUES
	for (int i = 0; i < particles->size(); i++){
		if (particles->at(i)->isFree()){
			Vec3 kF = Vec3(0, 0, 0);
			kF.set(particles->at(i)->getForce());
			k2F.at(i) = kF;
			Vec3 kV = Vec3(0, 0, 0);
			kV.set(particles->at(i)->getVel());
			k2V.at(i) = kV;
		}
		particles->at(i)->getForce()->clear();
	}
	///////////////////////////////////////////////////////////////
	///////////////////////////////////////////////// GET K3 VALUES
	for (int i = 0; i < particles->size(); i++){
		if (particles->at(i)->isFree()){

			particles->at(i)->getPos()->x = pos.at(i).x + k2V.at(i).x * 0.5f * deltaT;
			particles->at(i)->getPos()->y = pos.at(i).y + k2V.at(i).y * 0.5f * deltaT;
			particles->at(i)->getPos()->z = pos.at(i).z + k2V.at(i).z * 0.5f * deltaT;

			particles->at(i)->velocity->x = vel.at(i).x + k2F.at(i).x * 0.5f * deltaT / particles->at(i)->getMass();
			particles->at(i)->velocity->y = vel.at(i).y + k2F.at(i).y * 0.5f * deltaT / particles->at(i)->getMass();
			particles->at(i)->velocity->z = vel.at(i).z + k2F.at(i).z * 0.5f * deltaT / particles->at(i)->getMass();
		}
	}
	applyForces();
	///////////////////////////////////////////////////////////////
	//////////////////////////////////////////////// SAVE K3 VALUES
	for (int i = 0; i < particles->size(); i++){
		if (particles->at(i)->isFree()){
			Vec3 kF = Vec3(0, 0, 0);
			kF.set(particles->at(i)->getForce());
			k3F.at(i) = kF;
			Vec3 kV = Vec3(0, 0, 0);
			kV.set(particles->at(i)->getVel());
			k3V.at(i) = kV;
		}
		particles->at(i)->getForce()->clear();
	}
	///////////////////////////////////////////////////////////////
	///////////////////////////////////////////////// GET K4 VALUES
	for (int i = 0; i < particles->size(); i++){
		if (particles->at(i)->isFree()){

			particles->at(i)->getPos()->x = pos.at(i).x + k3V.at(i).x * 0.5f * deltaT;
			particles->at(i)->getPos()->y = pos.at(i).y + k3V.at(i).y * 0.5f * deltaT;
			particles->at(i)->getPos()->z = pos.at(i).z + k3V.at(i).z * 0.5f * deltaT;

			particles->at(i)->velocity->x = vel.at(i).x + k3F.at(i).x * 0.5f * deltaT / particles->at(i)->getMass();
			particles->at(i)->velocity->y = vel.at(i).y + k3F.at(i).y * 0.5f * deltaT / particles->at(i)->getMass();
			particles->at(i)->velocity->z = vel.at(i).z + k3F.at(i).z * 0.5f * deltaT / particles->at(i)->getMass();
		}
	}
	applyForces();
	///////////////////////////////////////////////////////////////
	//////////////////////////////////////////////// SAVE K4 VALUES
	for (int i = 0; i < particles->size(); i++){
		if (particles->at(i)->isFree()){
			Vec3 kF = Vec3(0, 0, 0);
			kF.set(particles->at(i)->getForce());
			k4F.at(i) = kF;
			Vec3 kV = Vec3(0, 0, 0);
			kV.set(particles->at(i)->getVel());
			k4V.at(i) = kV;
		}
		particles->at(i)->getForce()->clear();
	}
	///////////////////////////////////////////////////////////////
	////////////////////////////////////////// COMBINE VALUES K1-K4
	for (int i = 0; i < particles->size(); i++){
		particles->at(i)->age += deltaT;
		if (particles->at(i)->isFree()){
			//update position
			particles->at(i)->getPos()->x = pos.at(i).x + deltaT / 6.0f * (k1V.at(i).x + 2.0f * k2V.at(i).x + 2.0f * k3V.at(i).x + k4F.at(i).x);
			particles->at(i)->getPos()->y = pos.at(i).y + deltaT / 6.0f * (k1V.at(i).y + 2.0f * k2V.at(i).y + 2.0f * k3V.at(i).y + k4F.at(i).y);
			particles->at(i)->getPos()->z = pos.at(i).z + deltaT / 6.0f * (k1V.at(i).z + 2.0f * k2V.at(i).z + 2.0f * k3V.at(i).z + k4F.at(i).z);
			//update velocity
			particles->at(i)->velocity->x = vel.at(i).x + deltaT / (6.0f * particles->at(i)->getMass()) * (k1F.at(i).x + 2.0f * k2F.at(i).x + 2.0f * k3F.at(i).x + k4F.at(i).x);
			particles->at(i)->velocity->y = vel.at(i).y + deltaT / (6.0f * particles->at(i)->getMass()) * (k1F.at(i).y + 2.0f * k2F.at(i).y + 2.0f * k3F.at(i).y + k4F.at(i).y);
			particles->at(i)->velocity->z = vel.at(i).z + deltaT / (6.0f * particles->at(i)->getMass()) * (k1F.at(i).z + 2.0f * k2F.at(i).z + 2.0f * k3F.at(i).z + k4F.at(i).z);
		}
	}
}

void ntBasePhysics::applyForces(){
	///////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////// GRAVITY
	for (int i = 0; i < particles->size(); i++){
		particles->at(i)->getForce()->add(&gravity);
	}
	///////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////// VEC FORCE
	for (int i = 0; i < particles->size(); i++){
		ntVec3 pVel = ntVec3(
			particles->at(i)->velocity->x * -drag,
			particles->at(i)->velocity->y * -drag,
			particles->at(i)->velocity->z * -drag);
			///velocity needs to be initialized during 
			///as part of set/apply forces
		//std::cout << "line 338 | ntBasePhysics.cpp:  " << pVel.x << endl; ///retrns -0  ????
		particles->at(i)->getForce()->add(&pVel);
	}
	///////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////// SPRING
	for (int i = 0; i < springs->size(); i++){
		springs->at(i)->apply();
	}
	///////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////// ATTRACTORS
	for (int i = 1; i < attractors->size(); i++){
		Attractor* attractor = attractors->at(i);
		for (int j = 0; j < particles->size(); j++){
			attractor->setP0(particles->at(j));
			attractors->at(i)->apply();
		}
	}
}
void ntBasePhysics::clearForces(){
	for (int i = 0; i < particles->size(); i++){
		particles->at(i)->getForce()->clear();
	}
}
//Runge - Kutta takes about 4 times as many cycles as Modified Euler.
//In return the system gets very stable.If you plan to have have over 1000 particles
//interacting with each other try Modified Euler.If you will have fewer particles and 
//want tight springs and quicker responses use Runge - Kutta.
//Stick with the default and if things get slow try switching to Euler.
//You may have to add more drag or reduce the strength of springs and attractions to keep it stable.