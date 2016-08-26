///////////////////////////////////////////////////////////////////
// openGl scratch libs							///////////////////
// ntCube.h extends ntBaseShape.h				///////////////////
// created by James Warton on 02/23/2014		///////////////////
///////////////////////////////////////////////////////////////////
#ifndef CUBE_JPW_NTECHNIUM
#define CUBE_JPW_NTECHNIUM
#define _USE_MATH_DEFINES

#include "ntBaseSolid.h"

class ntCube : public ntBrep {
private:

	void init();
	float width;

public:

    ntCube();
	ntCube(const ntVec3& pos, float width);
    void display();
};
#endif 