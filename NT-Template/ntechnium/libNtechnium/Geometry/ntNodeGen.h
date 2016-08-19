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
#include "ntNormal.h"
#include "ntFace3.h"
#include "ntMath.h"
using namespace jpw;

enum SectMode {SQUARE, POLYGON, POLYPARAM, RECTANGLE};
class ntNodeGen{
private:

	int div = 5;					/// REPLACE WITH SET FUNCTION
	float neck = 0.1;				/// EDGE PARAMETER AT BASE OF 
	SectMode mode = POLYPARAM;		/// ADD SET FUNCTION

	//NODE OBJECTS
	//ntVec3 *v0;					//NODE JUNCTURE	
	//std::vector<ntVec3 *> vecs;	//SPOKE END POINTS v1
	//std::vector<ntEdge*> edges;	//NODE SPOKES

	std::vector<ntVec3 *>		vecs;
	std::vector<ntVertex*>		verts;
	std::vector<ntEdge*>		edges;
	std::vector<ntFace3*>		faces;
	std::vector<ntPolyline*>	profiles;

	ntColor4f col = ntColor4f(0.25f,0.25f,0.25f,1);
	std::vector<ntCol4> cols;

	// PROFILE TYPES:  SQUARE, POLYGON, POLYPARM, RECTANGLE
	void gen_profile(ntEdge* edge, float p);	//proportional to length
	//void gen_profile(ntEdge* edge, ntPolygon polygon);
	void gen_profile(ntEdge* edge, int sides, float radius);
	void gen_profile(ntEdge* edge, float dim0, float dim1);

	void gen_branch();
	void gen_node();

	void init();
public:

	ntNodeGen();
	ntNodeGen(std::vector<ntEdge*>* edges);
	ntNodeGen(std::vector<ntEdge*> edges);

	void set_Color(ntColor4f col);
	void set_Parameters(SectMode mode, int div, float neck);

	void display();
	void display_Verts();
	void display_Edges();
};
#endif
