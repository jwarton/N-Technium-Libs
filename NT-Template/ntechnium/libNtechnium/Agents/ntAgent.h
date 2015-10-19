///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////// ntAgent.h
// openGl scratch libs							///////////////////
// Particle Based Agent class					///////////////////
// created by James Warton on 04/27/2014		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef PARTICLE_AGENT_JPW_NTECHNIUM
#define PARTICLE_AGENT_JPW_NTECHNIUM

#include "ntParticle.h"

//class ntAgent;
//typedef ntAgent Agent;

class ntAgent:public ntParticle{
private:


public:
	ntAgent* nearestNeighbor;
	std::vector<ntAgent*> neighbors;

	float vision;
	ntVec3 heading;

	ntAgent();
	ntAgent(ntVec3* pos);
	ntAgent(ntVec3* pos, ntVec3 heading);
	ntAgent(ntVec3* pos, ntVec3 heading, float vision);
	ntAgent(ntVec3* pos, ntVec3 heading, float s, float a, float c);

	vector<ntAgent*> getNeighbors();
	void addNeighbor(ntAgent* a);
		
	void displayVision();
	void displayHeading();
	void displayAgent();
};

inline vector<ntAgent*> ntAgent::getNeighbors(){
	return neighbors;
}
#endif
