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
//
//ntCircle::ntCircle(ntVec3* v0,ntVec3* v1,ntVec3* v2,ntColor4f col):
//v0(v0),v1(v1),v2(v2),col(col){
//	this->vecs[0] = v0;
//	this->vecs[1] = v1;
//	this->vecs[2] = v2;
//	init();
//}
//
//ntCircle::ntCircle(ntVec3* v0,ntVec3* v1,ntVec3* v2,ntVertex* vert0,ntVertex* vert1,ntVertex* vert2):
//v0(v0),v1(v1),v2(v2),vert0(vert0),vert1(vert1),vert2(vert2){
//
//	this->vecs[0] = v0;
//	this->vecs[1] = v1;
//	this->vecs[2] = v2;
//
//	verts.push_back(this->vert0);
//	verts.push_back(this->vert1);
//	verts.push_back(this->vert2);
//
//	edges.push_back(ntEdge(v0,v1,vert0,vert1));
//	edges.push_back(ntEdge(v1,v2,vert1,vert2));
//	edges.push_back(ntEdge(v2,v0,vert2,vert0));
//
//	//initialize centroid and normal
//	calcCentroid();
//	calcNorm();
//}

void ntCircle::init(){
	seg = 18;															//add set function and unique constructor
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
	glBegin(GL_LINE_LOOP);
	//glBegin(GL_POINTS);
	//glPointSize(1);															
	for (int i = 0; i<seg; ++i) {
		glVertex3f(vecs.at(i)->x, vecs.at(i)->y, 0);
	}
	glEnd();
}