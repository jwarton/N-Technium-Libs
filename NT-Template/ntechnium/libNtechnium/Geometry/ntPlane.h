///////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////// ntMeshPts.h
// openGl scratch libs							///////////////////
// Mesh Plane									///////////////////
// created by James Warton on 04/05/2014		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef PLANE_JPW_NTECHNIUM
#define PLANE_JPW_NTECHNIUM

#include <iostream>
#include "ntBaseShape.h"
#include "ntBasePhysics.h"

class ntPlane : public BaseShape {
    int U;
	int V;

	//ntParticle* corners[4];

public:
	ntPlane();
	ntPlane(const ntVec3& pos, const float&  dimX, const float&  dimY, int U, int V);
	ntPlane(const ntVec3& pos, const ntVec3&  rot, const ntVec3&  dim, int U, int V);
	ntPlane(const ntVec3& pos, const ntVec3&  rot, const ntVec3&  dim, const ntColor4f& fillCol, const ntColor4f& strokeCol);

	void setColor(ntColor4f col);

    void display();
    void displayNorms(float len);
	void displayVerts(float dim);
	void displayEdges(float w);

	void displayNorms();
	void displayVerts();
	void displayEdges();

	///////////////////////////////////////////////////////////
	//////////////////////////////////////// PHYSICS OPERATIONS
	bool enablePhysics = false;	//unique constructor enables physics
	std::vector <Particle*> particles;  //node with particle dynamics
	std::vector <Particle*> corners;
	std::vector <Spring*> springs;		//branch with spring dynamics
	

	void addPhysics(bool enable);
	void addSpring(Particle* p0, Particle* p1);

	void fixCorners();

	std::vector<Particle*>* getParticles();
	std::vector<Particle*>* getCorners();
	std::vector<Spring*>* getSprings();

private:
    void init();
};

inline std::vector<Particle*>* ntPlane::getParticles(){
	//std::vector<Particle*>* particlePtrs = &particles;
	return &particles;
}

inline std::vector<Particle*>* ntPlane::getCorners(){
	//std::vector<Particle*>* particlePtrs = &corners;
	return &corners;//particlePtrs;
}

inline std::vector<Spring*>* ntPlane::getSprings() {
	//std::vector<Spring*>* springPtrs = &springs;
	return &springs; //springPtrs;
}

inline void ntPlane::fixCorners(){
	for (int i = 0; i <4; i++){
		corners.at(i)->fixed = true;
	}
}
#endif 