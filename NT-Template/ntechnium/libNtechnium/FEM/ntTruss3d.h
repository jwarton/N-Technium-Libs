///////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////// ntTruss3d_ARM.h
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
#include <time.h>
#include <armadillo>

#include <arrayfire.h>
#include <af/util.h>
#include <af/dim4.hpp>
#include <af/traits.hpp>

#include "nt_Utility.h"
#include "ntMath.h"
#include "ntElement.h"
#include "ntNode.h"

using namespace jpw;
//using namespace arma;

//enum D_mode{ vQ, vF, vP, vS, vW};  ///RELOCATED TO MATH CLASS

class ntTruss {
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

	//HOST DATA
	arma::mat K;									//GLOBAL STIFFNESS
	arma::mat Kb;									//BOUNDARY CONDITIONS
	arma::mat Q;									//NODAL DISPLACEMENT
	arma::mat F;									//FORCE COMPONENTS
	arma::mat P;									//AXIAL FORCES
	arma::mat S;									//ELEMENT STRESSES

	std::vector <int> bc_Id;

	void init();
	void eval();
	
	void eval_GPU();
	void eval_GPUsp();
	void eval_GPUcs();
	void eval_CPU();
	
	D_mode m;										//CURRENT DISPLAY MODE

	//MATRIX PERFORMANCE DATA
	struct tm timeData;
	time_t timeStamp;

	clock_t t_CPU = 0;
	clock_t t_copy = 0;
	clock_t t_GPU = 0;
	clock_t t_copy_sp = 0;
	clock_t t_GPU_cs = 0;

	int eval_iterations;
	std::vector <float> iterations_CPU;
	std::vector <float> iterations_GPU;
	std::vector <float> iterations_copy;

public:
	std::vector <ntNode*> nodes;					//nodes
	std::vector <ntElement*> elements;				//elements
	//std::vector <ntForces*> forces;				//forces

	ntTruss();
	ntTruss(std::vector <ntElement*> elements);
	ntTruss(std::vector <ntNode*> nodes, std::vector <ntElement*> elements);

	void def_KG();									//define global stiffness matrix
	void def_FG();									//define matrix of known forces
	void def_BC();									//define boundary conditions

	void set_FixedNodes(ntNode node, ntVec3 dir);
	void set_F(ntNode node, ntVec3 dir, float mag);

	void opt_Size(float sMax, float dMax, float QgMax);

	///////////////////////////////////////////////////////////////////
	/////////////////////////////////////////// MATRIX PERFORMANCE DATA
	void write_txt(std::string s);

	int get_dim_Q();
	int get_dim_F();
	int get_dim_K();

	vector <float> get_t_CPU();
	vector <float> get_t_GPU();
	vector <float> get_t_copy();

	void run();
	void display_mode(D_mode m);
	void display();
};
#endif