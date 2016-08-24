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
	double radius;
	int sides;

	void init();
protected:
	bool isClosed = true;

public:

	ntPolygon();
	ntPolygon(const ntVec3& pos);
	ntPolygon(float radius, int sides);
	ntPolygon(const ntVec3& pos, float radius, int sides);
};
#endif