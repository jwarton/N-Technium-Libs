///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////// ntConvexHull.h
// openGl scratch libs							///////////////////
// Convex Hull Solver Class	                    ///////////////////
// created by James Warton on 08/22/2016		///////////////////
///////////////////////////////////////////////////////////////////
#ifndef CONVEX_HULL_JPW_NTECHNIUM
#define CONVEX_HULL_JPW_NTECHNIUM
#define _USE_MATH_DEFINES

#include <iostream>
#include "ntBaseSolid.h"
#include "ntMeshPts.h"

class ntConvexHull : public ntBrep {
private:
	void init();

	std::vector<ntVec3*>* pt_cloud;
	std::vector<ntVertex> pts;

	void mesh_addPts();

public:
    ntConvexHull();
	ntConvexHull(std::vector<ntVec3*>* pt_cloud);

	void solve();		/// CALCULATE CONVEX HULL FROM PT_CLOUD

	void add_point(ntVec3* vec);
	void add_points(std::vector<ntVec3*>* vecs);

	void display();
};
#endif 