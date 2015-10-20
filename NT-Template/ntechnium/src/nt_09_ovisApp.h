///////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////// nt_09_ovisApp.h
// openGl scratch libs							///////////////////
// Derived Content Class:  Triangles from Pts	///////////////////
// created by James Warton on 10/06/2015		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef APPLICATION_OVIS_v01_JPW_NTECHNIUM
#define APPLICATION_OVIS_v01_JPW_NTECHNIUM
#define _USE_MATH_DEFINES

#include <iostream>
#include <sstream>
#include <array>
#include <math.h>
#include <ctime>
#include <time.h>
#include <iomanip>
#include <armadillo>

#include "ntMath.h"
#include "ntMatrix4.h"
#include "nt_AppContent.h"

#include "ntPtsTxt.h"
#include "ntPanel.h"

using namespace jpw;
class ovisApp : public AppContent {
private:
	int panel_Index = 0;	//DISPLAY INDEX

	string url;
	string path = nt_Utility::getPathToOutput();;
	string pathExtension = "ovis\\";
	string fileName = "ptPos_05_OvisTriPts";
	string fileExt = ".txt";

	bool isStartFile = false;
	bool isEndSubs = false;
	bool isSubNext = false;
	bool isEndFile = false;

	struct tm timeData;
	time_t timeStamp;

	int panel_NUM = 0;
	int panel_Dim = 0;

	float vertPos[3];
	float normVec[3];

	///////TEMPORARY PANEL DETAILS
	string panel_ID = "<< ERROR >>";
	string panel_Norm = "<< ERROR >>";
	string vertex_Pos = "";

	ntVec3 verts[3];
	ntVec3 norms[3];
	ntVec3 norm_G;

	vector<ntPanel*> panels;

	void read_DATA();
	Vec3 add_Vert(string line);

	string format_Norm(string line);
	string format_POS(string line);
	string format_VEC(ntVec3* vec);

	void align_Panel(ntPanel* panel_ptr, ntVec3* axis);
	void align_Panel(ntPanel* panel_ptr, ntVec3* axis_A, ntVec3* axis_B, ntVec3* pos);
	void round_Pos(ntPanel* panel_ptr,float tolerance);
	void write_Panel(ntPanel* panel_ptr);
	double calc_Area(ntPanel* panel_ptr);

public:
	//////////////////////////////VARIABLES UNIQUE TO DERIVED CLASS
	///////////////////////////////////////////////////////////////
	ntTextIO ptData00;

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