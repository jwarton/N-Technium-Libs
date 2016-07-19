#include "ntCircle.h"

ntCircle::ntCircle(){}

ntCircle::ntCircle(ntVec3* pos, float rad):
pos(pos),rad(rad){
	col = ntColor4f(1,1,1,1);
	init();
}
ntCircle::ntCircle(ntVec3* pos, float rad, ntCol4 col):
	pos(pos), rad(rad), col(col) {
	init();
}
ntCircle::ntCircle(ntVec3* pos, float rad, int seg, ntCol4 col) :
	pos(pos), rad(rad), seg(seg), col(col) {
	init();
}

void ntCircle::init(){															//add set function and unique constructor
	for (int j = 0; j < seg; ++j) {

		float theta = 2.0f * M_PI * j / seg;							//get the current angle
		float x = rad * cosf(theta);									//calculate the x component
		float y = rad * sinf(theta);									//calculate the y component		
		
		ntVec3* vec = new ntVec3(x + pos->x, y + pos->y, 0);
		vecs.push_back(vec);											//output vertex
	}

	//initialize centroid and normal
	//calcCentroid();
	//calcNorm();
	calcArea();
}
void ntCircle::calcCentroid(){
	//cent = new ntVec3();
	//cent->x = (v1->x + v2->x + v0->x)/3;
 //   cent->y = (v1->y + v2->y + v0->y)/3;
 //   cent->z = (v1->z + v2->z + v0->z)/3;
	//centroid = ntVertex(cent);
	//centroid.setSize(2);
	//centroid.setColor(ntColor4f(1,1,1,0));
}

bool ntCircle::pt_isInside(ntVec3* point) {
	int i, j, nvert = vecs.size();
	bool c = false;

	double x_Mn = pos->x - rad;
	double x_Mx = pos->x + rad;
	double y_Mx = pos->y + rad;
	double y_Mn = pos->y - rad;

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
void ntCircle::calcNorm(){
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
void ntCircle::calcArea() {
	area = M_PI * pow(rad,2);
}
double ntCircle::get_Area(){
	return area;
}
void ntCircle::setColor(ntColor4f col){
	this->col=col;
	for(int i = 0; i<verts.size(); i++){
		verts.at(i)->setColor(col);
	}
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
void ntCircle::display(){
	glColor4f(col.r, col.g, col.b, col.a);
	glLineWidth(1);
	glBegin(GL_LINE_LOOP);													
	for (int i = 0; i<vecs.size(); i++) {
		glVertex2f(vecs.at(i)->x, vecs.at(i)->y);
	}
	glEnd();
}
