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

#include <thread>
#include <future>
#include <chrono>

#include <cstdio>
#include <arrayfire.h>
#include <af/util.h>
#include <af/dim4.hpp>
#include <af/traits.hpp>
#include <cstdlib>
#include <cstddef>         // std::size_t

#include "ntMath.h"
#include "ntColor4f.h"
#include "ntMatrix4.h"
#include "nt_AppContent.h"

#include "ntPtsTxt.h"
#include "ntPanel.h"
#include "ntFace3.h"
#include "ntGraph.h"
#include "ntType.h"


using namespace jpw;

class ovisApp : public AppContent {
private:
	//static struct tm timeData;
	//static time_t timeStamp;
	clock_t t_CPU = 0;

	///////////////////////////////////////////////////////////////
	////////////////////////////////////// SYSTEM DISPLAY VARIABLES
	string font = "AGENCYR";
	ntType title;
	ntType t_00;
	ntType t_01;
	ntType t_02;
	ntType t_03;
	ntType t_04;
	ntType t_05;
	ntType t_06;
	ntType t_07;
	ntType t_08;
	ntType t_09;
	ntType t_10;
	ntType t_11;
	ntType t_12;
	ntType t_13;
	ntType t_14;
	ntType t_15;
	ntType t_16;
	ntType t_17;
	ntType t_18;
	ntType t_19;

	void init_SysData();
	void display_SysData();
	///////////////////////////////////////////////////////////////
	//////////////////////////////////////// SOURCE IMAGE VARIABLES
	string url_IMG;
	string path_IMG = nt_Utility::getPathToResources();
	string pathExtension_IMG = "imgs\\ovis\\";
	//string fileName_IMG = "ovis_stripes";
	//string fileName_IMG = "ovis_4x4_RGJW-15k";// "ovis_4x4_RG-15k";//"ovis_6x6_HY15k"; //"ovis_8x8_RG-15k";//"ovis_3x3_HY15k"; //"ovis_3x3_DD15k";// "ovis_8x8"; //
	//string fileName_IMG = "ovis_8x8"; //
	string fileName_IMG = "ovis_4x4_CBJW-10k"; //"ovis_6x6_CBJW-10k"; //  "ovis_4x4_RGJW-10k";//
	string fileExt_IMG = ".jpg";

	af::array img_IN;
	af::array img_2D;
	arma::mat img_2d;

	///////////////////////////////////////////////////////////////
	///////////////////////////////////////// SOURCE DATA VARIABLES
	//string fileName_TXT = "ptPos_14_OvisTriPts";
	//string fileName_TXT = "ptPos_20_OvisZeppSys";
	//string fileName_TXT = "OvisZeppSys_160314";
	string fileName_TXT = "Z_00_composite"; // "Z7";
	//string fileName_TXT = "B0100";		//FASTENER LOCATION REVIEW PANELS
	//string fileName_TXT = "AC_001";		//ACOUSTIC WIND TUNNEL TEST PANELS
	//string fileName_TXT = "GP_001";

	static string path;

	bool isStartFile =		false;
	bool isEndSubs =		false;
	bool isSubNext =		false;
	bool isEndFile =		false;
	bool isMultiThread =	false;
	bool isNode =			false;
	bool isCorner =			false;
	bool isFastener =		false;

	int panel_NUM = 0;
	int panel_Dim = 0;
	///////////////////////////////////////////////////////////////
	/////////////////////////////////////// TEMPORARY PANEL DETAILS
	string panel_ID =	"<< ERROR >>";
	string panel_Norm = "<< ERROR >>";
	string panel_Vert = "<< ERROR >>";
	string panel_CPts = "<< ERROR >>";
	string panel_UVW =	"<< ERROR >>";
	string panel_Dir =  "<< ERROR >>";
	
	ntVec3 cpts[5];
	ntVec3 fpts[5];
	ntVec3 verts[3];
	ntVec3 norm_G;
	ntVec3 param_UVW;
	///////////////////////////////////////////////////////////////
	//////////////////////////////////////////// LIST OF ALL PANELS
	std::vector<ntPanel*> panels;
	std::vector<ntFace3*> faces;

	///////////////////////////////////////////////////////////////
	//////////////////////////////////////////// UTILITIY FUNCTIONS
	void read_DATA();
	void read_IMG();
	///////////////////////////////////////////////////////////////
	/////////////////////////////// GRAPH PERFORATION | PANELS DATA
	ntGraph graph00;	// PANEL PERFORATION HISTOGRAM
	ntGraph graph01;	// PERFORATION AREA
	ntGraph graph02;	// IMAGE HISTOGRAM
	static std::vector<float> p_Rad;

	static int n_perf;
	static float areaS_gross;
	static float areaS_net;
	static float areaP_Max;
	static float areaP_Min;

	static float phi_Min;
	static float phi_Max;
	///////////////////////////////////////////////////////////////
	///////////////////////////////////////// MULTITHREAD FUNCTIONS
	static void funct(ntPanel* panel_ptr);
	static void fun02(int ind_S, int ind_E, std::vector<ntPanel*>* panels, int x);
	static void write_Panels_TXT(int ind_S, int ind_E, std::vector<ntPanel*>* panels);
	static void write_Panels_IMG(int ind_S, int ind_E, std::vector<ntPanel*>* panels);
	///
	static int gen;
	static int gen_G;
	static int gen_L;
	static bool isImgLoaded;
	static bool isTxtLoaded;
	static bool isExceed;
	static arma::mat img_00;
	static int img_X;
	static int img_Y;
	static int img_T;

	Vec3 add_VEC(string line);

	static string format_STR(string line);
	static string format_VEC(ntVec3* vec);

	static void align_Panel(ntPanel* panel_ptr, ntVec3* axis);
	static void align_Panel(ntPanel* panel_ptr, ntVec3* axis_A, ntVec3* axis_B, ntVec3* pos);
	static void round_Pos(ntPanel* panel_ptr,float tolerance);
	static void write_Panel_TXT(ntPanel* panel_ptr);
	static void write_Panel_IMG(ntPanel* panel_ptr);
	static void map_ImgCol(ntPanel* panel_ptr);
	static void tile_Img(int U, int V, af::array img);
	static void log_Angles(ntPanel* panel_ptr);

	int panel_Index = 0;	// DISPLAY INDEX FOR CURRENT PANEL

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
	D_mode m = vS;							 //CURRENT DISPLAY MODE
	L_mode mode_L = vV;

	void display();
	void display_IMG();
};
#endif
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////