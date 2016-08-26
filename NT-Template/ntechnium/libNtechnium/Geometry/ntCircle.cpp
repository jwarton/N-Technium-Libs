#include "ntCircle.h"

ntCircle::ntCircle(){}

ntCircle::ntCircle(const ntVec3& pos, float rad):
ntPolygon(pos),rad(rad){
	seg = 36;

	dim.x = rad;
	dim.y = 0;
	dim.z = 0;

	init();
}
ntCircle::ntCircle(const ntVec3& pos, float rad, int seg): 
	ntPolygon(pos), rad(rad) {
	init();
}
ntCircle::ntCircle(const ntVec3& pos, float rad, ntCol4 col):
	ntPolygon(pos), rad(rad){
	this->col = col;
	init();
}
ntCircle::ntCircle(const ntVec3& pos, float rad, int seg, ntCol4 col) :
	ntPolygon(pos), rad(rad), seg(seg){
	this->col = col;
	init();
}

void ntCircle::init(){	
	// EXCEPTION FOR INVALID POLYGON SEGMENTS
	if (seg < 18) {
		this->seg = 18;
		std::cout << "EXCEPTION OVER RIDE : (int sides) MUST BE > 18" << endl;
	}
	// PLOT VERTEX POSITIONS
	for (int i = 0; i < seg; ++i) {
		float theta = 2.0f * M_PI * i / seg;
		float x = rad * cosf(theta);
		float y = rad * sinf(theta);	
		
		ntVec3* v = new ntVec3(x + pos.x, y + pos.y, 0);
		vecs.push_back(v);
		/// UPDATE: TRANSFORM TO POS, ROT//
		ntVertex* vert = new  ntVertex(v);
		verts.push_back(vert);
	}
	init_edges();
	//INITIALIZE CENTROID AND NORMAL
	//calcCentroid();
	//calcNorm();
	calc_area();
}

void ntCircle::display_dots() {
	glColor4f(col.r, col.g, col.b, col.a);
	glBegin(GL_POINTS);
	glPointSize(1);															
	for (int i = 0; i<seg; ++i) {
		glVertex2f(vecs.at(i)->x, vecs.at(i)->y);
	}
	glEnd();
}
void ntCircle::calc_area() {
	area = M_PI * pow(rad, 2);
}