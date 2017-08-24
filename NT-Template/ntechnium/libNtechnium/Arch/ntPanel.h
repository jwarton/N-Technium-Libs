///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////// ntPanel.h
// openGl scratch libs							///////////////////
// Triangle Panel Class	                        ///////////////////
// created by James Warton on 10/09/2015		///////////////////
///////////////////////////////////////////////////////////////////


#ifndef PANEL_JPW_NTECHNIUM
#define PANEL_JPW_NTECHNIUM

#include <iostream>
#include <array>
#include <vector>
#include <numeric>

#include "ntVec3.h"
#include "ntColor4f.h"
#include "ntVertex.h"
#include "ntEdge.h"
#include "ntNormal.h"
#include "ntCircle.h"
#include "ntPolyline.h"
#include "ntFace3.h"
#include "ntTriCell.h"
#include "ntMath.h"
#include "ntMatrix4.h"
#include "ntGraph.h"

#include <armadillo>

using namespace jpw;

enum grid_Type { SQU, TRI, DIA };  // ADD: DIV, EQU
enum perf_Mode { GRID, SUBD };
enum perf_Type { TRICELL, DOT };
enum fast_Type { CORNER, EQ_DIV};

class ntPanel{
private:
	perf_Mode	perf_mode	=	SUBD;
	perf_Type	perf_type	=	TRICELL;
	perf_Style  perf_style	=	perf_03;
	grid_Type	grid_type	=	TRI;
	fast_Type	fast_type   =   CORNER;

	void init();
	///////////////////////////////////////////////////////////////
	/////////////////////////////////////////////// PANEL TEXT DATA
	string panel_ID;
	string n_G;						  //GLOBAL PANEL NORMAL
	string string_UVW;				  //GLOBAL PARAMETER
	string dir;
	float weight;
	int region;

	///////////////////////////////////////////////////////////////
	/////////////////////////////// GRAPH PERFORATION | PANELS DATA
	ntGraph graph;
	ntGraph graph_P;
	ntGraph graph_A;

	void sub_Div(std::vector< vector <ntFace3>* >*	faces, int gen, bool isPanel);
	void plot_Perf_GR(int div, grid_Type grid = TRI);
	void plot_Perf_SD(int div);
	float calc_Perf_R(Vec3 *vec, float val);

public:
	///////////////////////////////////////////////////////////////
	////////////////////////////////////////// PANEL DATA STRUCTURE
	ntVec3							*v0,*v1,*v2;
	ntVec3							*vecs[13];
	ntVertex						*vert0,*vert1,*vert2;
	std::vector<ntVertex*>			 verts;
	std::vector<ntEdge>				 edges;		// CENTERLINE 0-2 : NODE 
												// CENTERLINE 3-5 : FASTENER 
												// CENTERLINE 6-8 : GAP 

	std::vector<ntVec3*> c_G;		  //GLOBAL PANEL CORNER POSITONS
	std::vector<ntVec3*> c_L;		  //LOCAL  PANEL CORNER POSITONS
	std::vector<ntVec3*> f_G;		  //GLOBAL PANEL FASTENER CL END POINT POSITONS
	std::vector<ntVec3*> f_L;		  //LOCAL  PANEL FASTENER CL END POINT POSITONS
	std::vector<ntVec3*> v_G;		  //GLOBAL PANEL VERTEX POSITONS
	std::vector<ntVec3*> v_L;		  /// NOT IN USE // LOCAL PANEL VERTEX POSITION 

	///////////////////////////////////////////////////////////////
	///////////////////////////////////// SUBIVISION DATA STRUCTURE
	std::vector <ntVec3*>			vecs_UV;   //  UV AT CORNERS
	std::vector <ntVec3*>			vecs_SD;   //  LOCAL COORDINATE
	std::vector <vector <ntVec3*>*> p_XY_Rows; //  VEC POS BY ROW
	std::vector <vector <ntVec3*>*> p_UV_Rows; //  UVW PRM BY ROW
	std::vector <vector <ntFace3>*>	faces_L;   //  LOCAL COORDINATE
	std::vector <vector <ntFace3>*>	faces_G;   //  WORLD COORDINATE

	bool	is_Odds		= true;
	bool	is_SubDiv	= false;
	bool	is_PerfSD   = false;
	bool	is_Graph    = false;
	int		cnt_SubDiv	= 0;
	float	image_Val;				 // PIXEL VALUE AT PNL CENTROID
	std::vector <float> image_Vals;  // PIXEL VALUE AT SD CENTROIDs
	///////////////////////////////////////////////////////////////
	/////////////////////////////////////// TRI CELL DATA STRUCTURE
	std::vector <ntTriCell> cells;
	std::vector <ntTriCell> cells_L;
	std::vector <float> scFx;
	void set_TriCells();

