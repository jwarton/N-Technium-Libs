///////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////// ntTruss3d.h
// openGl scratch libs							///////////////////
// Space Truss Class							///////////////////
// created by James Warton on 11/04/2014		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef SPACE_TRUSS_JPW_NTECHNIUM
#define SPACE_TRUSS_JPW_NTECHNIUM
#define _USE_MATH_DEFINES

#include <iostream>
#include <vector>
#include <cmath>
#include <math.h>

#include <arrayfire.h>
#include <af/util.h>
#include <af/dim4.hpp>
#include <af/traits.hpp>

#include "nt_Utility.h"
#include "ntMath.h"
#include "ntElement.h"
#include "ntNode.h"

using namespace jpw;

enum D_mode{ vQ, vF, vP, vS, vW};

class ntTruss{
private:
	bool isCurrent = false;							//SUSPEND CALCULATIONS AFTER RUNNING ONCE

	int elementCnt = 0;
	int nodeCnt = 0;
	int BCs = 0;
	int DOF = 3;
	int rows = 0;
	int cols;

	float qMax;										//MAXIMUM DISPLACEMENT
	int	  nMax;										//NODE INDEX W/ MAX DISPLACEMENT
	float fMax;										//
	float sMax;										//MAXIMUM ELEMENT TENSION STRESS
	int   eMax;										//ELEMENT INDEX
	float sMin;										//MAXIMUM ELEMENT COMPRESSION STRESS
	int   eMin;										//ELEMENT INDEX
	float W;										//RESULTING APPROXIMATION OF WEIGHT

	//DESIGN CRITERIA
	float SeMax;									///MAXIMUM ALLOWABLE ELEMENT STRESS
	float QnMax;									///MAXIMUM ALLOWABLE NODE DISPLACEMENT
	float QgMax;									///MAXIMUM ALLOWABLE GLOBAL DISPLACEMENT

	//DEVICE DATA
	af::array K;									//GLOBAL STIFFNESS
	af::array Kb;									//BOUNDARY CONDITIONS
	af::array Q;									//NODAL DISPLACEMENT
	af::array F;									//FORCE COMPONENTS
	af::array P;									//AXIAL FORCES
	af::array S;									//ELEMENT STRESSES

	//HOST COPY										//COPIES DATA FOR DISPLAY
	std::vector <float> Q_h;
	std::vector <float> F_h;
	std::vector <float> P_h;
	std::vector <float> S_h;

	std::vector <int> bc_Id;

	void init();
	void eval();
	void host_copy();

	D_mode m;
public:
	std::vector <ntNode*> nodes;					//nodes
	std::vector <ntElement*> elements;				//elements
	//std::vector <ntForces*> forces;				//forces

	ntTruss();
	ntTruss(std::vector <ntElement*> elements);
	ntTruss(std::vector <ntNode*> nodes, std::vector <ntElement*> elements);

	void def_KG();				//define global stiffness matrix
	void def_FG();				//define matrix of known forces
	void def_BC();				//define boundary conditions
	/*
	void set_FixedNodes(ntNode node, ntVec3 dir);
	void set_F(ntNode node, ntVec3 dir, float mag);

	void opt_Size(float sMax, float dMax, float QgMax);
	
	void get_Q();				//nodal displacement
	void get_F();				//force components and reactions
	void get_S();				//element stress
	void get_P();				//element axial forces
	*/
	void write_txt(std::string s);

	void run();
	void display_mode(D_mode m);
	void display();
};
#endif