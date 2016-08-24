///////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////// ntCircle.h
// openGl scratch libs							///////////////////
// Circlke Class extends Polygon | BaseShape	///////////////////
// created by James Warton on 10/25/2015		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef CIRCLE_POLYGON_JPW_NTECHNIUM
#define CIRCLE_POLYGON_JPW_NTECHNIUM
#define _USE_MATH_DEFINES

#include "ntPolygon.h"
 
using namespace jpw;

class ntCircle: public ntPolygon{
private:
	int seg;		//sides
	float rad;		//radius

	void calc_area();
	void init();
public:

	ntCircle();
	ntCircle(const ntVec3& pos, float rad);
	ntCircle(const ntVec3& pos, float rad, int seg);

	ntCircle(const ntVec3& pos, float rad, ntCol4 col);
	ntCircle(const ntVec3& pos, float rad, int seg, ntCol4 col);

	void display_dots();
};
#endif
