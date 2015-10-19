///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// ntTree.cpp
// openGl scratch libs							///////////////////
// Recursive Branching Class					///////////////////
// created by James Warton on 09/05/2013		///////////////////
///////////////////////////////////////////////////////////////////

#include "ntTree.h"

ntTree::ntTree(){
}
ntTree::ntTree(Vec3* pos0, float len0, int genTotal, bool enablePhysics) :
pos0(pos0), len0(len0), genTotal(genTotal), enablePhysics(enablePhysics){
	dir0 = Vec3(0, 0, 1);
	setMutations(5, 3, 5, .75);
}
ntTree::ntTree(Vec3* pos0, Vec3 dir0, float len0, int genTotal):
pos0(pos0),dir0(dir0),len0(len0),genTotal(genTotal){
	setMutations(5, 3, 5, .75);
}
ntTree::ntTree(Vec3* pos0, Vec3 dir0, float len0, int genTotal, bool enablePhysics) :
pos0(pos0), dir0(dir0), len0(len0), genTotal(genTotal), enablePhysics(enablePhysics){
	setMutations(5, 3, 5, .75);
}
ntTree::ntTree(Vec3* pos0, Vec3 dir0, float len0, int genTotal, bool randomize, bool enablePhysics) :
pos0(pos0), dir0(dir0), len0(len0), genTotal(genTotal), randomize(randomize), enablePhysics(enablePhysics){
}
void ntTree::init(){
	dim = .2;
	w = .2;
	dir0.unitize();

	srand(static_cast <unsigned> (time(0)));
	ntVec3* pos = new ntVec3(pos0->x, pos0->y, pos0->z);
	vecs.push_back(pos);
	addNode(pos, 1);
	
	if (enablePhysics == true){
		Particle* p0 = new Particle(pos);
		if (isFixedRoot == true){
			p0->fixed = true;
		}
		particles.push_back(p0);
		recTree(pos, dir0, len0, 1, p0);
	} else {
		recTree(pos, dir0, len0, 1);
	}
}

void ntTree::addNode(Vec3* posE, float size){
	ntVertex* vert = new ntVertex(posE, size);
	verts.push_back(vert);
}

void ntTree::addBranch(Vec3* posS, Vec3* posE){
	ntEdge* line = new ntEdge(posS, posE);
	branches.push_back(line);	//add branch to collection
	addNode(posE, 1);
}

void ntTree::addSpring(Particle* p0, Particle* p1){
	float r = restLen * p0->distanceTo(*p1);	//rest length
	float ks = kFx;								// spring constant | set relative to environmental forces
	float d = dFx;								// damping range:  0-1 | where 1= critical damping or 0 bounce from l to rl 
												// values < 0 takes longer to balance
												// vlaues > 1 takes longer to reach rest length

	Spring* spring = new Spring(*p0, *p1, ks, d, r);
	springs.push_back(spring);					//add branch to collection
}

