///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////// ntBaseShape.h
// openGl scratch libs							///////////////////
// BaseShape Class for all derived shape class	///////////////////
// created by James Warton on 02/23/2014		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef BASESHAPE_JPW_NTECHNIUM
#define BASESHAPE_JPW_NTECHNIUM

#include <iostream>
#include <vector>
#include "ntTup3i.h"
#include "ntVertex.h"
#include "ntFace3.h"

using namespace std;
using namespace jpw;
class BaseShape {
    
protected:
    ntVec3 pos;
    ntVec3 dim, rot;
    ntColor4f fillCol, strokeCol;
    
    virtual void init() = 0;
 
public:
	std::vector<ntVec3*> vecs;
	std::vector<ntVertex*> verts;
    std::vector<ntTup3i> inds;
	std::vector<ntEdge> edges;
    std::vector<ntFace3> faces;

	std::vector<ntVec3*>* vecsPtr;

    BaseShape();
	BaseShape(const ntVec3& pos);
	BaseShape(const ntVec3& pos, const ntVec3&  rot, const ntVec3&  dim);
    BaseShape(const ntVec3& pos, const ntVec3&  rot, const ntVec3&  dim, const ntColor4f& fillCol, const ntColor4f& strokeCol);
	//used by derived class mesh 
	BaseShape(std::vector<ntVec3*>* vecsPtr);

	//virtual ~BaseShape();
    virtual void move();
    virtual void display() = 0;
    virtual void displayNorms(float len = 3)=0;

    void setPos(const ntVec3& pos);
    ntVec3 getPos() const;

	///add init function to generate vec* pairs
	///add function that generates collection particles from vecs
	///add function that generates collection of springs from vec pairs
};

inline void BaseShape::setPos(const ntVec3& pos){
    this->pos = pos;
}

inline ntVec3 BaseShape::getPos() const{
    return pos;
}
#endif