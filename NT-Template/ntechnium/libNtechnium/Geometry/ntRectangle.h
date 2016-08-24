///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////// ntRectangle.h
// openGl scratch libs							///////////////////
// Rectangle Class extends Polygon | BaseShape	///////////////////
// created by James Warton on 08/24/2016		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef RECTANGLE_JPW_NTECHNIUM
#define RECTANGLE_JPW_NTECHNIUM

#include "ntPolygon.h"

class ntRectangle : public ntPolygon{
protected:
	void init();
	double dimX, dimY;

public:

	ntRectangle();
	ntRectangle(float dimX, float dimY);
	ntRectangle(const ntVec3& pos, float dimX, float dimY);
};
#endif