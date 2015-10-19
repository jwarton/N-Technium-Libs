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
	ntSphere(const ntVec3& pos, const ntVec3&  rot, const ntVec3&  dim, int U, int V);
    ntSphere(const ntVec3& pos, const ntVec3&  rot, const ntVec3&  dim, const ntColor4f& fillCol, const ntColor4f& strokeCol);

	void setColor(ntColor4f col);

    void display();
    void displayNorms(float len);
	void displayVerts(float dim);
	void displayEdges(float w);

	void displayNorms();
	void displayVerts();
	void displayEdges();

private:
    void init();
};
#endif 