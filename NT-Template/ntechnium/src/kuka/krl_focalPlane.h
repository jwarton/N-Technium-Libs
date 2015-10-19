///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////// krl_focalPlane.h
// openGl scratch libs							///////////////////
// Derived Content Class: KRL-src file test		///////////////////
// write krl for focal plane test on DMLS		///////////////////
// created by James Warton on 05/24/2014		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef APPLICATION_KRL_SRC_FOCAL_PLANE_JPW_NTECHNIUM
#define APPLICATION_KRL_SRC_FOCAL_PLANE_JPW_NTECHNIUM

#include <iostream>
#include "ntMath.h"
#include "ntMatrix4.h"
#include "nt_AppContent.h"

#include "nt_KRLprogram.h"

using namespace jpw;
class krlFPlane : public AppContent {
private:

public:
	//////////////////////////////VARIABLES UNIQUE TO DERIVED CLASS
	///////////////////////////////////////////////////////////////
	KRLprog program01;

	vector <ntVec3*> points;

	////////////////////////////////////////////////// CONSTRUCTORS
	//////////////////////////////// DEFAULT NOT NEEDED FOR DERIVED
	///////////////////////////////////////////////////////////////

	//////////////////////////////////////// REQUIRED CLASS METHODS
	///////////////////////////////////////////////////////////////
	void init();
	void run();
	/////////////////////////////////////////////////// CONVIENENCE
	///////////////////////////////////////////////////////////////
	void display();
};
#endif
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////