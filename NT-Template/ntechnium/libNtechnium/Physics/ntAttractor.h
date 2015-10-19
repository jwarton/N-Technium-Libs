///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// ntSpring.h
// openGl scratch libs							///////////////////
// Spring Class									///////////////////
// created by James Warton on 04/18/2014		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef ATTRACTORS_JPW_NTECHNIUM
#define ATTRACTORS_JPW_NTECHNIUM

#include <iostream>
#include <vector>
#include "ntVec3.h"
#include "ntParticle.h"

class Attractor{
private:
	float k;		//attractor strength
	float distMin;	//attractor range
	float distMinSquared;

	Vec3 pos;
	Particle p0;
	Particle p1;

	bool enabled = true;

public:
	Attractor();
	Attractor(Vec3 pos, float k, float distMin);
	Attractor(Particle p0, Vec3 pos, float k, float distMin);
	Attractor(Particle p0, Particle p1, float k, float distMin);

	void enable();
	void disable();
	void apply();

	float getDistMin();
	float getStrength();

	void setP0(Particle* p);

	void setDistMin(float d);
	void setStrength(float k);

	bool isEnabled();
	bool isDisabled();

	void displayRange();	//display range as sphere
	void displayDirection();			//display tractor beam as line;
};
#endif