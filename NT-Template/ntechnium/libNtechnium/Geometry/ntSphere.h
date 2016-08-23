// ntSphere.h extends ntBaseShape.h
// openGl scratch libs
// created by James Warton on 02/23/2014

#ifndef SPHERE_JPW_NTECHNIUM
#define SPHERE_JPW_NTECHNIUM
#define _USE_MATH_DEFINES

#include <iostream>
#include "ntBaseShape.h"

class ntSphere : public BaseShape {
    int U;
	int V;
public:

    ntSphere();
	ntSphere(const ntVec3& pos, const float&  rad, int U, int V);
	ntSphere(const ntVec3& pos, const ntVec3&  rot, const ntVec3&  rad, int U, int V);
    ntSphere(const ntVec3& pos, const ntVec3&  rot, const ntVec3&  rad, const ntColor4f& fillCol, const ntColor4f& strokeCol);

	void set_color(ntColor4f col);

    void display();

	void display_edges(float w);
	void display_edges();

private:
    void init();
};
#endif 