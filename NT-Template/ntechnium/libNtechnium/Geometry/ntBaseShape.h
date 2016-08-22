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
	std::vector<ntVec3*>	vecs;
	std::vector<ntVertex*>	verts;
    std::vector<ntTup3i>	inds;
	std::vector<ntEdge>		edges;
    std::vector<ntFace3>	faces;

	std::vector<ntVec3*>*	vecsPtr;

    BaseShape();
	BaseShape(const ntVec3& pos);
	BaseShape(const ntVec3& pos, const ntVec3&  rot, const ntVec3&  dim);
    BaseShape(const ntVec3& pos, const ntVec3&  rot, const ntVec3&  dim, const ntColor4f& fillCol, const ntColor4f& strokeCol);
	
	BaseShape(std::vector<ntVec3*>* vecsPtr);	//USED BY DERIVED CLASS MESH 
	//virtual ~BaseShape();

	///////////////////////////////////////////////////////////////
	////////////////////////////////////// EUCLIDIAN TRANSORMATIONS
	void translate(ntVec3 vec);
	void translate(ntVec3 pt0, ntVec3 pt1);
	void translate(ntVec3 dir, float mag);
	void translate(ntVec3 pos, ntVec3 dir, float dist);
	void rotate_x (ntVec3 pos, float theta);
	void rotate_y (ntVec3 pos, float theta);
	void rotate_z (ntVec3 pos, float theta);
	void rotate_3d(ntVec3 axis, float theta);
	void orient   (ntVec3 axis_T, ntVec3 axis_S);

	///////////////////////////////////////////////////////////////
	///////////////////////////////////////////// DISPLAY FUNCTIONS
    virtual void display() = 0;

	void display_norms(float len = 0.5);
	void display_verts(float dim);
	void display_edges(float w);

	void display_norms();
	void display_verts();
	void display_edges();

    void setPos(const ntVec3& pos);
    ntVec3 getPos() const;

	///ADD INIT FUNCTION TO GENERATE VEC* PAIRS
	///ADD FUNCTION THAT GENERATES COLLECTION PARTICLES FROM VECS
	///ADD FUNCTION THAT GENERATES COLLECTION OF SPRINGS FROM VEC PAIRS
};

inline void BaseShape::setPos(const ntVec3& pos){
    this->pos = pos;
}

inline ntVec3 BaseShape::getPos() const{
    return pos;
}
#endif