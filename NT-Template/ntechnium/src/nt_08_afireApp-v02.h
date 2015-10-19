///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////// nt_08_afireApp.h
// openGl scratch libs							///////////////////
// Derived Content Class:  	Array Fire Test App	///////////////////
// created by James Warton on 03/18/2015		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef APPLICATION_ARRAYFIRE_v02_JPW_NTECHNIUM
#define APPLICATION_ARRAYFIRE_v02_JPW_NTECHNIUM
#define NOMINMAX
#define _USE_MATH_DEFINES

#include "nt_AppContent.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <array>
#include <math.h>
#include <sstream>
#include <iomanip>

#include <cstdio>
#include <arrayfire.h>
#include <af/util.h>
#include <af/dim4.hpp>
#include <af/traits.hpp>
#include <cstdlib>

#include "nt_Utility.h"
#include "ntMath.h"

using namespace jpw;
using namespace af;

class afApp02 : public AppContent {
private:
	
public:
	//////////////////////////////VARIABLES UNIQUE TO DERIVED CLASS
	///////////////////////////////////////////////////////////////
	af::array img_00;

	int img_X;
	int img_Y;

	af::array img_Sum;// = img_00 * 0;
	af::array img_Avg;// = img_00 * 0;
	af::array img_Var;// = img_00 * 0;
	
	std::string pathIn;
	std::string pathOut;
	std::string pathExtension;
	std::string fileName;
	std::string fileExt;
	std::string	url;

	int rangeBeg;
	int rangeEnd;
	int range;

	bool doHistogram;

	////////////////////////////////////////////////// CONSTRUCTORS
	//////////////////////////////// DEFAULT NOT NEEDED FOR DERIVED
	///////////////////////////////////////////////////////////////
	

	//////////////////////////////////////// REQUIRED CLASS METHODS
	///////////////////////////////////////////////////////////////
	void init();
	void run();

	void imgAvg(std::string fileName, int rangeBeg, int rangeEnd);
	void imgVar(std::string fileName, int rangeBeg, int rangeEnd);
	void imgHis(std::string fileName, af::array histogram);
	
	/////////////////////////////////////////////////// CONVIENENCE
	///////////////////////////////////////////////////////////////
	void display();
};
#endif
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////