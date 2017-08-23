///////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////// ntPolygon.h
// openGl scratch libs							///////////////////
// Polygon Class extends Polyline | BaseShape	///////////////////
// created by James Warton on 08/24/2016		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef POLYGON_JPW_NTECHNIUM
#define POLYGON_JPW_NTECHNIUM

#include "ntPolyline.h"

using namespace std;

class ntPolygon : public ntPolyline{
private:

	void calc_norm();
	void calc_centroid();
	void calc_area();

	void init();
protected:
	ntPolygon(const ntVec3& pos);

	int sides;
	double radius;
	double area;
	ntVec3 norm;
	ntVec3* cent;

	ntVertex centroid;
	ntNormal normal;
public:

	ntPolygon();
	ntPolygon(float radius, int sides);
	ntPolygon(const ntVec3& pos, float radius, int sides);

	ntPolygon(float radius, int sides, ntCol4 color);
	ntPolygon(const ntVec3& pos, float radius, int sides, ntCol4 color);

	bool pt_isInside(ntVec3* point);
	double get_area();
	int get_seg_count();
};
#endif