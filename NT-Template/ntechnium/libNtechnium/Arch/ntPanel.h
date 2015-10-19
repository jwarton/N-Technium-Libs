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
#include "ntVertex.h"
#include "ntVec3.h"
#include "ntEdge.h"
#include "ntNormal.h"
 
using namespace jpw;

class ntPanel{
private:
	void init();

	string panel_ID;
	string n_G;						//GLOBAL PANEL NORMAL
	string p_G;
	std::vector<ntVec3*> v_G;		//GLOBAL PANEL VERTEX POSITON
	std::vector<ntVec3*> v_L;		// LOCAL PANEL VERTEX POSTION

	void set_vG();
public:

	ntVec3 *v0,*v1,*v2;
	ntVec3* vecs[3];
	ntVertex *vert0,*vert1,*vert2;
	std::vector<ntVertex*> verts;
	std::vector<ntEdge> edges;
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

	//float calcArea();

	void set_ID(string panel_ID);
	void set_nG(string n_G);
	void set_pG(string p_G);

	string get_ID();	//PANEL ID
	string get_n_G();	//GLOBAL NORMAL
	string get_p_G();
	std::vector<ntVec3*> get_v_G();	//GLOBAL VERTICES

	void display();
};
#endif
