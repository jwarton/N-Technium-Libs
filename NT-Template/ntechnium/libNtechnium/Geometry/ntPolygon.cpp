#include "ntPolygon.h"

ntPolygon::ntPolygon() {}
ntPolygon::ntPolygon(const ntVec3 & pos):
	ntPolyline(pos){
}
ntPolygon::ntPolygon(float radius, int sides) :
	radius(radius), sides(sides) {
	pos = ntVec3(0, 0, 0);
	dim.x = radius;
	dim.y = 0;
	dim.z = 0;

	init();
}
ntPolygon::ntPolygon(const ntVec3 & pos, float radius, int sides):
ntPolyline(pos), radius(radius), sides(sides){
	dim.x = radius;
	dim.y = 0;
	dim.z = 0;
	
	init();
}

void ntPolygon::init() {
	// EXCEPTION FOR INVALID POLYGON SEGMENTS
	if (sides < 3) {
		this->sides = 3;
		std::cout << "EXCEPTION OVER RIDER : (int sides) MUST BE > 3" << endl;
	}
	// PLOT VERTEX POSITIONS
	for (int j = 0; j < sides; j++) {
		float theta = 2.0f * M_PI * j / sides;
		float x = radius * cosf(theta);
		float y = radius * sinf(theta);

		ntVec3* v = new ntVec3(x, y, 0);
		vecs.push_back(v);
		/// UPDATE: TRANSFORM TO POS, ROT//
		ntVertex vert = ntVertex(v);
		verts.push_back(&vert);
	}
	init_edges();
}