void ntTree::recTree(Vec3* posS, Vec3 dir, float length, int gen, Particle* particle){

	int cnt;
	if (gen <= genTotal){

		if (gen < genTotal){
			cnt = branchCnt;
		} else {
			cnt = 0;
		}
		if (gen == genTotal-1){
			cnt = leafCnt;
		}
		
		Vec3* posE = new Vec3(posS->x, posS->y, posS->z);
		posE->translate(posS, &dir, length);
		vecs.push_back(posE);
		addBranch(posS, posE);
		///////////////////////////////////////////////////particle spring implementation
		Particle* p1 = new Particle(posE);
		if (enablePhysics == true){
			if (isFixedBase == true & gen == 0){
				p1->fixed = true;
			}
			if (isFixedLeaves == true & gen == genTotal){
				if (isLeafSet){
					posE->z = leaf_Z;		//end node of leaf element
				}
				p1->fixed = true;			//end node of leaf element
			}
			particles.push_back(p1);
			addSpring(particle, p1);
		}
		///////////////////////////////////////////////////node branch construction
		for (int i = 0; i < cnt; i++){
			//set end position based on direction of growth vector and length
				if (randomize == true){
					//set range for randomizations
					rT = minT + float(((maxT - minT)* rand()) / static_cast <float> (RAND_MAX + 1.0));
					rP = minP + float(((maxP - minP)* rand()) / static_cast <float> (RAND_MAX + 1.0));
					rL = minL + float(((maxL - minL)* rand()) / static_cast <float> (RAND_MAX + 1.0));
				}

				//set rotation angles
				float Theta = (((360 / cnt)*i) + rT) * (M_PI / 180);
				float Phi = (phi + rP) * (M_PI / 180);

				//randomize direction vector
				float len = (length * lenFx) * rL;

				//new direction vector
				Vec3 dirN = Vec3(dir.x, dir.y, dir.z);
				dirN.unitize();
				float vX = 0;
				float vY = 0;
				float vZ = 0;

				//vector perpendicular to parent branch direction
				if (dir.x != 0 && dir.y != 0){
					vX = dir.x;// 0;
					vY = dir.y;// 0;
					vZ = 0;// (dir.x + dir.y) / dir.z;
				}
				else {
					vX = 1;
					vY = 0;
					vZ = 0;
				}

				Vec3 rotAxis = dirN.cross(&Vec3(vX, vY, vZ));	//vector to find with product
				rotAxis.unitize();								//vector perpendicular to direction

				//rotation transforms
				ntMatrix4 phiR = ntMatrix4(&dirN);
				phiR.rotate3d(Phi, rotAxis);
				ntMatrix4 thetaR = ntMatrix4(&dirN);
				thetaR.rotate3d(Theta, dir);

				if (enablePhysics == true){
					recTree(posE, dirN, len, gen + 1, p1);
				} else {
					recTree(posE, dirN, len, gen + 1);
					//delete p1;
				}
		}
	}
}


void ntTree::setLeaf_Z(float z){
	this->isLeafSet = true;
	this->leaf_Z = z;
}

void ntTree::setMutations(int leafCnt, int branchCnt, float phi, float lenFx){
	this->leafCnt = leafCnt;
	this->branchCnt = branchCnt;
	this->lenFx = lenFx;
	this->phi = phi;
}
void ntTree::setRandomize(float minT, float maxT, float minP, float maxP, float minL, float maxL){
	randomize = true;
	//set range for randomizations
	this->minT = minT;
	this->maxT = maxT;
	this->minP = minP;
	this->maxP = maxP;
	this->minL = minL;
	this->maxL = maxL;
}
void ntTree::setDynamics(float restLen, float dFx, float kFx){
	this->restLen = restLen;
	this->dFx =	dFx;
	this->kFx = kFx;
}
void ntTree::setFixedRoot(bool isFixed){
	isFixedRoot = isFixed;
}
void ntTree::setFixedBase(bool isFixed){
	isFixedBase = isFixed;
}
void ntTree::setFixedLeaves(bool isFixed){
	isFixedLeaves = isFixed;
}

void ntTree::display(){
	displayVerts();
	displayBranches();
}
void ntTree::displayVerts(){
	for (int i = 0; i<verts.size(); ++i){
		verts.at(i)->display();
	}
}
void ntTree::displayVerts(float dim){
	for (int i = 0; i<verts.size(); ++i){
		verts.at(i)->setSize(dim);
		verts.at(i)->display();
	}
}
void ntTree::displayBranches(){
	for (int i = 0; i < branches.size(); ++i){
		branches.at(i)->display();
	}
}
void ntTree::displayBranches(float w){
	for (int i = 0; i < branches.size(); ++i){
		branches.at(i)->display(w);
	}
}

//srand and random number generator
//http://stackoverflow.com/questions/686353/c-random-float-number-generation
