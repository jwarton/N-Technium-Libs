///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// ntSpring.h
// openGl scratch libs							///////////////////
// Spring Class									///////////////////
// created by James Warton on 04/18/2014		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef SPRINGS_JPW_NTECHNIUM
#define SPRINGS_JPW_NTECHNIUM

#include <iostream>
#include <vector>
#include "ntBaseShape.h"
#include "ntParticle.h"

class ntSpring;
typedef ntSpring Spring;

class ntSpring{
private:
	float restLength;		// defaults to .75 length
	float springConstant;	// spring constant | set relative to environmental forces
	float damping;			// 0-1 | 1 = critical damping yielding 0 bounce from 1 to r1
							// values < 0 create instability and take longer to balance
							// values > 1 takes longer to reach rest length
	
	ntParticle p0, p1;
	bool enabled;

	void init();
	void setP0(ntParticle p);
	void setP1(ntParticle p);

public:
	ntSpring();
	ntSpring(Particle p0, Particle p1);
	//ntSpring(Particle* p0, Particle* p1);
	ntSpring(Particle p0, Particle p1, float ks, float d, float r);


	void enable();
	void disable();
	bool isEnabled();

	Particle getP0();
	Particle getP1();

	float getLength();
	float getRestLength();
	void setRestLength(float rl);

	float getStrength();
	void setStrength(float ks);

	float getDamping();
	void setDamping(float d);

	void apply();
};
#endif

//http://murderandcreate.com/physics/