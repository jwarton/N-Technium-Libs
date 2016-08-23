///////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////// ntMeshPts.h
// openGl scratch libs							///////////////////
// Mesh Class from explicit point list			///////////////////
// created by James Warton on 04/05/2014		///////////////////
///////////////////////////////////////////////////////////////////
#ifndef MESH_PTS_JPW_NTECHNIUM
#define MESH_PTS_JPW_NTECHNIUM

#include <iostream>
#include <vector>
#include "ntBaseShape.h"
#include "ntMatrix4.h"
#include "ntBasePhysics.h"

using namespace std;

namespace jpw{
	class ntMeshPts;
	typedef ntMeshPts MeshPts;

	class ntMeshPts : public BaseShape {
	private:
		void init();
		int U;
		int V;

		std::vector <Vec3*>* vecsV = new vector<Vec3*>();
		std::vector <Vec3*>* vecsN = new vector<Vec3*>();
	public:

		using BaseShape::BaseShape;///may not be required
		///////////////////////////////////////////////////////////
		////////////////////////////////////////////// CONSTRUCTORS
		ntMeshPts();
		ntMeshPts(std::vector<ntVec3*>* vecsPtr);
		ntMeshPts(std::vector<ntVec3*>* vecsPtr, int U, int V);

		void update();	///TODO
		void display();
		void display_verts();
		void display_verts(float dim);
		void display_norms();
		void display_norms(float len);
		void display_edges();
		void display_edges(float w);

		std::vector<ntVec3*>* getVecs();
		///////////////////////////////////////////////////////////
		//////////////////////////////////////// PHYSICS OPERATIONS
		bool enablePhysics = false;	//unique constructor enables physics
		std::vector <Particle*> particles;  //node with particle dynamics
		std::vector <Spring*> springs;		//branch with spring dynamics

		void addPhysics(bool enable);
		void addSpring(Particle* p0, Particle* p1);

		std::vector<Particle*>* getParticles();
		std::vector<Spring*>* getSprings();

		///////////////////////////////////////////////////////////
		/////////////////////////////////////////// MESH OPERATIONS
		///TODO FUNCTIONALITY
		void displayQuad();
		void relax();
		void subdivide(enum MeshType);

		enum MeshType {
			CATMULL_CLARK,
			MID_EDGE,
			VERT_CENTROID,
		};
	};
}

inline std::vector<ntVec3*>* ntMeshPts::getVecs() {
	std::vector<ntVec3*>* vecsPtr = &vecs;
	return vecsPtr;
}

inline std::vector<Particle*>* ntMeshPts::getParticles(){
	std::vector<Particle*>* particlePtrs = &particles;
	return particlePtrs;
}

inline std::vector<Spring*>* ntMeshPts::getSprings() {
	std::vector<Spring*>* springPtrs = &springs;
	return springPtrs;
}

#endif

/// This would create a COPY of the vector
// that would be local to this function's scope
//void tester(std::vector<Item*>);

// This would use a reference to the vector
// this reference could be modified in the
// tester function
/// This does NOT involve a second copy of the vector
///void tester(std::vector<Item*>&);

// This would use a const-reference to the vector
// this reference could NOT be modified in the
// tester function
/// This does NOT involve a second copy of the vector
//void tester(const std::vector<Item*>&);

// This would use a pointer to the vector
/// This does NOT involve a second copy of the vector
// caveat:  use of raw pointers can be dangerous and 
// should be avoided for non-trivial cases if possible
//void tester(std::vector<Item*>*);