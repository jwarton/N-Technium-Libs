///////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////// ntElement_arm.h
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

#ifndef SPACE_TRUSS_ELEMENT_JPW_NTECHNIUM_ARM
#define SPACE_TRUSS_ELEMENT_JPW_NTECHNIUM_ARM

#include <iostream>
#include <sstream>
#include <vector>
#include <array>
#include <armadillo>

#include "ntVec3.h"
#include "ntNode_arm.h"
#include "ntEdge.h"

//using namespace std;
using namespace jpw;
//using namespace arma;

enum FE_type { S1 = 1, T3 = 3, F6 = 6, Pn};

class ntElement: public ntEdge{
private:
	
	void init();
	void setDirection();

	FE_type FE;
	ntNode* n0;
	ntNode* n1;

	float E = 0;					//modulus of elasticity
	float I = 0;					//moment of interia
	float A = 0;					//area of cross section
	float L = 0;					//length
	float S = 0;					//stiffness factor
	Vec3  UVW;						//direction vector

	arma::mat ke;					//local stiffness				(E*A / L) * [i]
	arma::mat q = arma::zeros(1, 2);		//local displacements			{Q} * [t]
	arma::mat t = arma::zeros(2, 6);		//local transformation matrix	[ l m n 0 0 0; 0 0 0 l m n]
	arma::mat p = arma::zeros(2, 1);		//axial forces					[ke] * {q}

	arma::mat k = arma::zeros(6, 6);		//global element stiffness 
	arma::mat Q = arma::zeros(6, 1);		//global displacements
	arma::mat T = arma::zeros(6, 6);		//global transformation matrix
	std::array<int, 2> nodeInd;		//matrix indices for node relations


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
	arma::mat get_k();
	arma::mat get_p();
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
inline arma::mat ntElement::get_k(){
	return k;
}
inline arma::mat ntElement::get_p(){
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