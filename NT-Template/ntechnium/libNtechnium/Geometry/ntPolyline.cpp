#include "ntPolyline.h"

ntPolyline::ntPolyline() {}
ntPolyline::ntPolyline(ntVec3* vS, ntVec3* vE):
	BaseShape(),vS(vS), vE(vE){
	vecs.push_back(vS);
	vecs.push_back(vE);
	isClosed = false;
	init();
}
ntPolyline::ntPolyline(std::vector <ntVec3*> vecs, bool isClosed):
	BaseShape(vecs), isClosed(isClosed) {
	if (isClosed == true) {
		this->vecs.push_back(vecs[0]);
	}

	init();
}
//PROTECTED CONSTRUCTOR--ONLY ACCESSIBLE TO DERIVED
ntPolyline::ntPolyline(const ntVec3 & pos) :
	BaseShape(pos) {
}

void ntPolyline::init() {
	//INITIALIZE POLYLINE VERTEX DATA STRUCTURES
	vS = vecs[0];
	vE = vecs[vecs.size()];
	ntVertex vert = ntVertex(vS);
	verts.push_back(&vert);

	for (int i = 1; i < vecs.size(); i++) {
		if ((i != vecs.size() - 1) && (isClosed == true)) {
			vert = ntVertex(vecs[i]);
			verts.push_back(&vert);
		}
	}

	init_edges();
}

void ntPolyline::init_edges() {
	//DEFAULT LINE TYPE
	col = ntColor4f(0.25, 0.25, 0.25, 1);
	stroke = 0.5;

	for (int i = 1; i < vecs.size(); i++) {
		ntEdge e = ntEdge(vecs[i - 1], vecs[i]);
		e.setCol(col);
		e.setEdge(stroke);
		edges.push_back(e);
	}
}
void ntPolyline::close() {
	if (isClosed == false) {
		int i = vecs.size() - 1;
		ntEdge e = ntEdge(vecs[i], vecs[0]);
		edges.push_back(e);
		vecs.push_back(vecs[0]);
		isClosed = true;
	}
}
void ntPolyline::set_color(ntColor4f col){
	this->col = col;
	set_edge_color(col);
}
void ntPolyline::set_stroke(float w) {
	for (int i = 0; i < edges.size(); i++) {
		edges[i].setEdge(w);
	}
}
void ntPolyline::display(float w){
	stroke = w;
	display_edges(w);
}
void ntPolyline::display(){
	display_edges();
}
