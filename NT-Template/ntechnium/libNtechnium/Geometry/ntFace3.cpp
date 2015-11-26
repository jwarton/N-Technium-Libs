#include "ntFace3.h"

ntFace3::ntFace3(){}

ntFace3::ntFace3(ntVec3* v0,ntVec3* v1,ntVec3* v2):
v0(v0),v1(v1),v2(v2){
	this->vecs[0] = v0;
	this->vecs[1] = v1;
	this->vecs[2] = v2;
	col = ntColor4f(1,1,1,1);
	init();
}

ntFace3::ntFace3(ntVec3* v0,ntVec3* v1,ntVec3* v2,ntColor4f col):
v0(v0),v1(v1),v2(v2),col(col){
	this->vecs[0] = v0;
	this->vecs[1] = v1;
	this->vecs[2] = v2;
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
	vert0 = new ntVertex (v0,col);
	vert1 = new ntVertex (v1,col);
	vert2 = new ntVertex (v2,col);

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

bool ntFace3::pt_isInside(ntVec3* vec) {
	int i, j, nvert = verts.size();
	bool c = false;

	for (i = 0, j = nvert - 1; i < nvert; j = i++) {
		if (((vecs[i]->y >= vec->y) != (vecs[j]->y >= vec->y)) &&
			(vec->x <= (vecs[j]->x - vecs[i]->x) * (vec->y - vecs[i]->y) / (vecs[j]->y - vecs[i]->y) + vecs[i]->x)
			)
			c = !c;
	}
	return c;
}

void ntFace3::setColor(ntColor4f col){
	this->col=col;
	for(int i = 0; i<verts.size(); i++){
		verts.at(i)->setColor(col);
	}
}
void ntFace3::setUVW(ntVec3* uvw0, ntVec3* uvw1, ntVec3* uvw2) {
	this->uvw0 = uvw0;
	this->uvw1 = uvw1;
	this->uvw2 = uvw2;
	this->uvws[0] = uvw0;
	this->uvws[1] = uvw1;
	this->uvws[2] = uvw2;
}
void ntFace3::setUVW(std::vector <ntVec3*>	uvws) {
	this->uvw0 = uvws[0];
	this->uvw1 = uvws[1];
	this->uvw2 = uvws[2];
	this->uvws[0] = uvws[0];
	this->uvws[1] = uvws[1];
	this->uvws[2] = uvws[2];
}
void ntFace3::display(){
	glBegin(GL_TRIANGLES);
	glColor4f(verts.at(0)->col.r, verts.at(0)->col.g, verts.at(0)->col.b, verts.at(0)->col.a);
	glVertex3f(v0->x, v0->y, v0->z);
	glColor4f(verts.at(1)->col.r, verts.at(1)->col.g, verts.at(1)->col.b, verts.at(1)->col.a);
	glVertex3f(v1->x, v1->y, v1->z);
	glColor4f(verts.at(2)->col.r, verts.at(2)->col.g, verts.at(2)->col.b, verts.at(2)->col.a);
	glVertex3f(v2->x, v2->y, v2->z);
	glEnd();
	///TODO centroid functionality needs work
	//centroid.display();
}
void ntFace3::display(L_mode mode) {
	if (mode == vX) {
		glBegin(GL_TRIANGLES);
		glColor4f(verts.at(0)->col.r, verts.at(0)->col.g, verts.at(0)->col.b, verts.at(0)->col.a);
		glVertex3f(v0->x, v0->y, v0->z);
		glVertex3f(v1->x, v1->y, v1->z);
		glVertex3f(v2->x, v2->y, v2->z);
		glEnd();
	}
	if (mode == vC) {
		glBegin(GL_TRIANGLES);
		glColor4f(col.r, col.g, col.b, col.a);
		glVertex3f(v0->x, v0->y, v0->z);
		glVertex3f(v1->x, v1->y, v1->z);
		glVertex3f(v2->x, v2->y, v2->z);
		glEnd();
	}
	if (mode == vV) {
		glBegin(GL_TRIANGLES);
		glColor4f(verts.at(0)->col.r, verts.at(0)->col.g, verts.at(0)->col.b, verts.at(0)->col.a);
		glVertex3f(v0->x, v0->y, v0->z);
		glColor4f(verts.at(1)->col.r, verts.at(1)->col.g, verts.at(1)->col.b, verts.at(1)->col.a);
		glVertex3f(v1->x, v1->y, v1->z);
		glColor4f(verts.at(2)->col.r, verts.at(2)->col.g, verts.at(2)->col.b, verts.at(2)->col.a);
		glVertex3f(v2->x, v2->y, v2->z);
		glEnd();
	}
}