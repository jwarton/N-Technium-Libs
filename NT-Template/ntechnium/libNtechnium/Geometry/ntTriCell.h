///////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////// ntTriCell.h
// openGl scratch libs							///////////////////
// Triangle Cell Class	                        ///////////////////
// created by James Warton on 06/26/2016		///////////////////
///////////////////////////////////////////////////////////////////
#ifndef TRICELL_JPW_NTECHNIUM
#define TRICELL_JPW_NTECHNIUM

#include <iostream>
#include <vector>
#include <array>

#include "ntVec3.h"
#include "ntMatrix4.h"
#include "ntVertex.h"
#include "ntEdge.h"
#include "ntPolyline.h"
#include "ntNormal.h"
#include "ntFace3.h"
#include "ntMath.h"
using namespace jpw;

enum perf_Style {perf_00 , perf_01, perf_02, perf_03, perf_04, perf_05, perf_06, perf_07, perf_08, perf_09, perf_10};
enum display_Mode {EDGE, FACE, VERTEX};

class ntTriCell{
private:
	void init();

	ntVec3 *v0, *v1, *v2;

	ntVec3 *vecs[3];
	ntVec3 *uvw0, *uvw1, *uvw2;
	ntVec3 *uvws[3];
	ntVertex *vert0, *vert1, *vert2;
	
	std::vector<ntVertex*> verts;
	std::vector<ntEdge> edges;
	std::vector<ntFace3*> faces;
	std::vector<std::vector <ntPolyline*>> polylines;

	ntColor4f col;
	std::vector<ntCol4> cols;

	ntVec3	 norm;
	ntVec3  *cent;
	ntVertex centroid;
	ntNormal normal;

	double scFx = 0.5;

	void calc_EdgeMid();
	void calc_innerTri();
	void calc_starPts();
	void calc_Faces();

	void setPolylines();
	ntPolyline* add_Polyline(std::vector<int> index);

	display_Mode viewMode = VERTEX;
	perf_Style mode = perf_03;
	///////////////////////////////////////////////////////////////
	//perf_Style mode = perf_00;		// FULL FACE
	//perf_Style mode = perf_01;		// 3 POINT STAR OPEN CELL
	//perf_Style mode = perf_02;		// 3 POINT STAR OPEN CENTER
	//perf_Style mode = perf_03;		// 3 POINT STAR 
	//perf_Style mode = perf_04;		// CENTER FACES
	//perf_Style mode = perf_05;		// OUTER TRI OUTLINE
	//perf_Style mode = perf_06;		// BISECTOR EDGES
	//perf_Style mode = perf_07;		// BISECTOR ANGLES
	//perf_Style mode = perf_08;		// INNER TRI OUTLINE
	//perf_Style mode = perf_09;		// 3 POINT STAR OUTLINE
	//perf_Style mode = perf_09;		// RIGHT HAND 3 PT

public:

	ntTriCell();
	ntTriCell(ntVec3* v0,ntVec3* v1,ntVec3* v2);
	ntTriCell(ntFace3* face);

	bool pt_isInside(ntVec3* vec);
	void set_color(ntColor4f col);
	void setUVW(ntVec3* uvw0, ntVec3* uvw1, ntVec3* uvw2);
	void setUVW(std::vector <ntVec3*>	uvws);
	void setScale(float scFx);


	ntVec3* getCentroid();
	std::vector<std::vector <ntPolyline*>> get_Polylines();
	std::vector<ntVec3 *> vecs_SD;

	void calcNorm();
	void calcCentroid();

	void display();
	void display_Verts();
	void display_Edges();
	void display_Centroid();
	void display_Normal();
	void display(L_mode mode);
	void displaySubD(perf_Style mode);
};
inline std::vector<std::vector <ntPolyline*>> ntTriCell::get_Polylines() {
	return polylines;
}
inline ntVec3* ntTriCell::getCentroid(){
	return cent;
}
#endif
