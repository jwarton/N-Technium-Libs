#include "ntTriCell.h"

ntTriCell::ntTriCell(){}
ntTriCell::ntTriCell(ntVec3* v0,ntVec3* v1,ntVec3* v2):
v0(v0),v1(v1),v2(v2){
	this->vecs[0] = v0;
	this->vecs[1] = v1;
	this->vecs[2] = v2;
	col = ntColor4f(1,1,1,1);
	faces.push_back(new ntFace3(v0, v1, v2));
	init();
}
ntTriCell::ntTriCell(ntFace3* face) {
	faces.push_back(face);
	v0 = face->v0;
	v1 = face->v1;
	v2 = face->v2;

	this->vecs[0] = v0;
	this->vecs[1] = v1;
	this->vecs[2] = v2;
	col = ntCol4(0.25, 0.25, 0.25, 1);

	init();
}
void ntTriCell::init(){
	// INITIALIZE VECS 
	// 0, 1, 2
	vecs_SD.push_back(v0);
	vecs_SD.push_back(v1);
	vecs_SD.push_back(v2);

	// INITIALIZE VERTEX
	vert0 = new ntVertex (v0,col);
	vert1 = new ntVertex (v1,col);
	vert2 = new ntVertex (v2,col);
	verts.push_back(vert0);
	verts.push_back(vert1);
	verts.push_back(vert2);

	// INITIALIZE EDGES
	edges.push_back(ntEdge(v0,v1,col));
	edges.push_back(ntEdge(v1,v2,col));
	edges.push_back(ntEdge(v2,v0,col));

	// DUPLICATE VECS 
	// 3, 4, 5
	ntVec3* v00 = new ntVec3(v0->x, v0->y, v0->z);
	ntVec3* v01 = new ntVec3(v1->x, v1->y, v1->z);
	ntVec3* v02 = new ntVec3(v2->x, v2->y, v2->z);
	vecs_SD.push_back(v00);
	vecs_SD.push_back(v01);
	vecs_SD.push_back(v02);
	edges.push_back(ntEdge(v00, v01, col));
	edges.push_back(ntEdge(v01, v02, col));
	edges.push_back(ntEdge(v02, v00, col));

	// INITIALIZE SUB DIVISION POINTS
	calcCentroid();
	calcNorm();
	calc_EdgeMid();
	calc_innerTri();
	calc_starPts();
	calc_Faces();
	setPolylines();
}
void ntTriCell::calcCentroid(){
	cent = new ntVec3();
	cent->x = (v1->x + v2->x + v0->x)/3;
    cent->y = (v1->y + v2->y + v0->y)/3;
    cent->z = (v1->z + v2->z + v0->z)/3;
	centroid = ntVertex(cent);
	centroid.setSize(3);
	centroid.set_color(ntColor4f(1,0,0,1));
}
void ntTriCell::calcNorm(){
	ntVec3* t1 = new ntVec3();
    t1->set(v2);
    t1->sub(v1);

    ntVec3* t2 = new ntVec3();
    t2->set(v0);
    t2->sub(v1);

    t1->unitize();
    t2->unitize();
	
	norm = t1->cross(t2);
	normal = ntNormal(*cent,norm,.05);
	normal.colS = Col4(1, 0, 0, 1);
	normal.colE = Col4(1, 0, 0, .5);
}
void ntTriCell::calc_EdgeMid() {
	// CENTROID 
	// 6
	vecs_SD.push_back(cent);

	// OUTER TRIANGLE MIDPOINTS 
	// 7, 8, 9
	// EDGE BISECTOR EDGES
	for (int i = 0; i < 3; i++) {
		ntVec3* pt = edges.at(i).getMid();
		ntVertex* v = new ntVertex(pt);
		ntEdge e = ntEdge(cent, pt, col);

		vecs_SD.push_back(pt);
		verts.push_back(v);
		edges.push_back(e);
	}
	// ANGLE BISECTOR EDGES
	for (int i = 3; i < 6; i++) {
		ntEdge e = ntEdge(cent, vecs_SD[i], col);
		edges.push_back(e);
	}
}
void ntTriCell::calc_innerTri() {
	// INNER TRIANGLE CORNER POINTS
	// 10, 11, 12
	for (int i = 3; i < 6; i++) {
		ntVec3* pt = edges.at(i).get_PtP(scFx);
		ntVertex* v = new ntVertex(pt);

		vecs_SD.push_back(pt);
		verts.push_back(v);
	}
	//  INNER TRIANGLE
	ntEdge e;
	for (int i = 10; i < 13; i++) {
		
		if (i == 12) {
			e = ntEdge(vecs_SD[i], vecs_SD[10]);
		}
		else {
			e = ntEdge(vecs_SD[i], vecs_SD[i + 1]);
		}
		
		e.setCol(col);
		edges.push_back(e);
	}
	// INNER TRIANGLE MIDPOINTS
	// 13, 14, 15
	for (int i = 9; i < 12; i++) {
		ntVec3* pt = edges.at(i).getMid();
		ntVertex* v = new ntVertex(pt);

		vecs_SD.push_back(pt);
		verts.push_back(v);
	}
}
void ntTriCell::calc_starPts() {
		int index[6][2] = {
			{ 3, 10 },
			{ 3, 12 },
			{ 4, 11 },
			{ 4, 10 },
			{ 5, 12 },
			{ 5, 11 }
		};
		for (int i = 0; i < 6; i++) {
			int indS = index[i][0];
			int indE = index[i][1];
			ntEdge e = ntEdge(vecs_SD[indS], vecs_SD[indE]);
			e.setCol(col);
			edges.push_back(e);
		}
}
void ntTriCell::calc_Faces() {
	int index[7][3] = {
		{  3,   4,   10 },
		{  4,   5,   11 },
		{  5,   3,   12 },
		{  3,  10,   12 },
		{  4,  11,   10 },
		{  5,  12,   11 },
		{ 10,  11,   12 }
	};
	for (int i = 0; i < 7; i++) {
		int ind0 = index[i][0];
		int ind1 = index[i][1];
		int ind2 = index[i][2];
		ntFace3* f = new ntFace3(vecs_SD[ind0], vecs_SD[ind1], vecs_SD[ind2]);
		f->set_color(ntCol4(1, 1, 1, 0.75));
		faces.push_back(f);
	}
}
bool ntTriCell::pt_isInside(ntVec3* vec) {
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
void ntTriCell::setScale(float scFx) {

	cent->x = (v1->x + v2->x + v0->x) / 3;
	cent->y = (v1->y + v2->y + v0->y) / 3;
	cent->z = (v1->z + v2->z + v0->z) / 3;

	//  OUTER TRIANGLE CORNER/MID POINTS
	int cnt = vecs_SD.size();
	for (int i = 3; i < cnt; i++) {
		if (i != 6) {
			ntMatrix4 SC3 = ntMatrix4(vecs_SD.at(i));
			SC3.scale3d(scFx, *cent);
		}
	}

	cent->x = (vecs_SD[3]->x + vecs_SD[4]->x + vecs_SD[5]->x) / 3;
	cent->y = (vecs_SD[3]->y + vecs_SD[4]->y + vecs_SD[5]->y) / 3;
	cent->z = (vecs_SD[3]->z + vecs_SD[4]->z + vecs_SD[5]->z) / 3;

	//cent->x = (v1->x + v2->x + v0->x) / 3;
	//cent->y = (v1->y + v2->y + v0->y) / 3;
	//cent->z = (v1->z + v2->z + v0->z) / 3;

	//  INNER TRIANGLE CORNER POINTS
	for (int i = 6; i < 9; i++) {
		ntVec3* pt = edges.at(i).get_PtP(scFx);
		vecs_SD.at(i + 4)->set(pt);
	}
}
void ntTriCell::set_color(ntColor4f col){
	this->col=col;
	for(int i = 0; i<verts.size(); i++){
		verts.at(i)->set_color(col);
	}
}
void ntTriCell::setUVW(ntVec3* uvw0, ntVec3* uvw1, ntVec3* uvw2) {
	this->uvw0 = uvw0;
	this->uvw1 = uvw1;
	this->uvw2 = uvw2;
	this->uvws[0] = uvw0;
	this->uvws[1] = uvw1;
	this->uvws[2] = uvw2;
}
void ntTriCell::setUVW(std::vector <ntVec3*>	uvws) {

	this->uvw0 = uvws[0];
	this->uvw1 = uvws[1];
	this->uvw2 = uvws[2];
	this->uvws[0] = uvws[0];
	this->uvws[1] = uvws[1];
	this->uvws[2] = uvws[2];
}
void ntTriCell::setPolylines(){

	std::vector <vector<vector <int>>> mat_10;

	std::vector <vector <int>> mat_00;
	std::vector <int> ind_00 = { 0,1,2,0 };
	mat_00.push_back(ind_00);

	std::vector <vector <int>> mat_01;
	std::vector <int> ind_10 = { 3, 4, 5, 3 };
	mat_01.push_back(ind_10);

	std::vector <vector <int>> mat_02;
	std::vector <int> ind_20 = { 3, 10, 4, 11, 5, 12, 3 };
	mat_02.push_back(ind_20);

	std::vector <vector <int>> mat_03;
	std::vector <int> ind_30 = { 10, 11, 12, 10 };
	mat_03.push_back(ind_30);

	std::vector <vector <int>> mat_04;
	std::vector <int> ind_40 = { 3, 6 };
	std::vector <int> ind_41 = { 4, 6 };
	std::vector <int> ind_42 = { 5, 6 };
	mat_04.push_back(ind_40);
	mat_04.push_back(ind_41);
	mat_04.push_back(ind_42);

	std::vector <vector <int>> mat_05;
	std::vector <int> ind_50 = { 10, 6 };
	std::vector <int> ind_51 = { 11, 6 };
	std::vector <int> ind_52 = { 12, 6 };
	mat_05.push_back(ind_50);
	mat_05.push_back(ind_51);
	mat_05.push_back(ind_52);

	std::vector <vector <int>> mat_06;
	mat_06.push_back(ind_00);
	mat_06.push_back(ind_10);
	mat_06.push_back(ind_20);
	mat_06.push_back(ind_30);

	mat_10.push_back(mat_00);
	mat_10.push_back(mat_01);
	mat_10.push_back(mat_02);
	mat_10.push_back(mat_03);
	mat_10.push_back(mat_04);
	mat_10.push_back(mat_05);
	mat_10.push_back(mat_06);

	for (int i = 0; i < mat_10.size(); i++) {
		std::vector<ntPolyline*> plines;
		for (int j = 0; j < mat_10[i].size(); j++) {
			ntPolyline * pline = add_Polyline(mat_10[i][j]);
			plines.push_back(pline);
		}
		polylines.push_back(plines);
	}
}
ntPolyline*  ntTriCell::add_Polyline(std::vector<int> index) {

	std::vector <ntVec3 *> vecs_00;
	int cnt = index.size();
	bool isClosed = false;

	if (index[0] == index[cnt-1]) {
		isClosed = true;
		cnt = cnt - 1;
	}

	for (int i = 0; i < cnt; i++) {
		ntVec3* v = vecs_SD[index[i]];
		vecs_00.push_back(v);
	}

	ntPolyline * p = new ntPolyline(vecs_00, isClosed);
	return p;
}
void ntTriCell::display(){
	for (int i = 0; i < 3; i++) {
		edges.at(i).display();
	}
	displaySubD(mode);
}
void ntTriCell::display_Verts() {
	for (int i = 0; i < verts.size(); i++) {
		verts.at(i)->setSize(3);
		verts.at(i)->display();
	}
}
void ntTriCell::display_Edges() {
	for (int i = 0; i < edges.size(); i++) {
		edges.at(i).display();
	}
}
void ntTriCell::display_Centroid() {
	centroid.display();
}
void ntTriCell::display_Normal() {
	normal.display();
}
void ntTriCell::display(L_mode mode) {
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
void ntTriCell::displaySubD(perf_Style mode) {
	this->mode = mode;
	if (mode < 7) {
		for (int i = 0; i < polylines[mode].size(); i++) {
			polylines[mode][i]->display();
		}
	}
	//if (mode == perf_07) {
	//	faces.at(0)->set_color(ntCol4(1, 0, 0, 1));
	//	faces.at(0)->display();
	//}
	if (mode == perf_08) {
		for (int i = 1; i < 4; i++) {
			faces.at(i)->display();
		}
	}
	if (mode == perf_09) {
		for (int i = 4; i < 8; i++) {
			faces.at(i)->display();
		}
	}
	if (mode == perf_10) {
		for (int i = 7; i < 8; i++) {
			faces.at(i)->display();
		}
	}
}