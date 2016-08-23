// ntCube.h extends ntBaseShape.h
// openGl scratch libs
// created by James Warton on 02/23/2014

#ifndef CUBE_JPW_NTECHNIUM
#define CUBE_JPW_NTECHNIUM
#define _USE_MATH_DEFINES

#include <iostream>
#include "ntBaseShape.h"

class ntCube : public BaseShape {
private:
	void init();
	float width;
public:

    ntCube();
	ntCube(const ntVec3& pos, const float&  width);

    void display();
};
#endif 