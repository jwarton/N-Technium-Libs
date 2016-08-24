///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////// ntPolyline.h
// openGl scratch libs							///////////////////
// Polyline Class		                        ///////////////////
// created by James Warton on 07/16/2016		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef POLYLINE_JPW_NTECHNIUM
#define POLYLINE_JPW_NTECHNIUM

#include "ntBaseShape.h"

class ntPolyline : public BaseShape{
private:
	ntVec3* vS;
	ntVec3* vE;

	void init();

protected:
	ntPolyline(const ntVec3& pos);
	void init_edges();

	bool isClosed = false;
	ntColor4f col;
	float stroke;

public:

	ntPolyline();
	ntPolyline(ntVec3* vS, ntVec3* vE);
	ntPolyline(std::vector <ntVec3*> vecs, bool isClosed = false);

	double length();
	double length_seg(int seg);
	std::vector <ntVec3*> get_Vecs();

	void set_color(ntColor4f col);
	void set_stroke(float w);
	void close();

	void display(float w);
	void display();
};

inline double ntPolyline::length() {
	float length = 0;
	int cnt = vecs.size();
	for (int i = 0; i < cnt; i++) {
		length += vecs[i]->distance(vecs[i + 1]);
	}
	return length;
}
inline double ntPolyline::length_seg(int seg) {
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
/// MOVE TO BASE SHAPE
inline std::vector <ntVec3*> ntPolyline::get_Vecs() {
	return vecs;
}
#endif