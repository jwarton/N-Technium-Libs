#include "ntFace3.h"

ntFace3::ntFace3(){}

ntFace3::ntFace3(ntVec3* v0,ntVec3* v1,ntVec3* v2):
v0(v0),v1(v1),v2(v2){
	this->vecs[0] = v0;
	this->vecs[1] = v1;
	this->vecs[2] = v2;
	this->col = ntColor4f(1,1,1,1);
	init();
}

ntFace3::ntFace3(ntVec3* v0,ntVec3* v1,ntVec3* v2,ntColor4f col):
v0(v0),v1(v1),v2(v2){
	this->vecs[0] = v0;
	this->vecs[1] = v1;
	this->vecs[2] = v2;
	this->col = col;
	init();
}

ntFace3::ntFace3(ntVec3* v0,ntVec3* v1,ntVec3* v2,ntVertex* vert0,ntVertex* vert1,ntVertex* vert2):
v0(v0),v1(v1),v2(v2),vert0(vert0),vert1(vert1),vert2(vert2){

	this->vecs[0] = v0;
	this->vecs[1] = v1;
	this->vecs[2] = v2;

	verts.push_back(this->vert0);
	verts.push_back(this->vert1);
	verts.push_back(this->vert2);

	edges.push_back(ntEdge(v0,v1,vert0,vert1));
	edges.push_back(ntEdge(v1,v2,vert1,vert2));
	edges.push_back(ntEdge(v2,v0,vert2,vert0));

	//initialize centroid and normal
	calcCentroid();
	calcNorm();
}

void ntFace3::init(){
	//initialize vertex
	vert0 = &ntVertex (v0,col);
	vert1 = &ntVertex (v1,col);
	vert2 = &ntVertex (v2,col);
	verts.push_back(vert0);
	verts.push_back(vert1);
	verts.push_back(vert2);
	//initialize edges
	edges.push_back(ntEdge(v0,v1,col));
	edges.push_back(ntEdge(v1,v2,col));
	edges.push_back(ntEdge(v2,v0,col));
	//initialize centroid and normal
	calcCentroid();
	calcNorm();
}
void ntFace3::calcCentroid(){
	cent = new ntVec3();
	cent->x = (v1->x + v2->x + v0->x)/3;
    cent->y = (v1->y + v2->y + v0->y)/3;
    cent->z = (v1->z + v2->z + v0->z)/3;
	centroid = ntVertex(cent);
	centroid.setSize(2);
	centroid.setColor(ntColor4f(1,1,1,0));
}

void ntFace3::calcNorm(){
	ntVec3* t1 = new ntVec3();
    t1->set(v2);
    t1->sub(v1);

    ntVec3* t2 = new ntVec3();
    t2->set(v0);
    t2->sub(v1);

    t1->unitize();
    t2->unitize();
	
	norm = t1->cross(t2);
	//norm.invert();
	normal = ntNormal(*cent,norm,.05);
}

void ntFace3::setColor(ntColor4f col){
	this->col=col;
	for(int i = 0; i<verts.size(); i++){
		verts.at(i)->setColor(col);
	}
}

void ntFace3::display(){
	glBegin(GL_TRIANGLES);
	glColor4f(vert0->col.r, vert0->col.g, vert0->col.b, vert0->col.a);
    glVertex3f(v0->x, v0->y, v0->z);
	glColor4f(vert1->col.r, vert1->col.g, vert1->col.b, vert1->col.a);
    glVertex3f(v1->x, v1->y, v1->z);	
	glColor4f(vert2->col.r, vert2->col.g, vert2->col.b, vert2->col.a);
    glVertex3f(v2->x, v2->y, v2->z);
    glEnd();
	///TODO centroid functionality needs work
	//centroid.display();
}
