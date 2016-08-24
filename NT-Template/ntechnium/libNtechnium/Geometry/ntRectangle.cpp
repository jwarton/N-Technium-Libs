#include "ntRectangle.h"

ntRectangle::ntRectangle() {}
ntRectangle::ntRectangle(float dimX, float dimY) :
	dimX(dimX), dimY(dimY) {
	pos = ntVec3(0, 0, 0);
	dim.x = dimX;
	dim.y = dimY;
	dim.z = 0;
	init();
}
ntRectangle::ntRectangle(const ntVec3 & pos, float dimX, float dimY):
ntPolygon(pos), dimX(dimX), dimY(dimY) {
	dim.x = dimX;
	dim.y = dimY;
	dim.z = 0;
	init();
}

void ntRectangle::init() {
	float dim0, dim1;
	dim0 = dimX * 0.5;
	dim1 = dimY * 0.5;
	// PLOT VERTEX POSITIONS
	ntVec3 *v0 = new ntVec3( dim0,  dim1, 0);
	ntVec3 *v1 = new ntVec3(-dim0,  dim1, 0);
	ntVec3 *v2 = new ntVec3(-dim0, -dim1, 0);
	ntVec3 *v3 = new ntVec3( dim0, -dim1, 0);
	vecs.push_back(v0);
	vecs.push_back(v1);
	vecs.push_back(v2);
	vecs.push_back(v3);

	for (int i = 0; i < vecs.size(); i++) {
		ntVertex vert = ntVertex(vecs[i]);
		verts.push_back(&vert);
	}

	//for (int j = 0; j < sides; j++) {
	//	float theta = 2.0f * M_PI * j / sides;
	//	float x = radius * cosf(theta);
	//	float y = radius * sinf(theta);

	//	ntVec3* v = new ntVec3(x, y, 0);
	//	vecs.push_back(v);
	//	/// UPDATE: TRANSFORM TO POS, ROT//
	//	ntVertex vert = ntVertex(v);
	//	verts.push_back(&vert);
	//}

	init_edges();
}