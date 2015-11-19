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

using namespace jpw;

class ntPanel{
private:
	void init();
	///////////////////////////////////////////////////////////////
	/////////////////////////////////////////////// PANEL TEXT DATA
	string panel_ID;
	string n_G;						  //GLOBAL PANEL NORMAL
	string p_G;						  //GLOBAL VERTEX POSTIONS
	string string_UVW;				  //GLOBAL PARAMETER
	
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
	std::vector <ntVec3>			vecs_UV;   //     UV PARAMETERS
	std::vector <ntVec3*>			vecs_SD;   //  LOCAL COORDINATE
	std::vector <vector <ntFace3>*>	faces_L;   //  LOCAL COORDINATE
	std::vector <vector <ntFace3>*>	faces_G;   //  WORLD COORDINATE

	bool	is_SubDiv	= false;
	int		cnt_SubDiv	= 0;
	float	image_Val;				 // PIXEL VALUE AT PNL CENTROID
	std::vector <float> image_Vals;  // PIXEL VALUE AT SD CENTROIDs
	///////////////////////////////////////////////////////////////
	//////////////////////////////////// PERFORATION DATA STRUCTURE
	std::vector <ntCircle*>			perfs;     //  LIST OF ALL PERF
	std::vector <vector <ntVec3*>*> p_Rows;    /// PERF POS BY ROWS  USED BY calc_Perf02
	std::vector<ntVec3*>			p_Pos;	   ///  LIST OF ALL POS  USED BY ORTHO FUNCTION
	std::vector<float>				p_Rad;	   //   LIST OF ALL RAD

	///////////////////////////////////////////////////////////////
	//////////////////////////////////////// PERFORATION PARAMETERS
	float	r_Min =			0.1625;
	float	r_Max =			0.625;
	float	edge_Offset =	0.75;
	int		n_seg =			36;

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
	void sub_Div(std::vector< vector <ntFace3>* >*	faces, int gen, bool isPanel);
	void sub_Div(int div, bool isDiv);

	void calc_Perf_00();
	void calc_Perf_SD(int div);
	void add_Perf();

	bool ntPanel::pt_isInside(ntVec3* point);

	void set_ID(string panel_ID);
	void set_vG();
	void set_nG(string n_G);
	void set_pG(string p_G);
	void set_UVW(string string_UVW);
	void set_IMG(float val);

	string get_ID();	//PANEL ID
	string get_n_G();	//GLOBAL NORMAL
	string get_p_G();
	string get_UVW();

	std::vector<ntVec3*>	get_v_G();	//GLOBAL VERTICES
	std::vector<ntVec3*>	get_Perf();
	std::vector<float>		get_Perf_R();

	void display();
	void display_Edge();
	void display_EdgeSd(int gen = 0);
	void display_Face_L(int gen = 0);
	void display_Face_G(int gen = 0);
	void display_Perf();
};
#endif
