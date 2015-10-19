///////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////// ntElement.h
// openGl scratch libs							///////////////////
// Element Class								///////////////////
// created by James Warton on 11/04/2014		///////////////////
///////////////////////////////////////////////////////////////////
// element types:								///////////////////
// spring:	dof = 1; x_local		S1			///////////////////
// truss:	dof = 3; xyx			T3			///////////////////
// frame:	dof = 6; xyz uvw		F6			///////////////////
// plate:							Pn			///////////////////
///////////////////////////////////////////////////////////////////

#ifndef SPACE_TRUSS_ELEMENT_JPW_NTECHNIUM
#define SPACE_TRUSS_ELEMENT_JPW_NTECHNIUM

#include <iostream>
#include <sstream>
#include <vector>
#include <array>

#include <arrayfire.h>
#include <af/util.h>
#include <af/dim4.hpp>
#include <af/traits.hpp>

#include "ntVec3.h"
#include "ntNode.h"
#include "ntEdge.h"

using namespace jpw;
using namespace af;

enum FE_type { S1 = 1, T3 = 3, F6 = 6, Pn};

class ntElement: public ntEdge{
private:
	
	void init();
	void setDirection();

	FE_type FE;
	ntNode* n0;
	ntNode* n1;

	float E = 0;										//modulus of elasticity
	float I = 0;										//moment of interia
	float A = 0;										//area of cross section
	float L = 0;										//length
	float S = 0;										//stiffness factor
	Vec3  UVW;											//direction vector

	af::array ke;										//local stiffness				(E*A / L) * [i]
	af::array q = af::constant(0, 1, 2, f32);			//local displacements			{Q} * [t]
	af::array t = af::constant(0, 2, 6, f32);			//local transformation matrix	[ l m n 0 0 0; 0 0 0 l m n]
	af::array p = af::constant(0, 2, 1, f32);			//axial forces					[ke] * {q}

	af::array k = af::constant(0, 6, 6, f32);			//global element stiffness 
	af::array Q = af::constant(0, 6, 1, f32);			//global displacements
	af::array T = af::constant(0, 6, 6, f32);			//global transformation matrix
	std::array<int, 2> nodeInd;							//matrix indices for node relations


public:
	ntElement();
	ntElement(ntNode* n0, ntNode* n1, float A, float E, std::array<int, 2> nodeInd);
	ntElement(ntNode* n0, ntNode* n1, float A, float I, float E, std::array<int, 2> nodeInd);

	std::array<int, 2> get_nodeInd();

	void def_k();
	void def_p();
	void def_T();
	void def_ke(); ///FOR BEAMS ONLY

	Vec3 getDirection();
	af::array get_k();
	af::array get_p();
	float get_E();
	float get_A();

	//set during design iterations
	void set_A(float a);
};
inline std::array<int, 2> ntElement::get_nodeInd(){
	return nodeInd;
}

inline Vec3  ntElement::getDirection() {
	setDirection();
	return UVW;
}
inline float ntElement::get_E(){
	return E;
}
inline float ntElement::get_A(){
	return A;
}
inline af::array ntElement::get_k(){
	return k;
}
inline af::array ntElement::get_p(){
	def_p();
	return p;
}
inline void ntElement::setDirection() {
	L = getLength();
	UVW.x = (v1->x - v0->x) / L;
	UVW.y = (v1->y - v0->y) / L;
	UVW.z = (v1->z - v0->z) / L;
}
inline void ntElement::set_A(float a){
	this->A = a;
}
#endif