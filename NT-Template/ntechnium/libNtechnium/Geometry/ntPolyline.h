///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////// ntPolyline.h
// openGl scratch libs							///////////////////
// Polyline Class		                        ///////////////////
// created by James Warton on 07/16/2016		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef POLYLINE_JPW_NTECHNIUM
#define POLYLINE_JPW_NTECHNIUM

#include <iostream>
#include <sstream>
#include <vector>
#include "ntVec3.h"
#include "ntVertex.h"
#include "ntEdge.h"

using namespace std;
using namespace jpw;

class ntPolyline{
private:
	ntVec3* vS;
	ntVec3* vE;
	std::vector <ntVec3*>	vecs;
	std::vector <ntVertex>	verts;
	std::vector <ntEdge>	edges;

	ntColor4f col;
	float w;							// LINE WEIGHT THICKNESS
	bool isClosed = false;

	void init();
public:

	ntPolyline();
	ntPolyline(std::vector <ntVec3*> vecs, bool isClosed = false);

	double get_Length();
	double get_SegLength(int seg);
	std::vector <ntVec3*> get_Vecs();

	void setCol(ntColor4f col);
	void setWeight(float w);
	void setClosed();

	void display(float w);
	void display();
};

inline double ntPolyline::get_Length() {
	float length = 0;
	int cnt = vecs.size();
	for (int i = 0; i < cnt; i++) {
		length += vecs[i]->distance(vecs[i + 1]);
	}
	return length;
}
inline double ntPolyline::get_SegLength(int seg) {
	double length;
	int i;
	if (seg < edges.size()) {
		i = seg;
	} else {
		i = edges.size();
		std::cout << "ERROR:: SEGMENT INDEX EXCEEDS POLYLINE SEGMENTS\n" << endl;
	}
	length = edges.at(i).getLength();
	return length;
}
inline std::vector <ntVec3*> ntPolyline::get_Vecs() {
	return vecs;
}
#endif