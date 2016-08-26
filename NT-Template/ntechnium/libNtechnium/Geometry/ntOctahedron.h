///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////// ntOctahedron.h
// openGl scratch libs							///////////////////
// ntOctahedron.h extends ntBrep | ntBaseShape	///////////////////
// created by James Warton on 08/26/2014		///////////////////
///////////////////////////////////////////////////////////////////
#ifndef OCTAHEDRON_JPW_NTECHNIUM
#define OCTAHEDRON_JPW_NTECHNIUM
#define _USE_MATH_DEFINES

#include "ntBaseSolid.h"

class ntOctahedron : public ntBrep {
private:

	void init();

public:
    ntOctahedron();
	ntOctahedron(const ntVec3& pos, float rad);
    void display();
};
#endif 