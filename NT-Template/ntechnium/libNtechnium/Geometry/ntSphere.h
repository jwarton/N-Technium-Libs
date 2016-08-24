// ntSphere.h extends ntBaseShape.h
// openGl scratch libs
// created by James Warton on 02/23/2014

#ifndef SPHERE_JPW_NTECHNIUM
#define SPHERE_JPW_NTECHNIUM
#define _USE_MATH_DEFINES

#include "ntBaseSolid.h"

class ntSphere : public ntBrep {
    int U;
	int V;
public:

    ntSphere();
	ntSphere(const ntVec3& pos, const float&  rad, int U, int V);
	ntSphere(const ntVec3& pos, const ntVec3&  rot, float rad, int U, int V);


    void display();

	void display_edges(float w);
	void display_edges();

private:
    void init();
};
#endif 