///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// ntTetra.h
// openGl scratch libs							///////////////////
// ntTetra.h extends ntBaseShape.h				///////////////////
// created by James Warton on 02/23/2014		///////////////////
///////////////////////////////////////////////////////////////////
#ifndef TETRAHEDRON_JPW_NTECHNIUM
#define TETRAHEDRON_JPW_NTECHNIUM
#define _USE_MATH_DEFINES

#include "ntBaseSolid.h"

class ntTetra : public ntBrep {
private:

	void init();

public:
    ntTetra();
	ntTetra(const ntVec3& pos, float rad);
    void display();
};
#endif 