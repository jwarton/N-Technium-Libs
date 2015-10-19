///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////// ntParticle.h
// openGl scratch libs							///////////////////
// Particle Class								///////////////////
// created by James Warton on 04/18/2014		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef PARTICLES_JPW_NTECHNIUM
#define PARTICLES_JPW_NTECHNIUM

#include <iostream>
#include <vector>
#include "ntVec3.h"
#include "ntColor4f.h"
#include "ntVertex.h"
//#include "ntNode.h"

///////////////////////////////// FORWARD DECLARE
/////////////////////////////// FOR FRIEND ACCESS
class ntTree;
class ntPlane;
class ntBasePhysics;  
////////////////// ?? REFACTOR ACCESS W/O FREINDS

class ntParticle;
typedef ntParticle Particle;

///REFACTOR TO DERIVE FROM NODE CLASS
///REMOVE COMMON MEMBERS BETWEEN NODE CLASSES
class ntParticle : public ntVertex{
//class ntParticle : public ntVertex{
private:
	void init();
	void reset();	//SHARED BY NODE W/ ADDED MEMBER ACCESS

protected:
	float mass;
	bool dead;
	bool show;
	int age;

	bool fixed;		//SHARED BY NODE
	bool fixed_X;	//SHARED BY NODE
	bool fixed_Y;	//SHARED BY NODE
	bool fixed_Z;	//SHARED BY NODE
	
public:
	friend class ntTree;
	friend class ntPlane;
	friend class ntBasePhysics;

	ntVec3* velocity;	///heading?
	ntVec3* force;		

	ntParticle();
	ntParticle(ntVec3* pos);
	ntParticle(ntVec3* pos, ntVec3* vel);
	ntParticle(ntVec3* pos, ntVec3* vel, Col4 col);

	ntVec3* getPos();
	ntVec3* getVel();
	ntVec3* getForce();

	void setVel(ntVec3 v);

	float getMass();
	bool isFree();			//ADD TO NODE
	bool isFixed();			//ADD TO NODE
	bool isDead();
	int getAge();

	float distanceTo(ntParticle p);
	//void display();			//SHARED BY VERTEX
};
#endif

//http://murderandcreate.com/physics/