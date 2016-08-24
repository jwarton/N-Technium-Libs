///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// ntSquare.h
// openGl scratch libs							///////////////////
// Square Class extends Rectangle | BaseShape	///////////////////
// created by James Warton on 08/24/2016		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef SQUARE_RECTANGLE_JPW_NTECHNIUM
#define SQUARE_RECTANGLE_JPW_NTECHNIUM

#include "ntRectangle.h"

class ntSquare: public ntRectangle {
public:

	ntSquare();
	ntSquare(float dim);
	ntSquare(const ntVec3& pos, float dim);

	ntSquare(float dim, ntCol4 color);
	ntSquare(const ntVec3& pos, float dim, ntCol4 color);
};
#endif