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
	float get_edge_min();

	bool pt_isInside(ntVec3* point);

	void set_color(ntCol4 col);
	void set_stroke(float w);
	void close();
	void offset(double dist);

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

	if (index == vecs.size()-1) { index = 0;}

	double theta;
	int val_00 = index;
	int ind_1 = index + 1;
	int ind_2 = index - 1;

	ntVec3* p0 = new ntVec3(0, 0, 0);
	ntVec3* d1 = new ntVec3(0, 0, 0);
	ntVec3* d2 = new ntVec3(0, 0, 0);

	if (index > vecs.size() || index < 0) {
		std::cout << "ERROR:: INDEX EXCEEDS VERTEX COUNT FOR POLYLINE OBJECT" << endl;
	} else {
		if (isClosed == true) {

			if (index == vecs.size() - 2){
				ind_1 = 0;
			}
			if (index == 0) {
				ind_2 = vecs.size() - 2;
			}
			p0->set(vecs[index]);
			d1->set(vecs[ind_1]);
			d2->set(vecs[ind_2]);

			d1->sub(p0);
			d2->sub(p0);

			d1->unitize();
			d2->unitize();

			theta = d1->angle(d2);
		}
	}
	delete p0, d1, d2;
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

inline void ntPolyline::offset(double dist)
{
	double a = dist * 2;
	if (isClosed == true) {
		std::vector <ntVec3*> v_temp;
		for (int i = 0; i < vecs.size()-1; i++) {

			ntVec3* vector_bi = new ntVec3(0,0,0);
			vector_bi->set(&vec_bisect(i));
			double theta = angle_vert(i);// *0.5;
			double c = a / sin(theta);
			vector_bi->mult(c);
			v_temp.push_back(vector_bi);
		}
		for(int i = 0; i < vecs.size() - 1; i++){
			vecs.at(i)->add(v_temp[i]);
			if (i == 0) {
				vecs.at(vecs.size() - 1)->set(vecs.at(0));
			}
		}
		for (int i = 0; i < vecs.size() - 1; i++) {
			std::cout << vecs.at(i)->y << endl;
		}
	}
}
/// MOVE TO BASE SHAPE
inline std::vector <ntVec3*> ntPolyline::get_Vecs() {
	return vecs;
}
inline float ntPolyline::get_edge_min()
{
	float lenMin = 999999999999;
	float d2;
	ntVec3* v00 = new ntVec3();
	ntVec3* v01 = new ntVec3();
	int index;
	int cnt = edges.size();

	for (int i = 0; i < cnt; i++) {
		v00->set(edges[i].v0);
		v01->set(edges[i].v1);
		d2 = v00->distSqrd(v01);

		if (d2 < lenMin) {
			lenMin = d2;
			index = i;
		}
	}
	delete v00, v01;
	//SET LENGTH TO ACTUAL EDGE LENGTH
	lenMin = edges[index].getLength();
	return lenMin;
}
inline bool ntPolyline::pt_isInside(ntVec3 * point)
{
	int i, j, nvert = vecs.size()-1;

	bool c = false;
	if (isClosed) {
		for (i = 0, j = nvert - 1; i < nvert; j = i++) {
			if (((vecs[i]->y >= point->y) != (vecs[j]->y >= point->y)) &&
				(point->x <= (vecs[j]->x - vecs[i]->x) * (point->y - vecs[i]->y) / (vecs[j]->y - vecs[i]->y) + vecs[i]->x)
				)
				c = !c;
		}
	}
	return c;
}
#endif