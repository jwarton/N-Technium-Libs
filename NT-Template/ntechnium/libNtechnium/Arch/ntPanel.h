///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////// ntPanel.h
// openGl scratch libs							///////////////////
// Triangle Panel Class	                        ///////////////////
// created by James Warton on 10/09/2015		///////////////////
///////////////////////////////////////////////////////////////////


#ifndef PANEL_JPW_NTECHNIUM
#define PANEL_JPW_NTECHNIUM

#include <iostream>
#include <vector>
#include "ntVec3.h"
#include "ntColor4f.h"
#include "ntVertex.h"
#include "ntEdge.h"
#include "ntNormal.h"
#include "ntCircle.h"
#include "ntFace3.h"
#include "ntMath.h"
#include "ntMatrix4.h"
#include "ntGraph.h"

#include <armadillo>

using namespace jpw;

enum grid_T { SQU, TRI, DIA };  // ADD: DIV, EQU

class ntPanel{
private:
	enum P_mode { GRID, SUBD };
	P_mode perf_Mode = SUBD;

	void init();
	///////////////////////////////////////////////////////////////
	/////////////////////////////////////////////// PANEL TEXT DATA
	string panel_ID;
	string n_G;						  //GLOBAL PANEL NORMAL
	string p_G;						  //GLOBAL VERTEX POSTIONS
	string string_UVW;				  //GLOBAL PARAMETER

	///////////////////////////////////////////////////////////////
	/////////////////////////////// GRAPH PERFORATION | PANELS DATA
	ntGraph graph;	

	void sub_Div(std::vector< vector <ntFace3>* >*	faces, int gen, bool isPanel);
	void plot_Perf_GR(int div, enum grid_T grid);
	void plot_Perf_SD(int div);
	float calc_Perf_R(Vec3 *vec, float val);

public:
	///////////////////////////////////////////////////////////////
	////////////////////////////////////////// PANEL DATA STRUCTURE
	ntVec3							*v0,*v1,*v2;
	ntVec3							*vecs[3];
	ntVertex						*vert0,*vert1,*vert2;
	std::vector<ntVertex*>			 verts;
	std::vector<ntEdge>				 edges;

	std::vector<ntVec3*> v_G;		  //GLOBAL PANEL VERTEX POSITONS
	std::vector<ntVec3*> v_L;		  ///  NOT IN USE // LOCAL PANEL VERTEX POSITION 

	///////////////////////////////////////////////////////////////
	///////////////////////////////////// SUBIVISION DATA STRUCTURE
	std::vector <ntVec3*>			vecs_UV;   //  UV AT CORNERS
	std::vector <ntVec3*>			vecs_SD;   //  LOCAL COORDINATE
	std::vector <vector <ntVec3*>*> p_XY_Rows; //  VEC POS BY ROW
	std::vector <vector <ntVec3*>*> p_UV_Rows; //  UVW PRM BY ROW
	std::vector <vector <ntFace3>*>	faces_L;   //  LOCAL COORDINATE
	std::vector <vector <ntFace3>*>	faces_G;   //  WORLD COORDINATE

	bool	is_SubDiv	= false;
	bool	is_PerfSD   = false;
	bool	is_Graph = false;
	int		cnt_SubDiv	= 0;
	float	image_Val;				 // PIXEL VALUE AT PNL CENTROID
	std::vector <float> image_Vals;  // PIXEL VALUE AT SD CENTROIDs
	///////////////////////////////////////////////////////////////
	//////////////////////////////////// PERFORATION DATA STRUCTURE
	std::vector <ntCircle*>			perfs;     //  LIST OF ALL PERF
	std::vector<ntVec3*>			p_Pos;	   //  LIST OF ALL POS  USED BY ORTHO FUNCTION
	std::vector<float>				p_Rad;	   //  LIST OF ALL RAD
	std::vector<ntVec3*>			p_UVs;	   //  LIST OF UV AT PERF POS
	std::vector<float>				p_Col;	   //  LIST OF VAL AT PERF POS

					
	///////////////////////////////////////////////////////////////
	//////////////////////////////////////// PERFORATION PARAMETERS
	float	r_Min =			0.1875;	//3/16"
	float	r_Max =			0.625;	//5/8"
	float	edge_Offset =	0.75 + r_Max;
	int		n_seg =			36;
	int		perf_size;

	bool is_Increment =		true;
	bool is_Noise = true;// false;
	///////////////////////////////////////////////////////////////
	////////////////////////////////////////////// PANEL PARAMETERS
	ntColor4f	col;
	ntVec3		norm;
	ntVec3*		cent;
	ntVertex	centroid;
	ntNormal	normal;
	float		area;		/// SHOULD BE PRIVATE MEMBER VARIALBES
							/// ACCESSIBLE BY FUNCTION CALL ONLY

	///////////////////////////////////////////////////////////////
	////////////////////////////////////////////////// CONSTRUCTORS
	ntPanel();
	ntPanel(ntVec3* v0,ntVec3* v1,ntVec3* v2);

	///////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////// FUNCTIONS
	void set_Color(ntColor4f col);		/// RENAME
	void calcNorm();					/// RENAME
	void calcCentroid();				/// RENAME
	//void calc_Area();

	void sub_Div(int gen);
	void sub_Div(int div, bool isDiv);

	ntVec3* map_UVW(std::vector<ntVec3*> vec_XYZ, std::vector<ntVec3*> vec_UVW, ntVec3* xyz);
	ntVec3* map_UVW(arma::mat T, ntVec3* xyz);
	arma::mat get_Mat_T(std::vector<ntVec3*> vec_XYZ, std::vector<ntVec3*> vec_UVW);

	void plot_Perf(int div, enum grid_T grid);
	void add_Perf();


	void set_Graph();

	bool pt_isInside(ntVec3* point);
	ntVec3* pt_Mid(ntVec3* v0, ntVec3* v1);

	void set_ID(string panel_ID);
	void set_vG();
	void set_nG(string n_G);
	void set_pG(string p_G);
	void set_UVW(string string_UVW);
	void set_UVW(std::vector <ntVec3*>	vecs_UV);
	void set_IMG(float val);

	string get_ID();	//PANEL ID
	string get_n_G();	//GLOBAL NORMAL
	string get_p_G();
	string get_UVW();

	std::vector<ntVec3*>	get_v_G();	//GLOBAL VERTICES
	std::vector<ntVec3*>	get_Perf();
	std::vector<float>		get_Perf_R();

	void display();
	void display_Graph();
	void display_Edge();
	void display_EdgeSd(int gen = 0);
	void display_Face_L(L_mode mode, int gen = 0);
	void display_Face_G(int gen = 0);
	void display_Perf();
};
#endif
