///////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////// ntBasePhysics.h
// openGl scratch libs							///////////////////
// Particle Physics Engine						///////////////////
// created by James Warton on 04/17/2014		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef BASEPHYSICS_JPW_NTECHNIUM
#define BASEPHYSICS_JPW_NTECHNIUM

#include <iostream>
#include <vector>
#include "ntBaseShape.h"
#include "ntAttractor.h"
#include "ntParticle.h"
#include "ntSpring.h"
#include "ntMatrix4.h"

class ntBasePhysics;
typedef ntBasePhysics ntPhysics;

class ntBasePhysics {
private:
	int tick;
	void init();

	Vec3 force;		//
	Vec3 gravity;	//0-5 recommended

	float drag;		//0.001 default | range 0-1 | high values simulate drag in liquids

protected:
	std::vector <Vec3*>* vecsPtr = new vector<Vec3*>;
	std::vector <ntEdge*>* edgePtrs = new vector<ntEdge*>;
	std::vector <Particle*>* particles = new vector<Particle*>;
	std::vector <Spring*>* springs = new vector<Spring*>;
	std::vector <Attractor*>* attractors = new vector<Attractor*>;
	//std::vector <Force*>* forces = new vector<Force*>;

public:
	ntBasePhysics();
	ntBasePhysics(ntVec3 force, float drag);

	ntBasePhysics(std::vector<ntVec3*>* vecsPtr);
	ntBasePhysics(std::vector<ntVec3*>* vecsPtr, ntVec3 force);
	ntBasePhysics(std::vector<ntVec3*>* vecsPtr, ntVec3 force, float drag);
	ntBasePhysics(std::vector<ntEdge*>* edgePtrs, float ks, float d, ntVec3 force);
	
	//set particles and springs from physics enabled geom
	void setParticles(std::vector<Particle*>* particles);
	void setSprings(std::vector <Spring*>* springs);

	//add new particles to engine
	void addParticles(std::vector<ntVec3*>* vecsPtr);
	void addParticle(Vec3* pos);

	//add new springs forces require paired particle indices ie. edge v0-v1
	void addSprings(std::vector<ntEdge*>* edgePtrs, float ks, float d);
	void addSpring(Vec3* v0, Vec3* v1, float ks, float d, float r);

	//these effect all particles equally
	void addAttractor(Attractor* a);
	void addRepeller(Attractor* r);
	void setAttractor(Vec3 pos, float k,float minDist);
	void setRepeller(Vec3 pos, float k, float maxDist);

	void setGravity(float magnitude);
	void setForce(Vec3 dir0, float magnitude);
	void setDrag(float drag);

	void applyForces();
	void clearForces();

	void eulerIntegration();
	void modEulerIntegration();
	void rungeKuttaIntegration();

	void run();
};
#endif

//http://murderandcreate.com/physics/