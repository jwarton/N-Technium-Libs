#include "ntConvexHull.h"

ntConvexHull::ntConvexHull(){
}

ntConvexHull::ntConvexHull(std::vector<ntVec3*>* pt_cloud):
pt_cloud(pt_cloud){	

	for (int i = 0; i < pt_cloud->size(); i++) {
		ntVertex vert(pt_cloud->at(i));
		pts.push_back(vert);
	}

	init();
}

void ntConvexHull::init(){
	int cnt = 0;

	if (pt_cloud->size() > 4) {

		mesh_addPts();

		/// INITIAL MESH

		/// REMOVE MESH VERTICES FROM LIST

		/// REMOVE POINTS INSIDE OF INITIAL MESH

		/// INCREMENTALLY ADD POINTS TO MESH

			/// REMOVE INTERIOR FACE

		/// PROVIDE VEC INDEX FOR REMAINING FACES
		inds.push_back(ntTup3i(0, 1, 2));
		inds.push_back(ntTup3i(3, 0, 1));
		inds.push_back(ntTup3i(3, 1, 2));
		inds.push_back(ntTup3i(3, 2, 0));

	} else if (pt_cloud->size() == 4) {
		mesh_addPts();
		inds.push_back(ntTup3i(0, 1, 2));
		inds.push_back(ntTup3i(3, 0, 1));
		inds.push_back(ntTup3i(3, 1, 2));
		inds.push_back(ntTup3i(3, 2, 0));
	} else if (pt_cloud->size() == 3) {
		/// DEFINE EXCEPTION TO REMOVE COLINEAR POINTS
		mesh_addPts();
		inds.push_back(ntTup3i(0, 1, 2));
		std::cout << "  ERROR:  NO POLYHEDRON WAS FORMED | INSUFFICIENT POINT DATA" << endl;
	} else if (pt_cloud->size() == 2) {
		mesh_addPts();
		std::cout << "  ERROR:  NO POLYHEDRON WAS FORMED | INSUFFICIENT POINT DATA" << endl;
	} else if (pt_cloud->size() == 1) {
		mesh_addPts();
		std::cout << "  ERROR:  NO POLYHEDRON WAS FORMED | INSUFFICIENT POINT DATA" << endl;
	}

	/////////////////////////////////////////////////////////////////////////// INSTANTIATE FACES
	for (int i = 0; i<inds.size(); ++i) {
		edges.push_back(ntEdge(vecs.at(inds.at(i).elem0), vecs.at(inds.at(i).elem1)));
	}

	/////////////////////////////////////////////////////////////////////////// INSTANTIATE FACES
	for (int i = 0; i<inds.size(); ++i) {
		faces.push_back(ntFace3(vecs.at(inds.at(i).elem0), vecs.at(inds.at(i).elem1), vecs.at(inds.at(i).elem2)));
	}
}
void ntConvexHull::mesh_addPts() {
	for (int i = 0; i < pt_cloud->size(); i++) {
		vecs.push_back(pt_cloud->at(i));
		ntVertex* vertex = new ntVertex(vecs.at(i));
		verts.push_back(vertex);
		
		pts.at(i).set_color(ntCol4(1, 0, 0, 1));
	}
}

bool ntConvexHull::pt_isInside(ntVec3 * point, ntMeshPts mesh) {
	/// VALIDATE POINTS ON BOUNDARY
	/// ENSURE FACE WINDING ORDER IS CONSISTENT

	bool test = false;
	int cnt = 0;
	
	for (int i = 0; i < mesh.faces.size(); i++) {
		ntVec3 norm = mesh.faces.at(i).getNorm();
		ntVec3 cent = mesh.faces.at(i).getCent();
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

void ntConvexHull::add_point(ntVec3 * vec)
{
	pt_cloud->push_back(vec);
	ntVertex vert(vec);
	pts.push_back(vert);
}
void ntConvexHull::add_points(std::vector<ntVec3*>* vecs)
{
	pt_cloud->insert(pt_cloud->end(), vecs->begin(), vecs->end());
	for (int i = 0; i < vecs->size(); i++) {
		ntVertex vert(vecs->at(i));
		pts.push_back(vert);
	}
}
void ntConvexHull::display() {
	for (int i = 0; i < pts.size(); ++i) {
		pts.at(i).display(1);
	}
	for (int i = 0; i<faces.size(); ++i) {
		faces.at(i).display();
	}
}
