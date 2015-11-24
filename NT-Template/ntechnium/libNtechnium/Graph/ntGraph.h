///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////// ntGraph.h
// openGl scratch libs							///////////////////
// Graph Data Display Class						///////////////////
// created by James Warton on 10/05/2015		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef GRAPH_DISPLAY_JPW_NTECHNIUM
#define GRAPH_DISPLAY_JPW_NTECHNIUM
#define _USE_MATH_DEFINES

#include <iostream>
#include <algorithm>
#include <math.h>

#include "ntColor4f.h"
#include "ntVec3.h"
#include "ntVertex.h"
#include "ntEdge.h"
#include "ntMath.h"

class ntGraph {
private:
	std::string data;

	std::vector <vertex*> verts_S;
	std::vector <vertex*> verts_E;
	std::vector <ntEdge*> bars;
	std::vector <ntEdge*> lines;

	std::string param1;		//time
	std::string param2;		//elements
	std::string param3;		//iterations

	//std::vector <vector <float>*> vals;
	std::vector <float> vals;
	std::vector <float> vals_0;
	std::vector <float> vals_1;
	std::vector <float> vals_2;
	std::vector <float> vals_3;
	////GRAPH POSITION
	ntVec3* pos;

	////GRAPH DIMENSIONS
	ntVec3* dim;
	float dimX = .75;
	float dimY = .1;
	float dimZ = .1;

	//GRAPH COLOR
	ntColor4f colS = Col4(0, 0, 1, 1);
	ntColor4f colE = Col4(0, 1, 0, 1);
	ntColor4f colT = Col4(0.5, 0.5, 0.5, 0.0);

	void parse_Data();
	void plot_Data();
	void test_Data();

public:
	bool is_ColSet = false;

	ntGraph();
	ntGraph(std::string data);
	ntGraph(float x, float y, float z);
	ntGraph(ntVec3* pos, ntVec3* dim, std::vector <float> vals);

	void set_Data(std::string data);
	void set_Param(float val, int param);
	void set_Param(std::vector <float> vals, int param);
	void set_Color(ntColor4f colT);

	void init();
	void run();
	void display();
};
#endif