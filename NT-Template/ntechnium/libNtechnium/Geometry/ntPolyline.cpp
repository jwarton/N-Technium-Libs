#include "ntPolyline.h"

ntPolyline::ntPolyline() {}
ntPolyline::ntPolyline(ntVec3* vS, ntVec3* vE):
vS(vS), vE(vE){
	vecs.push_back(vS);
	vecs.push_back(vE);
	isClosed = false;
	init();
}
ntPolyline::ntPolyline(std::vector <ntVec3*> vecs, bool isClosed) :
	vecs(vecs), isClosed(isClosed) {
	if (isClosed == true) {
		this->vecs.push_back(vecs[0]);
	}

	init();
}

void ntPolyline::init() {
	//DEFAULT LINE TYPE
	col = ntColor4f(0.25, 0.25, 0.25, 1);
	w = .1f;

	//INITIALIZE POLYLINE VERTEX DATA STRUCTURES
	int cnt = vecs.size();
	vS = vecs[0];
	vE = vecs[cnt];
	ntVertex vert = ntVertex(vS);
	verts.push_back(vert);

	for (int i = 1; i < cnt; i++) {
			ntVec3* v0 = vecs[i - 1];
			ntVec3* v1 = vecs[i];
			if ((i != cnt - 1) && (isClosed == true)) {
				vert = ntVertex(v1);
				verts.push_back(vert);
			}
			ntEdge e = ntEdge(v0, v1);
			edges.push_back(e);
	}
}

void ntPolyline::setClosed() {
	if (isClosed == false) {
		int i = vecs.size() - 1;
		ntEdge e = ntEdge(vecs[i], vecs[0]);
		edges.push_back(e);
		vecs.push_back(vecs[0]);
		isClosed = true;
	}
}
void ntPolyline::setCol(ntColor4f col){
this->col  = col;
}
void ntPolyline::setWeight(float w){
this->w = w;
}

void ntPolyline::display(float w){
	this->w = w;
	display();
}
void ntPolyline::display(){
	glLineWidth(w);
	glColor4f(col.r, col.g, col.b, col.a);
	glBegin(GL_LINES);
	for (int i = 0; i < vecs.size()-1; i++) {
		glVertex3f(vecs[i]->x, vecs[i]->y, vecs[i]->z);
		glVertex3f(vecs[i+1]->x, vecs[i+1]->y, vecs[i+1]->z);
	}
	glEnd();
}
