///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////// krl_rhinoTemp.h
// openGl scratch libs							///////////////////
// Derived Content Class: KRL-src file test		///////////////////
// write krl for rhino templates on DMLS		///////////////////
// created by James Warton on 08/28/2014		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef APPLICATION_KRL_TEXT_FILE_TEMPLATE_JPW_NTECHNIUM
#define APPLICATION_KRL_TEXT_FILE_TEMPLATE_JPW_NTECHNIUM

#include <iostream>
//#include <fstream>
#include "ntMath.h"
#include "ntMatrix4.h"
#include "nt_AppContent.h"

#include "nt_KRLprogram.h"

using namespace jpw;
class krlMatLab : public AppContent {
private:

public:
	//////////////////////////////VARIABLES UNIQUE TO DERIVED CLASS
	///////////////////////////////////////////////////////////////
	KRLprog program01;

	std::vector <ntVec3 *> vecs;
	std::vector <ntVertex *> verts;

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

	string format(string line);
	void addPt(Vec3 * pos);
};
#endif
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////