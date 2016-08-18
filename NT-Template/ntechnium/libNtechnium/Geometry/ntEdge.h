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

enum line_Type{ SOLID, DASHED, DOTTED};

class ntEdge;
typedef ntEdge edge;

class ntEdge{
private:
	void init();
	line_Type type = SOLID;
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

	double length();
	float getLength();
	float getLength2();

	ntVec3*  getMid();
	ntVec3*	 get_PtP(double param);
	void setCol(ntColor4f col);
	void setEdge(float w);
	void display(float w);
	void display_mode(line_Type line_type);
	void display();		///REFACTOR FOR UNIFORM APPROACH
	void display_X();	///TEMPORARY FOR TRUSS DISPLAY
};
inline double ntEdge::length() {
	double length = v0->distance(v1);
	return length;
}
inline float ntEdge::getLength() {	
	ntVec3*axis = new ntVec3(1,0,0);
	ntVec3 vE;

	vE.set(v1);
	vE.sub(v0);
	vE.align(axis);

	delete axis;

	float length = abs(vE.x);
	return length;
}
inline float ntEdge::getLength2() {

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
inline ntVec3* ntEdge::get_PtP(double param) {

	ntVec3 * v = new ntVec3(v1->x, v1->y, v1->z);
	v->scale(v0, param);
	return v;
}

#endif