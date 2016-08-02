///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////// nt_11_c3-App-v02.h
// openGl scratch libs							///////////////////
// Derived Content Class: Image Mosaic		    ///////////////////
// created by James Warton on 06/24/2016		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef APPLICATION_C3_TOKYO_V02_JPW_NTECHNIUM
#define APPLICATION_C3_TOKYO_V02_JPW_NTECHNIUM

#include <iostream>
#include <iomanip>
#include "ntMath.h"
#include "ntMatrix4.h"
#include "nt_AppContent.h"
#include "nt_Utility.h"
#include "ntImage.h"
#include "ntPtsTxt.h"
#include "ntMosaic.h"
#include "ntTriSkin.h"

using namespace jpw;
class tokyoApp02 : public AppContent {
private:

	//string url_IMG;
	//string url_OUT;

	string path_IN = nt_Utility::getPathToResources();
	string path_TXT_IN = path_IN + "txt\\c3\\";
	string path_IMG_IN = path_IN + "imgs\\c3\\01\\";

	//string file_TXT = "AC_001";
	//string file_TXT = "C3_001";			// PLANE FOLDS
	string file_TXT = "C3_401";		// TRIVIAL SURFACE
	//string file_TXT = "C3_004";		// XY PLANE
	//string file_IMG = "lenna";
	//string file_IMG = "profile";
	string file_IMG = "ovis_4x4_CBJW-10k";

	string path_Out = nt_Utility::getPathToOutput() + "c3\\03\\";
	
	ntImage img_00;
	ntTextIO ptData_00;
	ntTriSkin skin_00;

	///////////////////////////////////////////////////////////////
	////////////////////////////////// SYSTEM DATA AND PERFORMANCE
	clock_t t_CPU = 0;
	string t_LoadPanels = "PANEL DATA LOAD TIME:         ";
	string t_LoadImage = "IMAGE DATA LOAD TIME:         ";
	string t_Transform = "PANEL TRANSFORMATION TIME:    ";
	string t_TranPan = "AVG PANEL TRANSFORM TIME:     ";
	double t_transform = 0;
	string t_Process = "TOTAL PROCESSING TIME:        ";
	string t_EVAL = "";

public:
	////////////////////////////////////////////////// CONSTRUCTORS
	//////////////////////////////// DEFAULT NOT NEEDED FOR DERIVED
	///////////////////////////////////////////////////////////////

	//////////////////////////////////////// REQUIRED CLASS METHODS
	///////////////////////////////////////////////////////////////
	void init();
	void run();
	/////////////////////////////////////////////////// CONVIENENCE
	///////////////////////////////////////////////////////////////
	void display();
};
#endif
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////