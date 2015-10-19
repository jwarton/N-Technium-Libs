///////////////////////////////////////////////////////////////////
/////////////////////////////////////////////// nt_02_treeApp-v02.h
// openGl scratch libs							///////////////////
// Derived Content Class:  	Tree w/ Physics		///////////////////
// created by James Warton on 10/18/2014		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef APPLICATION_TREE_v02_JPW_NTECHNIUM
#define APPLICATION_TREE_v02_JPW_NTECHNIUM

#include <iostream>
#include "ntMath.h"
#include "nt_AppContent.h"

#include "ntSphere.h"
#include "ntMeshPts.h"
#include "ntTree.h"
#include "ntPtsTxt.h"
#include "ntBasePhysics.h"
#include "ntSwarm.h"

using namespace jpw;
class treeApp02 : public AppContent {
private:

public:
	//////////////////////////////VARIABLES UNIQUE TO DERIVED CLASS
	///////////////////////////////////////////////////////////////
	ntTree tree00;
	ntTree tree01;
	ntBasePhysics force00;
	ntBasePhysics force01;

	std::vector<ntTree*> trees;
	vector <ntVertex*> verts;

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