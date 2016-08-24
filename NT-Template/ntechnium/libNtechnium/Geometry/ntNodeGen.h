///////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////// ntNodeGen.h
// openGl scratch libs							///////////////////
// Node Generator Class	                        ///////////////////
// created by James Warton on 08/18/2016		///////////////////
///////////////////////////////////////////////////////////////////
#ifndef NODEGEN_JPW_NTECHNIUM
#define NODEGEN_JPW_NTECHNIUM

#include <iostream>
#include <vector>
#include <array>

#include "ntVec3.h"
#include "ntMatrix4.h"
#include "ntVertex.h"
#include "ntEdge.h"
#include "ntPolyline.h"
#include "ntPolygon.h"
#include "ntRectangle.h"
#include "ntSquare.h"
#include "ntNormal.h"
#include "ntFace3.h"
#include "ntConvexHull.h"
#include "ntMath.h"
using namespace jpw;

enum SectMode {SQUARE, POLYGON, POLYPARAM, RECTANGLE};
class ntNodeGen{
private:

	/// REFACTOR TO SEPARATE NODE OBJECTS
	//LOCAL PROPERTIES---VARIABLE FOR EACH NODE
	double dim00 = 0.0;					// PROFILE X
	double dim01 = 0.0;					// PROFILE Y

										//NODE OBJECTS
										//ntVec3 *v0;					//NODE JUNCTURE	
										//std::vector<ntVec3 *> vecs;	//SPOKE END POINTS v1
										//std::vector<ntEdge*> edges;	//NODE SPOKES
	ntConvexHull node;
	/// ^^^^^^^^^^^^^^^
	//GLOBAL PROPERTIES---TEMPORARY
	double dimX		= 0.02;				// PROFILE X
	double dimY		= 0.035;			// PROFILE Y
	double dimZ;						// PROFILE Z
	double p		= 0.015;			// PROFILE AS RATIO OF BRANCH LENGTH

	int sides		= 6;				// NUMBER OF PROFILE SEGMENTS 
	int div			= 5;				// REPLACE WITH SET FUNCTION
	double neck		= 0.1;				// EDGE PARAMETER AT BASE OF NODE
	double t_max	= 0.45;				// MAX NECK PARAMERTER
	SectMode mode	= POLYPARAM;		// DEFAULT PROFILE MODE

	bool isUniform	= false;

	std::vector<ntVec3 *>		vecs;
	std::vector<ntVertex*>		verts;
	std::vector<ntEdge*>		edges;
	std::vector<ntFace3*>		faces;
	std::vector<ntPolyline*>	profiles;

	ntPolygon profile;	///ONLY USED WHEN ALL PROFILES ARE UNIFORM
						///OTHER WISE REPLACED WITH LAST PROFILE

	ntColor4f col = ntColor4f(0.25f,0.25f,0.25f,1);
	std::vector<ntCol4> cols;

	// PROFILE TYPES:  SQUARE, POLYGON, POLYPARM, RECTANGLE
	void gen_profile();
	std::vector <ntVec3*> gen_profile(float w);

	void gen_branch();
	void gen_node();

	// 
	void gen_convex();

	// PROPERTY MANAGEMENT
	void calc_thickness();

public:

	ntNodeGen();
	ntNodeGen(std::vector<ntEdge*>* edges);
	ntNodeGen(std::vector<ntEdge*> edges);

	void init();

	void set_Color(ntColor4f col);
	void set_Parameters(SectMode mode, int div, double neck);
	void set_Parameters();
	void set_Profile(ntPolygon profile);
	void display();
	void display_Verts();
	void display_Edges();
};
#endif
