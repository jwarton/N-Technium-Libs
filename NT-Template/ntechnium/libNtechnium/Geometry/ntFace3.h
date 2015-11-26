// ntFace3.h
// openGl scratch libs
// created by James Warton on 02/20/2014

#ifndef FACE3_JPW_NTECHNIUM
#define FACE3_JPW_NTECHNIUM

#include <iostream>
#include <vector>
#include "ntVertex.h"
#include "ntVec3.h"
#include "ntEdge.h"
#include "ntNormal.h"
#include "ntMath.h"
using namespace jpw;

class ntFace3{
private:
	void init();

public:
	ntVec3 *v0,*v1,*v2;
	ntVec3 *vecs[3];
	ntVec3 *uvw0, *uvw1, *uvw2;
	ntVec3 *uvws[3];
	ntVertex *vert0,*vert1,*vert2;
	std::vector<ntVertex*> verts;
	std::vector<ntEdge> edges;
	
	ntColor4f col;
	std::vector<ntCol4> cols;

	
	ntVec3	 norm;
	ntVec3  *cent;
	ntVertex centroid;
	ntNormal normal;

	ntFace3();
	ntFace3(ntVec3* v0,ntVec3* v1,ntVec3* v2);
	ntFace3(ntVec3* v0,ntVec3* v1,ntVec3* v2, ntColor4f col);
	ntFace3(ntVec3* v0,ntVec3* v1,ntVec3* v2,ntVertex* vert0,ntVertex* vert1,ntVertex* vert2);

	bool pt_isInside(ntVec3* vec);
	void setColor(ntColor4f col);
	void setUVW(ntVec3* uvw0, ntVec3* uvw1, ntVec3* uvw2);
	void setUVW(std::vector <ntVec3*>	uvws);

	void calcNorm();
	void calcCentroid();
	void display();
	void display(L_mode mode);
};
#endif
