#include "ntEdge.h"

ntEdge::ntEdge(){}

ntEdge::ntEdge(ntVec3* v0,ntVec3* v1):
v0(v0),v1(v1){
	col = ntColor4f(1,1,1,1);
	w = .1f;
	init();
}
ntEdge::ntEdge(ntVertex* vert0, ntVertex* vert1):
vert0(vert0), vert1(vert1){
	v0 = vert0->pos;
	v1 = vert1->pos;
	verts[0] = vert0;
	verts[1] = vert1;
	colS = vert0->col;
	colE = vert0->col;
	w = 1;
}
ntEdge::ntEdge(ntVec3* v0,ntVec3* v1,ntColor4f col):
v0(v0),v1(v1),col(col){
	w = .1f;
	init();
}
ntEdge::ntEdge(ntVec3* v0,ntVec3* v1, float w):
v0(v0),v1(v1),w(w){
	col = ntColor4f(1,1,1,1);
	init();
}
ntEdge::ntEdge(ntVec3* v0,ntVec3* v1,ntColor4f col, float w):
v0(v0),v1(v1),col(col),w(w){
	init();
}
ntEdge::ntEdge(ntVec3* v0,ntVec3* v1,ntVertex* vert0, ntVertex* vert1):
v0(v0),v1(v1),vert0(vert0),vert1(vert1){
	w = 1;
	colS = vert0->col;
	colE = vert0->col;
	
	verts[0] = vert0;
	verts[1] = vert1;
}

void ntEdge::init(){
	colS = col;						///resolve init  some classes require internal vertex
	colE = col;						///others require colS and colE
	vert0 = &ntVertex (v0,colS);	/// unifiy approach to edge | vertex 
	vert1 = &ntVertex (v1,colE);	/// relationship between clases
	verts[0] = vert0;
	verts[1] = vert1;
}
void ntEdge::setCol(ntColor4f col){
this->col  = col;
this->colS = col;
this->colE = col;
}

void ntEdge::setEdge(float w){
this->w = w;
}

void ntEdge::display(float w){
	this->w = w;
	display();
}
void ntEdge::display(){
	glLineWidth(w);
	glBegin(GL_LINES);
	glColor4f(colS.r, colS.g, colS.b, colS.a);
	//glColor4f(vert0->col.r, vert0->col.g, vert0->col.b, vert0->col.a);
	glVertex3f(v0->x, v0->y, v0->z);
	glColor4f(colE.r, colE.g, colE.b, colE.a);
	//glColor4f(vert1->col.r, vert1->col.g, vert1->col.b, vert1->col.a);
	glVertex3f(v1->x, v1->y, v1->z);
	glEnd();
}
void ntEdge::display_X(){
	glLineWidth(w);
	glBegin(GL_LINES);
	glColor4f(vert0->col.r, vert0->col.g, vert0->col.b, vert0->col.a);///
	glVertex3f(v0->x, v0->y, v0->z);
	glColor4f(vert1->col.r, vert1->col.g, vert1->col.b, vert1->col.a);///
	glVertex3f(v1->x, v1->y, v1->z);
	glEnd();
}