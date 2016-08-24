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
	std::string name;
    ntVec3 pos, dim, rot;
 
    virtual void init() = 0;
 
	///ADD INIT FUNCTION TO GENERATE VEC* PAIRS
	///ADD FUNCTION THAT GENERATES COLLECTION PARTICLES FROM VECS
	///ADD FUNCTION THAT GENERATES COLLECTION OF SPRINGS FROM VEC PAIRS
public:

	std::vector<ntVec3*>*	vecsPtr;	/// MOVE TO PRIVATE
	std::vector<ntVec3*>	vecs;
    std::vector<ntTup3i>	inds;

	std::vector<ntVertex*>	verts;
	std::vector<ntEdge>		edges;
	std::vector<ntFace3>	faces;


    BaseShape();
	///CONSTRUCTORS USED BY PRIMITIVE SOLIDS
	BaseShape(const ntVec3& pos);
	BaseShape(const ntVec3& pos, const ntVec3&  rot);
	BaseShape(const ntVec3& pos, const ntVec3&  rot, const ntVec3&  dim);

	///CONSTRUCTORS USED BY ARBITRARY SHAPE CLASSES
	BaseShape(std::vector<ntVec3*> vecs);	//USED BY DERIVED CLASS MESH 
	BaseShape(std::vector<ntVec3*>* vecs);
	//virtual ~BaseShape();

	///////////////////////////////////////////////////////////////
	///////////////////////////////////////////// UTILITY FUNCTIONS
	void set_pos(const ntVec3& pos);
	void set_rot(const ntVec3& rot);

	ntVec3 get_pos() const;
	ntVec3 get_rot() const;

	std::string get_name() const;
	///////////////////////////////////////////////////////////////
	////////////////////////////////////// EUCLIDIAN TRANSORMATIONS
	void translate(ntVec3 vec);
	void translate(ntVec3 pt0, ntVec3 pt1);
	void translate(ntVec3 dir, float mag);
	void translate(ntVec3 pos, ntVec3 dir, float dist);
	void rotate_x (ntVec3 pos, float theta);
	void rotate_y (ntVec3 pos, float theta);
	void rotate_z (ntVec3 pos, float theta);
	void rotate_3D(ntVec3 axis, float theta);
	void orient   (ntVec3 axis_T, ntVec3 axis_S);

	///////////////////////////////////////////////////////////////
	//////////////////////////////////////////// DISPLAY PROPERTIES
	void set_color(ntColor4f col);
	void set_vert_color(ntColor4f col);
	void set_edge_color(ntColor4f col);
	void set_face_color(ntColor4f col);

	///////////////////////////////////////////////////////////////
	///////////////////////////////////////////// DISPLAY FUNCTIONS
    virtual void display() = 0;

	void display_norms(float len);
	void display_verts(float dim);
	void display_edges(float w);

	void display_norms();
	void display_verts();
	void display_edges();

	void print();
	void print(std::string s);
};

inline void BaseShape::set_pos(const ntVec3& pos){
    this->pos = pos;
}
inline void BaseShape::set_rot(const ntVec3& rot) {
	this->rot = rot;
}

inline ntVec3 BaseShape::get_pos() const{
    return pos;
}
inline ntVec3 BaseShape::get_rot() const {
	return rot;
}
inline std::string BaseShape::get_name() const {
	return name;
}
#endif