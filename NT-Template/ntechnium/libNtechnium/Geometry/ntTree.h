///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////// ntTree.h
// openGl scratch libs							///////////////////
// Recursive Branching Class					///////////////////
// created by James Warton on 09/05/2013		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef RECURSIVE_TREE_JPW_NTECHNIUM
#define RECURSIVE_TREE_JPW_NTECHNIUM
#define _USE_MATH_DEFINES

#include <iostream>
#include <vector>
#include <cstdlib>	//for srand()
#include <ctime>	//for time()

#include "ntIncludeLibs.h"
#include "ntBasePhysics.h"

class ntTree;
typedef ntTree Tree;

class ntTree{
private:
	Col4 col;		//global color overwrite
	float dim;		//vertex size
	float w;		//global branch thickness

	Vec3* pos0;		//base position of branch1
	Vec3  dir0;		//base vector direction branch1
	float len0;		//base branch length

	int genTotal;	//growth generations

	int branchCnt = 3;	//new branches at current node
	int leafCnt = 5;	//leaves at final branch

	float theta;	//rotation about current node vector
	float phi;		//rotation from current vector
	float lenFx;	//percentage of previous generations length

	bool randomize = false;
	float rT = 0;	//summation
	float rP = 0;	//summation
	float rL = 1;	//percentage

	//set range for randomizations
	float minT = 5;		//theta randomization
	float maxT = 15;
	float minP = 5;		//phi randomization
	float maxP = 15;
	float minL = 0.95;	//length randomization
	float maxL = 1.25;

	//physics variables
	bool enablePhysics = false;	//unique constructor enables physics
	bool isFixedRoot = true;
	bool isFixedBase = true;
	bool isFixedLeaves = false;
	bool isLeafSet = false;
	
	float leaf_Z;

	float restLen = 0.9;
	float dFx = 0.6;
	float kFx = 0.2;
	enum physics;

public:
	//each generation pushes to these collections
	//pointer to collection of vecs
	//allows all vecs to be passed to external class w/o copy
	std::vector <Vec3*> vecs;			//nodes
	std::vector <vertex*> verts;		//nodes
	std::vector <ntEdge*> branches;		//branches

	std::vector <Particle*> particles;  //node with particle dynamics
	std::vector <Spring*> springs;		//branch with spring dynamics

	ntTree();
	ntTree(Vec3* posS, float length, int gen, bool enablePhysics);
	ntTree(Vec3* posS, Vec3 dir, float length, int gen);
	ntTree(Vec3* posS, Vec3 dir, float length, int gen, bool enablePhysics);
	ntTree(Vec3* posS, Vec3 dir, float length, int gen, bool randomize, bool enablePhysics);

	void addNode(Vec3* posE, float size);
	void addBranch(Vec3* posS, Vec3* posE);
	void addSpring(Particle* p0, Particle* p1);
	void recTree(Vec3* posS, Vec3 dir, float length, int gen, Particle* particle = nullptr);
	void init();

	void setLeaf_Z(float z);

	void setMutations(int leafCnt, int branchCnt, float phi, float lenFx);
	void setRandomize(float minT, float maxT, float minP, float maxP, float minL, float maxL);
	void setDynamics(float restLen, float dFx, float kFx);
	void setFixedRoot(bool isFixed);
	void setFixedBase(bool isFixed);
	void setFixedLeaves(bool isFixed);

	void display();
	void displayVerts();
	void displayVerts(float dim);
	void displayBranches();
	void displayBranches(float w);

	std::vector<ntVec3*>* getVecs();
	std::vector<ntEdge*>* getEdges();

	std::vector<Particle*>* getParticles();
	std::vector<Spring*>* getSprings();
};

inline std::vector<ntVec3*>* ntTree::getVecs() {
	std::vector<ntVec3*>* vecsPtr = &vecs;
	return vecsPtr;
}

inline std::vector<ntEdge*>* ntTree::getEdges() {
	std::vector<ntEdge*>* edgePtrs = &branches;
	return edgePtrs;
}

inline std::vector<Particle*>* ntTree::getParticles(){
	std::vector<Particle*>* particlePtrs = &particles;
	return particlePtrs;
}

inline std::vector<Spring*>* ntTree::getSprings() {
	std::vector<Spring*>* springPtrs = &springs;
	return springPtrs;
}
#endif