	///////////////////////////////////////////////////////////////
	//////////////////////////////////// PERFORATION DATA STRUCTURE
	std::vector <ntCircle*>			perfs;		//  LIST OF ALL PERF
	std::vector <ntPolyline*>		lines;		//  LIST OF ALL PLINES
	std::vector<ntVec3*>			p_Pos;		//  LIST OF ALL POS  USED BY ORTHO FUNCTION
	std::vector<float>				p_Rad;		//  LIST OF ALL RAD
	std::vector<ntVec3*>			p_UVs;		//  LIST OF UV AT PERF POS
	std::vector<float>				p_Col;		//  LIST OF VAL AT PERF POS
	std::vector <ntCircle*>			fastr;		//  LIST OF ALL FASTENERS
	std::vector <ntVec3*>			f_Pos;		//  LIST OF FASTENER POS

	///////////////////////////////////////////////////////////////
	//////////////////////////////////////// PERFORATION PARAMETERS
	float	r_Min			= 0.1875;		//3/16"
	float	r_Max			= 0.5;			// 0.625;	//5/8"
	float	edge_Offset		= 0.75 + r_Max;
	int		n_seg			= 18;
	int		perf_size;
	double	perf_spaceMin	= 1.5;
	double	perf_area;
	double  perf_perc;
	double  fast_spaceMax	= 14.5;
	double	web_Min			= 0.5;
	float	gap_dist		= 0.375;

	bool is_Increment =		true;
	bool is_Noise =			false;	//true;// 
	///////////////////////////////////////////////////////////////
	////////////////////////////////////////////// PANEL PARAMETERS
	/// SHOULD BE PRIVATE MEMBER VARIABLES
	/// ACCESSIBLE BY FUNCTION CALL ONLY
	ntColor4f	col;
	ntVec3		norm;
	ntVec3*		cent;
	ntVertex	centroid;
	ntNormal	normal;
	double		area = 0;			//units [sq. inch]
	std::vector<double>	phi;		//units [degrees ]

	///////////////////////////////////////////////////////////////
	////////////////////////////////////////////////// CONSTRUCTORS
	ntPanel();
	ntPanel(ntVec3* v0,ntVec3* v1,ntVec3* v2);

	///////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////// FUNCTIONS
	void set_Color(ntColor4f col);
	void calcNorm();					
	void calcCentroid();				
	void calcArea();
	void calcPhi();

	void sub_Div(int gen);
	void sub_Div(int div, bool isDiv);

	ntVec3* eval_Fastener(std::vector <ntVec3*> vecs, ntVec3* vec, int index, bool isAsc);
	ntVec3* map_UVW(std::vector<ntVec3*> vec_XYZ, std::vector<ntVec3*> vec_UVW, ntVec3* xyz);
	ntVec3* map_UVW(arma::mat T, ntVec3* xyz);
	arma::mat get_Mat_T(std::vector<ntVec3*> vec_XYZ, std::vector<ntVec3*> vec_UVW);

	void plot_Fast(int div);
	void plot_Perf(int div, grid_Type grid, perf_Type type);
	void set_PerfStyle(perf_Style style);
	void set_PerfType(perf_Type type);
	void add_Perf();
	void reparam_UV();

	void set_Graph();

	bool    pt_isInside(ntVec3* point);
	ntVec3* pt_Mid(ntVec3* v0, ntVec3* v1);

	void set_ID(string panel_ID);
	void set_vG();
	void set_cG(ntVec3* pt, int index);
	void set_fG(ntVec3* pt, int index);
	void set_Edges();	void set_nG(string n_G);
	void set_UVW(string string_UVW);
	void set_UVW(std::vector <ntVec3*>	vecs_UV);
	void set_Dir(string dir);
	void set_Weight(string wht);
	void set_Region(string reg);
	void set_IMG(float val);

	string get_ID();	//PANEL ID
	string get_n_G();	//GLOBAL NORMAL
	string get_UVW();
	string get_Dir();
	float  get_MeanVal();
	float  get_Weight();
	int    get_Region();

	std::vector<ntVec3*>		get_v_G();		//GLOBAL VERTICES
	std::vector<ntVec3*>		get_c_G();		//GLOBAL CORNERS | TRIMMED OFFSET
	std::vector<ntVec3*>		get_c_L();		//LOCAL  CORNERS | TRIMMED OFFSET
	std::vector<ntVec3*>		get_f_L();		//LOCAL FASTENER CENTERLINE OFFSET VERTICES
	std::vector<ntVec3*>		get_Fast();		//LOCAL FASTENER POSITIONS
	std::vector<ntPolyline*>	get_PLin(); 
	std::vector<ntVec3*>		get_Perf();
	std::vector<float>			get_Perf_R();

	float get_Area();
	float get_AngleMin();
	float get_AngleMax();
	float get_EdgeMin();

	void display();
	void display_Graph();
	void display_Nodes();
	void display_Joint();
	void display_CL();
	void display_Edge();
	void display_EdgeSd(int gen = 0);
	void display_EdgeSD_G(int gen = 0);
	void display_Face_L(L_mode mode, int gen = 0);
	void display_Face_G(int gen = 0);
	void display_Perf();
};
#endif
