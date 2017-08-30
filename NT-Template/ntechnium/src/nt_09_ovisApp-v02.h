///////////////////////////////////////////////////////////////////
/////////////////////////////////////////////// nt_09_ovisApp-v02.h
// openGl scratch libs							///////////////////
// Derived Content Class: Ovis TriSkin Perf	    ///////////////////
// created by James Warton on 07/19/2016		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef APPLICATION_OVIS_v02_JPW_NTECHNIUM
#define APPLICATION_OVIS_v02_JPW_NTECHNIUM

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
class ovisApp02 : public AppContent {
private:

	//string url_IMG;
	//string url_OUT;
	string projectName = "ovis\\";

	string path_IN = nt_Utility::getPathToResources();
	string path_TXT_IN = path_IN + "txt\\" + projectName;
	string path_IMG_IN = path_IN + "imgs\\" + projectName;

	string file_TXT = "CD_20170823_";//"CD_";// "Z"; //"JW_composite_";//
	string file_IMG = "ovis_4x4_CBJW-10k-edge";// "ovis_4x4_CBJW-10k";

	string path_Out = nt_Utility::getPathToOutput() + projectName + "issue170828\\";
	
	ntImage img_00;
	ntTextIO ptData_00;
	ntTriSkin skin_00;

	///////////////////////////////////////////////////////////////
	////////////////////////////////// SYSTEM DATA AND PERFORMANCE
	clock_t t_CPU = 0;
	string t_LoadPanels	= "PANEL DATA LOAD TIME:         ";
	string t_LoadImage	= "IMAGE DATA LOAD TIME:         ";
	string t_Transform	= "PANEL TRANSFORMATION TIME:    ";
	string t_TranPan	= "AVG PANEL TRANSFORM TIME:     ";
	double t_transform	= 0;
	string t_Process	= "TOTAL PROCESSING TIME:        ";
	string t_EVAL		= "";

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