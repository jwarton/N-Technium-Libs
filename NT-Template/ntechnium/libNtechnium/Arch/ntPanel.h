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

	string panel_ID;
	string n_G;						//GLOBAL PANEL NORMAL
	string p_G;
	string string_UVW;				//GLOBAL PARAMETER
	std::vector<ntVec3*> v_G;		//GLOBAL PANEL VERTEX POSITON
	std::vector<ntVec3*> v_L;		// LOCAL PANEL VERTEX POSTION

	void set_vG();
	bool is_SubDiv = false;
	

public:
	int  cnt_SubDiv = 0;

	ntVec3		*v0,*v1,*v2;
	ntVec3		*vecs[3];
	ntVertex	*vert0,*vert1,*vert2;
	std::vector<ntVertex*>				verts;
	std::vector<ntEdge>					edges;

	std::vector <ntVec3*>			vecs_SD; //LOCAL COORDINATE
	std::vector <vector <ntFace3>*>	faces_L; //LOCAL COORDINATE
	std::vector <vector <ntFace3>*>	faces_G; //WORLD COORDINATE
	

	std::vector<ntCircle*>				perfs;
	std::vector<ntVec3>					vecs_UV;
	ntColor4f col;
	ntColor4f col_0;

	ntVec3 norm;
	ntVec3* cent;

	ntVertex centroid;
	ntNormal normal;
	float area;

	ntPanel();
	ntPanel(ntVec3* v0,ntVec3* v1,ntVec3* v2);

	void setColor(ntColor4f col);
	void calcNorm();
	void calcCentroid();
	//void calc_Area();
	void sub_Div(int gen);
	void sub_Div(std::vector< vector <ntFace3>* >*	faces, int gen, bool isPanel);
	void sub_Div(int div, bool isDiv);

	////////////////////////////////// PERFORATION PARAMETERS
	float image_Val;				// PIXEL VALUE AT CENTROID
	std::vector <float> image_Vals; // PIXEL VALUE AT SUBDIVISIONS

	void add_Perf();
	void calc_Perf01();
	void calc_Perf02();
	void calc_IMG();

	bool ntPanel::pt_isInside(ntVec3* point);

	std::vector <vector <ntVec3*>*> p_Rows;
	std::vector<ntVec3*>	p_Pos;
	std::vector<float>		p_Rad;

	float	r_Min = .1625;
	float	r_Max = .625;
	float	edge_Offset = .75;
	int		n_seg = 36;

	void set_ID(string panel_ID);
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
	void display_FaceL(int gen = 0);
	void display_FaceG(int gen = 0);
	void display_Perf();
};
#endif
