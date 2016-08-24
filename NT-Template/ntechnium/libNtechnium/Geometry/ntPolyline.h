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
	bool isClosed = false;

	ntPolyline(const ntVec3& pos);
	void init_edges();

	ntCol4 col = ntCol4(0.25, 0.25, 0.25, 1);
	float stroke  = 0.5;
public:

	ntPolyline();
	ntPolyline(ntVec3* vS, ntVec3* vE);
	ntPolyline(std::vector <ntVec3*> vecs);
	ntPolyline(std::vector <ntVec3*> vecs, bool isClosed);

	ntPolyline(ntVec3* vS, ntVec3* vE, ntCol4 col);
	ntPolyline(std::vector <ntVec3*> vecs, ntCol4 col);
	ntPolyline(std::vector <ntVec3*> vecs, bool isClosed, ntCol4 col);

	double length();
	double length_seg(int seg);
	std::vector <ntVec3*> get_Vecs();

	void set_color(ntCol4 col);
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