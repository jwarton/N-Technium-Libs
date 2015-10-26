// ntCircle.h
// openGl scratch libs
// created by James Warton on 10/25/2015

#ifndef CIRCLE_JPW_NTECHNIUM
#define CIRCLE_JPW_NTECHNIUM
#define _USE_MATH_DEFINES

#include <iostream>
#include <vector>
#include <math.h>

#include "ntVertex.h"
#include "ntVec3.h"
#include "ntEdge.h"
#include "ntNormal.h"
 
using namespace jpw;

class ntCircle{
private:
	void init();

public:
	ntVec3* pos;
	float rad;
	std::vector<ntVec3*> vecs;
	std::vector<ntVertex*> verts;
	std::vector<ntEdge> edges;
	ntColor4f col;
	float seg;

	ntVec3 norm;
	ntVec3* cent;
	ntVertex centroid;
	ntNormal normal;

	ntCircle();
	ntCircle(ntVec3* pos, float rad);
	ntCircle(ntVec3* pos, float rad, ntCol4 col);

	void setColor(ntColor4f col);
	void calcNorm();
	void calcCentroid();
	void display();
	void display_dots();
};
#endif
