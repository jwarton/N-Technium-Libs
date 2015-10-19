///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////// ntSwarm.h
// openGl scratch libs							///////////////////
// Swarm Engine									///////////////////
// created by James Warton on 04/26/2014		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef SWARM_BEHAVIOR_JPW_NTECHNIUM
#define SWARM_BEHAVIOR_JPW_NTECHNIUM

#include <iostream>
#include <vector>
#include "ntAgent.h"
#include "ntMatrix4.h"

class ntSwarm{
private:
	int runCylce;
	float zoneR; 
	float zoneS;
	float zoneA;
	float zoneC;

	float zoneSqrdR;
	float zoneSqrdS;
	float zoneSqrdA;
	float zoneSqrdC;

	//Physiomimetics-Physics Based Swarm Intelligence
	//review page 58 for initial parameters:
	float turnMaxS;
	float turnMaxA;
	float turnMaxC;

	int totalAgents;

protected:
	std::vector <ntAgent*>* agents = new vector<ntAgent*>();

public:
	ntSwarm();
	ntSwarm(std::vector<ntAgent*>* agents);
	ntSwarm(std::vector<ntAgent*>* agents, float zoneS, float zoneA, float zoneC);
	ntSwarm(std::vector<ntAgent*>* agents, float s, float a, float c, float turnMaxA, float turnMaxC, float turnMaxS);

	void setAgents(std::vector<ntAgent*>* agents);
	void addAgents(Vec3 pos, ntVec3 heading, float vision, int num);
	void clearAgents();

	void detectNeighbors(ntAgent* a);
	void applyAlignment(ntAgent* a);
	void applyCohesion(ntAgent* a);
	void applySeparation(ntAgent* a);

	void setAlignmentTurnMax(float a);
	void setCohesionTurnMax(float c);
	void setSeparationTurnMax(float s);
	void setSeparationMin(float s);

	void init();
	void run();
	void monitor();
};
#endif

//Physicomimetics: Physics-Based Swarm Intelligence
//William M.Spears, Diana F.Spears

//http://libcinder.org/docs/dev/flocking_chapter1.html