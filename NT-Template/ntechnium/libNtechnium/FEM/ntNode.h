///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////// ntNode_arm.h
// openGl scratch libs							///////////////////
// Element Class								///////////////////
// created by James Warton on 11/04/2014		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef FEM_NODE_JPW_NTECHNIUM
#define FEM_NODE_JPW_NTECHNIUM

#include <iostream>
#include <sstream>
#include <vector>
#include <array>
#include "ntVec3.h"
#include "ntVertex.h"
#include "ntEdge.h"

using namespace jpw;

class ntTruss;///////////////////////////////// FORWARD DECLARE
class ntElement;

enum BC_type { FIX, X, Y, Z, U, V, W, PIN, FREE };

class ntNode : public ntVertex{
private:

protected:
	std::string id = "node_";
	std::string id_i;			//identification
	Vec3 posI;					//position initial 
	Vec3 posD;					//position after displacement
	Vec3 Q;						//nodal displacement: (x,y,z)
	Vec3 R;						//nodal rotation: (u,v,w)
	Vec3 F;						//force applied at node
	Vec3 M;						//moment applied at node

	int maxDOF;					//set value from element init()
	int dof = 3;				//degrees of freedom
	int bc = 0;					//bc = dof - fixed conditions

	void def_BC();
	/////////////////////////// MEMBERS ACCESSED BY FRIEND CLASS
	bool free = true;
	bool fixed = false;
	bool fixed_X = false;
	bool fixed_Y = false;
	bool fixed_Z = false;
	bool fixed_U = true;
	bool fixed_V = true;
	bool fixed_W = true;

public:
	friend class ntTruss;
	friend class ntElement;

	ntNode(ntVec3* pos);
	ntNode(ntVec3* pos, int dof); ///TEMPORARY SOLUTION FOR FRAME ELEMENTS
	ntNode(ntVec3* pos, ntCol4 col);

	void set_BC(BC_type n);
	void set_FC(ntVec3  f);
	void set_MC(ntVec3  m);

	void reset();
	
	int get_DOF();
	int get_BC();

	bool is_Free();
};
inline ntNode::ntNode(ntVec3* pos) :
ntVertex(pos){
	posI.x = pos->x;
	posI.y = pos->y;
	posI.z = pos->z;
	maxDOF = 3;
}
inline ntNode::ntNode(ntVec3* pos, int dof) :
ntVertex(pos){
	posI.x = pos->x;
	posI.y = pos->y;
	posI.z = pos->z;
	maxDOF = dof;
}
inline ntNode::ntNode(ntVec3* pos, ntCol4 col):
ntVertex(pos, col){
	posI.x = pos->x;
	posI.y = pos->y;
	posI.z = pos->z;
	maxDOF = 3;
}
inline void ntNode::set_BC(BC_type n){
	if (n == FREE){
		free = true;
		fixed = false;
		fixed_X = false;
		fixed_Y = false;
		fixed_Z = false;
		if (maxDOF == 6){
			fixed_U = false;
			fixed_V = false;
			fixed_W = false;
		}
	} else {
		free = false;
		if (n == FIX){
			fixed = true;
			fixed_X = true;
			fixed_Y = true;
			fixed_Z = true;
			fixed_U = true;
			fixed_V = true;
			fixed_W = true;
		}
		if (n == PIN){
			fixed = false;
			fixed_X = true;
			fixed_Y = true;
			fixed_Z = true;
			fixed_U = false;
			fixed_V = false;
			fixed_W = false;
		}
		if (n == X){ fixed_X = true; }
		if (n == Y){ fixed_Y = true; }
		if (n == Z){ fixed_Z = true; }
		if (n == U){ fixed_U = true; }
		if (n == V){ fixed_V = true; }
		if (n == W){ fixed_W = true; }
	}
	def_BC();
}
inline void ntNode::set_FC(ntVec3 F){
	this->F = F;
}
inline void ntNode::set_MC(ntVec3 M){
	this->M = M;
}
inline void ntNode::def_BC(){
	dof = maxDOF;
	if (free == false){
		if (fixed == true){
			dof = 0;
		} else {
			if (fixed_X == true){
				dof -= 1;
			}
			if (fixed_Y == true){
				dof -= 1;
			}
			if (fixed_Z == true){
				dof -= 1;
			}
			if (fixed_U == true && maxDOF == 6){
				dof -= 1;
			}
			if (fixed_V == true && maxDOF == 6){
				dof -= 1;
			}
			if (fixed_W == true && maxDOF == 6){
				dof -= 1;
			}
		}
	}
}
inline void ntNode::reset(){
	free = true;
	fixed = false;
	fixed_X = false;
	fixed_Y = false;
	fixed_Z = false;
}
inline int ntNode::get_BC(){
	bc = maxDOF - dof;
	return bc;
}
inline int ntNode::get_DOF(){
	def_BC();
	return maxDOF;
}
inline bool ntNode::is_Free(){
	return free;
}
#endif

