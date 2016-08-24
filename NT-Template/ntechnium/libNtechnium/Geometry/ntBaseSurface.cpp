#include "ntBaseShape.h"
#include "ntBaseSolid.h"

ntBrep::ntBrep(){
}

bool ntBrep::pt_isInside(ntVec3 * point)
{
	/// VALIDATE POINTS ON BOUNDARY
	/// ENSURE FACE WINDING ORDER IS CONSISTENT

	bool test = false;
	int cnt = 0;

	for (int i = 0; i < faces.size(); i++) {
		ntVec3 norm = faces.at(i).getNorm();
		ntVec3 cent = faces.at(i).getCent();
		ntVec3 v0 = cent - point;

		v0.unitize();
		norm.unitize();

		ntVec3 v1 = v0 - norm;
		double len = v1.magSqrd();

		if (len > 2) {
			cnt += 1;
		}
	}
	if (cnt <= 0) {
		test = true;
	}
	return test;
}
