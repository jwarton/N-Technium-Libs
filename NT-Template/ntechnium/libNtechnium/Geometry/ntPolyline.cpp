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

void ntPolyline::init() {
	//DEFAULT LINE TYPE
	col = ntColor4f(0.25, 0.25, 0.25, 1);
	stroke = 0.5;

	//INITIALIZE POLYLINE VERTEX DATA STRUCTURES
	int cnt = vecs.size();
	vS = vecs[0];
	vE = vecs[cnt];
	ntVertex vert = ntVertex(vS);
	verts.push_back(&vert);

	for (int i = 1; i < cnt; i++) {
			ntVec3* v0 = vecs[i - 1];
			ntVec3* v1 = vecs[i];
			if ((i != cnt - 1) && (isClosed == true)) {
				vert = ntVertex(v1);
				verts.push_back(&vert);
			}
			ntEdge e = ntEdge(v0, v1);
			e.setCol(col);
			e.setEdge(stroke);
			edges.push_back(e);
	}
}

ntPolyline::ntPolyline(const ntVec3 & pos):
	BaseShape(pos){
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
