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
	double angle_vert(int index);
	ntVec3 vec_bisect(int index);
	std::vector <ntVec3*> get_Vecs();

	void set_color(ntCol4 col);
	void set_stroke(float w);
	void close();
	void offset(float dist);

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
inline double ntPolyline::angle_vert(int index){

	if (index == vecs.size()) { index = 0;}

	double theta;
	int val_00 = index;
	int val_01 = index + 1;
	int val_02 = index - 1;

	if (index > vecs.size() || index < 0) {
		std::cout << "ERROR:: INDEX EXCEEDS VERTEX COUNT FOR POLYLINE OBJECT" << endl;
	} else {
		if (isClosed == true) {
			if (index == vecs.size() - 1){
				val_01 = 0;
			}
			if (index == 0) {
				val_01 = 1;
				val_02 = vecs.size() - 2;
			}
			ntVec3 v00, v01, v02;
			v00 = vecs.at(val_00);
			v01 = vecs.at(val_01);
			v02 = vecs.at(val_02);

			v01 -= v00;
			v02 -= v00;

			v01.unitize();
			v02.unitize();

			theta = v01.angle(&v02);
		}
	}
	return theta;

}
inline ntVec3 ntPolyline::vec_bisect(int index)
{
	if (index == vecs.size()) { index = 0; }

	ntVec3 bisect;
	int val_00 = index;
	int val_01 = index + 1;
	int val_02 = index - 1;

	if (index > vecs.size() || index < 0) {
		std::cout << "ERROR:: INDEX EXCEEDS VERTEX COUNT FOR POLYLINE OBJECT" << endl;
	}
	else {
		if (isClosed == true) {
			if (index == vecs.size() - 1) {
				val_01 = 0;
			}
			if (index == 0) {
				val_01 = 1;
				val_02 = vecs.size() - 2;
			}

			ntVec3 v00, v01, v02;
			v00 = vecs.at(val_00);
			v01 = vecs.at(val_01);
			v02 = vecs.at(val_02);

			v01 -= v00;
			v02 -= v00;

			v01.div(v01.mag());
			v02.div(v02.mag());

			bisect = (v01 + v02);
			bisect.mult(0.5);
		}
	}
	return bisect;
}

inline void ntPolyline::offset(float dist)
{
	double a = dist;
	if (isClosed == true) {
		for (int i = 0; i < vecs.size()-1; i++) {

			ntVec3 vector_bi = vec_bisect(i);
			double theta = angle_vert(i) * 0.5;
			double c = a / tan(theta);

			vector_bi.mult(c);
			vecs.at(i)->add(&vector_bi);
			if (i == 0) {
				vecs.at(vecs.size() - 1)->set(vecs.at(0));
			}
		}
	}
}
/// MOVE TO BASE SHAPE
inline std::vector <ntVec3*> ntPolyline::get_Vecs() {
	return vecs;
}
#endif