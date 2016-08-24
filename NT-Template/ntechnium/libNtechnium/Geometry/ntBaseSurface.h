///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////// ntBaseShape.h
// openGl scratch libs							///////////////////
// BaseSolid Class for all derived geom class	///////////////////
// created by James Warton on 08/24/2016		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef BREP_SOLID_JPW_NTECHNIUM
#define BREP_SOLID_JPW_NTECHNIUM
#define _USE_MATH_DEFINES

#include <iostream>
#include "ntBaseShape.h"

class ntBrep : public BaseShape {
private:

	double vol;
	ntVec3 cent;
	virtual void init() = 0;

public:
    ntBrep();

	///////////////////////////////////////////////////////////////
	///////////////////////////////////////////// UTILITY FUNCTIONS
	double volume();
	ntVec3 centroid();

	bool pt_isInside(ntVec3* point);
	bool pt_isOnFace(ntVec3* point);

};
#endif 