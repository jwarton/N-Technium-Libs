///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////// ntElement.cpp
// openGl scratch libs							///////////////////
// Element Class								///////////////////
// created by James Warton on 11/04/2014		///////////////////
///////////////////////////////////////////////////////////////////

#include "ntElement.h"

ntElement::ntElement(){}

ntElement::ntElement(ntNode* n0, ntNode* n1, float A, float E, std::array<int, 2> nodeInd):
ntEdge(n0, n1), A(A), E(E), n0(n0), n1(n1), nodeInd(nodeInd){
	FE = T3;
	init();
}
ntElement::ntElement(ntNode* n0, ntNode* n1, float A, float I, float E, std::array<int, 2> nodeInd) :
ntEdge(n0, n1), A(A), E(E), n0(n0), n1(n1), nodeInd(nodeInd){
	FE = F6;
	///init();  FUNCTIONS INCOMPLETE FOR TESTING
}

void ntElement::init(){
	n0->maxDOF = FE;
	n1->maxDOF = FE;
	n0->def_BC();
	n1->def_BC();
	def_T();
	def_ke();
	def_k();
}

void ntElement::def_T(){

	UVW = getDirection();

	float l = UVW.x;			//DIRECTION COSINES X
	float m = UVW.y;			//DIRECTION COSINES Y
	float n = UVW.z;			//DIRECTION COSINES Z

	T(0, 0) = pow(l, 2);
	T(0, 1) = l*m;
	T(0, 2) = l*n;
	T(0, 3) = -1 * pow(l, 2);
	T(0, 4) = -1 * l*m;
	T(0, 5) = -1 * l*n;
	T(1, 1) = pow(m, 2);
	T(1, 2) = m*n;
	T(1, 3) = -1 * l*m;
	T(1, 4) = -1 * pow(m, 2);
	T(1, 5) = -1 * m*n;
	T(2, 2) = pow(n, 2);
	T(2, 3) = -1 * l*n;
	T(2, 4) = -1 * m*n;
	T(2, 5) = -1 * pow(n, 2);
	T(3, 3) = pow(l, 2);
	T(3, 4) = l*m;
	T(3, 5) = l*n;
	T(4, 4) = pow(m, 2);
	T(4, 5) = m*n;
	T(5, 5) = pow(n, 2);

	T(1, 0) = T(0, 1);
	T(2, 0) = T(0, 2);
	T(2, 1) = T(1, 2);
	T(3, 0) = T(0, 3);
	T(3, 1) = T(1, 3);
	T(3, 2) = T(2, 3);
	T(4, 0) = T(0, 4);
	T(4, 1) = T(1, 4);
	T(4, 2) = T(2, 4);
	T(4, 3) = T(3, 4);
	T(5, 0) = T(0, 5);
	T(5, 1) = T(1, 5);
	T(5, 2) = T(2, 5);
	T(5, 3) = T(3, 5);
	T(5, 4) = T(4, 5);
}
void ntElement::def_k(){

	UVW = getDirection();
	L = getLength();
	if (FE == T3){
		k = ((E * A )/ L) * T;
	}
	else if (FE == F6){
		def_ke();
		k = ke * T;
	}
}
void ntElement::def_p(){

	UVW = getDirection();
	L = getLength();

	float l = UVW.x;			//DIRECTION COSINES X
	float m = UVW.y;			//DIRECTION COSINES Y
	float n = UVW.z;			//DIRECTION COSINES Z
	if (FE == T3){
		t(0, 0) = l;
		t(0, 1) = m;
		t(0, 2) = n;
		t(1, 3) = l;
		t(1, 4) = m;
		t(1, 5) = n;
	} else if (FE == F6){
		t = af::array(6, 6);
		t = T;					///NOT CERTAIN OF THIS APPROACH
	}

	Q(0, 0) = n0->Q(0);
	Q(1, 0) = n0->Q(1);
	Q(2, 0) = n0->Q(2);
	Q(3, 0) = n1->Q(0);
	Q(4, 0) = n1->Q(1);
	Q(5, 0) = n1->Q(2);

	//Refer to p84 (2.47) and (2.48)  f = k * [Q] * [T]
	q = af::matmul(t, Q);		//q = (t * Q);
	p = af::matmul(ke, q);		//p = ke * q;
}
void ntElement::def_ke(){
	UVW = getDirection();
	L = getLength();
	if (FE == T3){
		ke = af::array(2, 2);

		ke(0, 0) =  (E*A) / L;
		ke(0, 1) = -(E*A) / L;
		ke(1, 0) = -(E*A) / L;
		ke(1, 1) =  (E*A) / L;
	
	} else if (FE == F6){
		///DEVELOP FUNCTION FOR EQUIVALENT FORCES
		ke = af::array(6, 6);

		ke(0, 0) = (E*A) / L;
		ke(0, 3) =-(E*A) / L;

		ke(1, 1) = ((E*I * 12) / pow(L, 3));
		ke(1, 2) = ((E*I * 6 ) / pow(L, 2));
		ke(1, 4) =-((E*I * 12) / pow(L, 3));
		ke(1, 5) = ((E*I * 6 ) / pow(L, 2));

		ke(2, 1) = ke(1, 2);
		ke(2, 2) = ((E*I * 4) / L);
		ke(2, 4) =-((E*I * 6) / pow(L, 2));
		ke(2, 5) = ((E*I * 2) / L);

		ke(3, 0) =-(E*A) / L;
		ke(3, 3) = (E*A) / L;

		ke(4, 1) = ke(1, 4);
		ke(4, 2) = ke(2, 4);
		ke(4, 4) = ((E*I * 12) / pow(L, 3));
		ke(4, 5) = -((E*I * 6) / pow(L, 2));

		ke(5, 1) = ke(1, 5);
		ke(5, 2) = ke(2, 5);
		ke(5, 4) = ke(4, 5);
		ke(5, 5) = ((E*I * 4) / L);

		//2D BEAM INTERPOLATION FUNCTION
		//ke(0, 0) = ((E * I) / L) *  12;
		//ke(0, 1) = ((E * I) / L) *  6 * L;
		//ke(0, 2) = ((E * I) / L) * -12;
		//ke(0, 3) = ((E * I) / L) *  6 * L;
		//ke(1, 0) = ke(0, 1);
		//ke(1, 1) = ((E * I) / L) *  4 * pow(L, 2);
		//ke(1, 2) = ((E * I) / L) * -6 * L;
		//ke(1, 3) = ((E * I) / L) *  2 * pow(L, 2);
		//ke(2, 0) = ke(0, 2);
		//ke(2, 1) = ke(1, 2);
		//ke(2, 2) = ((E * I) / L) *  12;
		//ke(2, 3) = ((E * I) / L) * -6 * L;
		//ke(3, 0) = ke(0, 3);
		//ke(3, 1) = ke(1, 3);
		//ke(3, 2) = ke(2, 3);
		//ke(3, 3) = ((E * I) / L) *  4 * pow(L, 2);
	}
}