///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////// ntIcashedron.h
// openGl scratch libs							///////////////////
// ntIcashedron.h extends ntBrep | ntBaseShape	///////////////////
// created by James Warton on 08/26/2014		///////////////////
///////////////////////////////////////////////////////////////////
#ifndef ICASHEDRON_JPW_NTECHNIUM
#define ICASHEDRON_JPW_NTECHNIUM
#define _USE_MATH_DEFINES

#include "ntBaseSolid.h"

class ntIcashedron : public ntBrep {
private:

	double g_mean = (sqrt(5) + 1) *0.5;

	void init();

public:
	ntIcashedron();
	ntIcashedron(const ntVec3& pos, float rad);
    void display();
};
#endif 