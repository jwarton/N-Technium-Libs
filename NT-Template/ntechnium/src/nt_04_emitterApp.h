///////////////////////////////////////////////////////////////////
/////////////////////////////////////////////// nt_04_emiitterApp.h
// openGl scratch libs							///////////////////
// Derived Content Class:  Emmitter Test		///////////////////
// created by James Warton on 05/02/2014		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef APPLICATION_EMITTER_TESTAPP_JPW_NTECHNIUM
#define APPLICATION_EMITTER_TESTAPP_JPW_NTECHNIUM

#include <iostream>
#include "ntMath.h"
#include "nt_AppContent.h"

#include "ntParticle.h"

using namespace jpw;
class emitterApp : public AppContent {
private:
	Vec3* posE;

	int W, D, H;
	int particleCnt;
	float dimR;
	float damping;

public:
	std::vector<Particle*>* particles = new std::vector<Particle*>;
	std::vector<ntVec3*>* pPos = new std::vector<ntVec3*>;
	std::vector<ntVec3*>* pVel = new std::vector<ntVec3*>;
	std::vector<ntColor4f>* pCol = new std::vector<ntColor4f>;

	emitterApp(int p, float dimR);
	
	void init();
	void run();

	void display();
};
#endif