///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////// nt_05_shapeApp.h
// openGl scratch libs							///////////////////
// Derived Content Class: Draw Sphere			///////////////////
// created by James Warton on 05/23/2014		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef APPLICATION_SHAPE_TESTAPP_JPW_NTECHNIUM
#define APPLICATION_SHAPE_TESTAPP_JPW_NTECHNIUM

#include <iostream>
#include "ntMath.h"
#include "ntMatrix4.h"
#include "nt_AppContent.h"

#include "ntPolyline.h"
#include "ntPolygon.h"
#include "ntCircle.h"
#include "ntRectangle.h"
#include "ntSquare.h"
#include "ntCube.h"
#include "ntTetra.h"
#include "ntSphere.h"
#include "ntOctahedron.h"


using namespace jpw;
class shapeApp : public AppContent {
private:

public:
	//////////////////////////////VARIABLES UNIQUE TO DERIVED CLASS
	///////////////////////////////////////////////////////////////
	/// ADD POLYHEDRON PRIMITIVE
	/// BASE CLASS TO MANAGE MESH DATA STRUCTURE
	/// UNIFIFIED INIT FUNCTION
	
	ntPolygon	triangle;
	ntSquare	square;
	ntRectangle rectangle;
	ntCircle	circle;

	ntTetra tetra_00;
	ntTetra tetra_01;
	ntTetra tetra_02;
	ntTetra tetra_03;

	ntCube cube_00;
	ntCube cube_01;
	ntCube cube_02;
	ntCube cube_03;

	ntOctahedron octo_00;
	ntOctahedron octo_01;
	ntOctahedron octo_02;
	ntOctahedron octo_03;

	ntSphere sphere_00;
	ntSphere sphere_01;
	ntSphere sphere_02;
	ntSphere sphere_03;

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