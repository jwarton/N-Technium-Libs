#include "ntPolygon.h"

ntPolygon::ntPolygon() {}
ntPolygon::ntPolygon(const ntVec3 & pos):
	ntPolyline(pos){
	isClosed = true;
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
	init();
}

ntPolygon::ntPolygon(float radius, int sides, ntCol4 color):
	radius(radius), sides(sides){
	col = color;
	pos = ntVec3(0, 0, 0);
	dim.x = radius;
	dim.y = 0;
	dim.z = 0;
	init();
}
ntPolygon::ntPolygon(const ntVec3 & pos, float radius, int sides, ntCol4 color):
	ntPolyline(pos), radius(radius), sides(sides) {
	col = color;
	dim.x = radius;
	init();
}

void ntPolygon::init() {
	isClosed = true;
	// EXCEPTION FOR INVALID POLYGON SEGMENTS
	if (sides < 3) {
		this->sides = 3;
		std::cout << "EXCEPTION OVER RIDE : (int sides) MUST BE > 3" << endl;
	}
	// PLOT VERTEX POSITIONS
	for (int i = 0; i < sides; i++) {
		float theta = 2.0f * M_PI * i / sides;
		float x = radius * cosf(theta);
		float y = radius * sinf(theta);

		ntVec3* v = new ntVec3(x, y, 0);
		vecs.push_back(v);

		*vecs[i] += pos;						//APPLY POSITION
		ntVertex vert = ntVertex(v);
		verts.push_back(&vert);
	}
	init_edges();
}

void ntPolygon::calc_centroid() {

	std::cout << "CALC_CENTROID IS FUNCTION UNDEFINED" << endl;
	//cent = new ntVec3();
	//cent->x = (v1->x + v2->x + v0->x)/3;
	//   cent->y = (v1->y + v2->y + v0->y)/3;
	//   cent->z = (v1->z + v2->z + v0->z)/3;
	//centroid = ntVertex(cent);
	//centroid.setSize(2);
	//centroid.set_color(ntColor4f(1,1,1,0));
}
void ntPolygon::calc_norm() {
	std::cout << "CALC_NORMAL IS FUNCTION UNDEFINED" << endl;
	//ntVec3* t1 = new ntVec3();
	//   t1->set(v2);
	//   t1->sub(v1);

	//   ntVec3* t2 = new ntVec3();
	//   t2->set(v0);
	//   t2->sub(v1);

	//   t1->unitize();
	//   t2->unitize();
	//
	//norm = t1->cross(t2);
	////norm.invert();
	//normal = ntNormal(*cent,norm,.05);
}
void ntPolygon::calc_area() {
	std::cout << "CALC_AREA IS FUNCTION UNDEFINED" << endl;
}

double ntPolygon::get_area() {
	return area;
}

bool ntPolygon::pt_isInside(ntVec3* point) {
	int i, j, nvert = vecs.size();
	bool c = false;

	double x_Mn = pos.x - radius;
	double x_Mx = pos.x + radius;
	double y_Mx = pos.y + radius;
	double y_Mn = pos.y - radius;

	if (point->x > x_Mn && point->x < x_Mx && point->y < y_Mx && point->y > y_Mn) {
		for (i = 0, j = nvert - 1; i < nvert; j = i++) {
			if (((vecs[i]->y >= point->y) != (vecs[j]->y >= point->y)) &&
				(point->x <= (vecs[j]->x - vecs[i]->x) * (point->y - vecs[i]->y) / (vecs[j]->y - vecs[i]->y) + vecs[i]->x)
				)
				c = !c;
		}
	}
	return c;
}