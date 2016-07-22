///////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////// ntTriSkin.h
// openGl scratch libs							///////////////////
// Triangulated Skin Class						///////////////////
// created by James Warton on 06/24/2015		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef TRISKIN_V00_JPW_NTECHNIUM
#define TRISKIN_V00_JPW_NTECHNIUM
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
#include "ntVec3.h"
#include "ntVertex.h"
#include "ntEdge.h"
#include "ntFace3.h"
#include "ntPanel.h"
#include "ntPtsTxt.h"
#include "ntGraph.h"
#include "ntType.h"
#include "ntImage.h"

using namespace std;
using namespace jpw;

class ntTriSkin{
private:
	///////////////////////////////////////////////////////////////
	////////////////////////////////// SYSTEM DATA AND PERFORMANCE
	void init_SysData();
	void display_SysData();
	void write_SysData(std::vector<string> lines);
	//string format_SEC(clock_t time);

	clock_t t_CPU			= 0;
	string t_LoadPanels		= "PANEL DATA LOAD TIME:         ";
	string t_LoadImage		= "IMAGE DATA LOAD TIME:         ";
	string t_Transform		= "PANEL TRANSFORMATION TIME:    ";
	string t_TranPan		= "AVG PANEL TRANSFORM TIME:     ";
	double t_transform		= 0;
	string t_Process		= "TOTAL PROCESSING TIME:        ";
	string t_EVAL			= "";
	double t_saveTXT		= 0;
	string t_saveTxt		= "SAVE PANEL DATA:              ";
	double t_saveIMG		= 0;
	string t_saveImage		= "SAVE PANEL IMAGE MAPS:        ";
	///////////////////////////////////////////////////////////////
	////////////////////////////////////// SYSTEM DISPLAY VARIABLES
	/*
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
	*/
	///////////////////////////////////////////////////////////////
	////////////////////////////////////// SOURCE DATA AND FILE I/O
	string url_IMG;
	string url_TXT;
	string path_OUT;

	string obj_Name;

	int img_X = 0;
	int img_Y = 0;

	ntImage  img_SRC;
	af::array img_IN;
	arma::mat img_00;

	bool isImgLoaded =		false;
	bool isExceed =			false;

	///////////////////////////////////////////////////////////////
	///////////////////////////////////////// SOURCE DATA VARIABLES
	bool isTxtLoaded =		false;
	bool isPathDefined =	false;
	bool isStartFile =		false;
	bool isEndSubs =		false;
	bool isSubNext =		false;
	bool isEndFile =		false;
	bool isNode =			false;
	bool isCorner =			false;
	bool isFastener =		false;
	
	int panel_Dim =		0;		/// PANELS IN PANEL VECTOR
	int panel_Index =	0;		/// DISPLAY INDEX FOR CURRENT PANEL
	///////////////////////////////////////////////////////////////
	/////////////////////////////////////// TEMPORARY PANEL DETAILS
	string panel_ID =	"<< ERROR >>";
	string panel_Norm = "<< ERROR >>";
	string panel_Vert = "<< ERROR >>";
	string panel_CPts = "<< ERROR >>";
	string panel_UVW =	"<< ERROR >>";
	string panel_Dir =  "<< ERROR >>";
	string panel_Weight = "0";
	string panel_Region = "0";
	
	ntVec3 cpts	[5];
	ntVec3 fpts	[5];
	ntVec3 verts[3];
	ntVec3 norm_G;
	ntVec3 param_UVW;

	///////////////////////////////////////////////////////////////
	////////////////////////////////////// DEFAULT PANEL PARAMETERS
	perf_Mode perf_mode		= SUBD;
	perf_Type perf_type		= DOT;
	perf_Style perf_style	= perf_06;
	grid_Type grid_type		= TRI;

	///////////////////////////////////////////////////////////////
	//////////////////////////////////////////// LIST OF ALL PANELS
	std::vector<ntPanel*> panels;
	std::vector<ntFace3*> faces;
	
	///////////////////////////////////////////////////////////////
	/////////////////////////////// GRAPH PERFORATION | PANELS DATA
	ntGraph graph00;	// PANEL PERFORATION HISTOGRAM
	ntGraph graph01;	// PERFORATION AREA
	ntGraph graph02;	// IMAGE HISTOGRAM

	std::vector<float> p_Rad;

	int	n_perf =		0;
	float areaS_gross = 0;
	float areaS_net =	0;
	float areaP_Max =	0;
	float areaP_Min =	99999999;

	float phi_Min =		180;
	float phi_Max =		0;
	int	  perfSpacing = 1.5;
	///////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////// DISPLAY
	D_mode mode_M = vW;						// CURRENT DISPLAY MODE
	L_mode mode_L = vV;

	///////////////////////////////////////////////////////////////
	//////////////////////////////////////////// UTILITIY FUNCTIONS
	void funct(ntPanel* panel_ptr);

	int gen = 5;
	int gen_G = gen;
	int gen_L = gen;

	void read_DATA();
	void read_IMG();

	ntVec3 add_VEC(string line);
	string format_STR(string line);
	string format_VEC(ntVec3* vec);

	void align_Panel(ntPanel* panel_ptr, ntVec3* axis);
	void align_Panel(ntPanel* panel_ptr, ntVec3* axis_A, ntVec3* axis_B, ntVec3* pos);
	void round_Pos(ntPanel* panel_ptr, float tolerance);
	void log_Angles(ntPanel* panel_ptr);

	void write_Panel_TXT(ntPanel* panel_ptr);
	void write_Panel_IMG(ntPanel* panel_ptr);

	void map_ImgCol(ntPanel* panel_ptr);
	void tile_Img(int U, int V, af::array img);

	bool doSaveTXT = false;
	bool doSaveIMG = false;

	///////////////////////////////////////////////////////////////
	///////////////////////////////////// DIPSLAY SCALING FUNCTIONS
	void set_Scale3D(ntPanel* panel_ptr, double scFx);		// TEMPORARY FUNCTION FOR DISPLAY ONLY
	void set_Scale2D(ntPanel* panel_ptr, double scFx);		// TEMPORARY FUNCTION FOR DISPLAY ONLY
public:
	///////////////////////////////////////////////////////////////
	////////////////////////////////////////////////// CONSTRUCTORS
	ntTriSkin();
	ntTriSkin(std::string url_TXT, std::string url_IMG, std::string obj_Name);

	ntTextIO ptData00;
	//////////////////////////////////////// REQUIRED CLASS METHODS
	///////////////////////////////////////////////////////////////
	void setPathOut(std::string url);
	void init();
	void run();

	/////////////////////////////////////////////////// CONVIENENCE
	///////////////////////////////////////////////////////////////
	void set_Parameters(grid_Type grid_type, perf_Type perf_type, float perf_spacing);
	void set_PerfStyle(perf_Style perf_style);
	void set_PerfType( perf_Type perf_type);
	void set_Gen(int gen);
	void set_Mode(D_mode mode);
	void set_Mode_L(L_mode mode);

	void save_IMG();
	void save_TXT();

	void display_Next();
	void display_Prev();
	///int get_Gen();
	/////////////////////////////////////////////////////// DISPLAY
	///////////////////////////////////////////////////////////////
	Col4 col_Select = Col4(1, 0.7, 0.9, 1);

	void display();
	void display_2D();
	void display_IMG();
};
#endif
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////