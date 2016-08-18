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
	void init();

	int mode = RECTANGLE;
	//NODE OBJECTS
	//ntVec3 *v0;					//NODE JUNCTURE	
	//std::vector<ntVec3 *> vecs;	//SPOKE END POINTS v1
	//std::vector<ntEdge*> edges;	//NODE SPOKES

	std::vector<ntVec3 *>	vecs;
	std::vector<ntVertex*>	verts;
	std::vector<ntEdge*>	edges;
	std::vector<ntFace3*>	faces;

	std::vector<ntPolyline*> profiles;

	ntColor4f col;
	std::vector<ntCol4> cols;

	void gen_profile(ntEdge* edge);
	void gen_profile(ntEdge* edge, float p);	//proportional to length
	//void gen_profile(ntEdge* edge, ntPolygon polygon);
	void gen_profile(ntEdge* edge, int sides, float radius);
	void gen_profile(ntEdge* edge, float dim0, float dim1);

	void gen_branch(int sides);
	void gen_node();

public:

	ntNodeGen();
	ntNodeGen(std::vector<ntEdge*>* edges);
	ntNodeGen(std::vector<ntEdge*> edges);

	void setColor(ntColor4f col);

	void display();
	void display_Verts();
	void display_Edges();
};
#endif
