#include "ntRectangle.h"

ntRectangle::ntRectangle() {}
ntRectangle::ntRectangle(float dimX, float dimY) :
	dimX(dimX), dimY(dimY) {
	pos = ntVec3(0, 0, 0);
	rot = ntVec3(0, 0, 0);
	dim.x = dimX;
	dim.y = dimY;
	dim.z = 0;
	init();
}
ntRectangle::ntRectangle(const ntVec3 & pos, float dimX, float dimY):
ntPolygon(pos), dimX(dimX), dimY(dimY) {
	dim.x = dimX;
	dim.y = dimY;
	init();
}

ntRectangle::ntRectangle(float dimX, float dimY, ntCol4 color):
	dimX(dimX), dimY(dimY) {
	pos = ntVec3(0, 0, 0);
	rot = ntVec3(0, 0, 0);
	col = color;
	dim.x = dimX;
	dim.y = dimY;
	dim.z = 0;
	init();
}

ntRectangle::ntRectangle(const ntVec3 & pos, float dimX, float dimY, ntCol4 color):
	ntPolygon(pos), dimX(dimX), dimY(dimY) {
	col = color;
	dim.x = dimX;
	dim.y = dimY;
	init();
}

void ntRectangle::init() {
	float dim0, dim1;
	dim0 = dimX * 0.5;
	dim1 = dimY * 0.5;
	// PLOT VERTEX POSITIONS
	ntVec3 *v0 = new ntVec3( dim0,  dim1, pos.z);
	ntVec3 *v1 = new ntVec3(-dim0,  dim1, pos.z);
	ntVec3 *v2 = new ntVec3(-dim0, -dim1, pos.z);
	ntVec3 *v3 = new ntVec3( dim0, -dim1, pos.z);
	vecs.push_back(v0);
	vecs.push_back(v1);
	vecs.push_back(v2);
	vecs.push_back(v3);
	for (int i = 0; i < vecs.size(); i++) {
		*vecs[i] += pos;						//APPLY POSITION
												//ROTATE rot
		ntVertex* vert = new ntVertex(vecs[i]);
		verts.push_back(vert);
	}
	init_edges();
}