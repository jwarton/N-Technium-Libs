// ntEdge.h
// openGl scratch libs
// created by James Warton on 02/20/2014

#ifndef EDGE_JPW_NTECHNIUM
#define EDGE_JPW_NTECHNIUM

#include <iostream>
#include <sstream>
#include <vector>
#include "ntVec3.h"
#include "ntVertex.h"

using namespace std;
using namespace jpw;

enum L_type{ SOLID, DASHED, DOT};

class ntEdge;
typedef ntEdge edge;

class ntEdge{
private:
	void init();

	L_type type;
public:
	//end positions
	ntVec3* v0;
	ntVec3* v1;
	//vertex pointers
	ntVertex *vert0,*vert1;
	ntVertex *verts[2];
	//color variables
	ntColor4f colS;
	ntColor4f colE;
	ntColor4f col;
	//line thickness
	float w;

	ntEdge();
	ntEdge(ntVec3* v0,ntVec3* v1);
	ntEdge(ntVertex* vert0, ntVertex* vert1);
	ntEdge(ntVec3* v0,ntVec3* v1,ntColor4f col);
	ntEdge(ntVec3* v0,ntVec3* v1,float weight);
	ntEdge(ntVec3* v0,ntVec3* v1,ntColor4f col,float weight);
	ntEdge(ntVec3* v0,ntVec3* v1,ntVertex* vert0, ntVertex* vert1);

	float getLength();
	ntVec3*  getMid();

	void setCol(ntColor4f col);
	void setEdge(float w);
	void display(float w);
	void display_mode(L_type type);
	void display();		///REFACTOR FOR UNIFORM APPROACH
	void display_X();	///TEMPORARY FOR TRUSS DISPLAY
};

inline float ntEdge::getLength() {
	float length = v0->distance(v1);
	return length;
}

inline ntVec3* ntEdge::getMid() {

	float x = (v0->x + v1->x) * 0.5;
	float y = (v0->y + v1->y) * 0.5;
	float z = (v0->z + v1->z) * 0.5;

	ntVec3 * v = new ntVec3(x, y, z);
	return v;
}

#endif