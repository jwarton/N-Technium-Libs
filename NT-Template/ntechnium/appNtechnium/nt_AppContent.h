///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////// nt_AppConent.h
// openGl scratch libs							///////////////////
// Base Content Class for all application		///////////////////
// created by James Warton on 04/30/2014		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef APPLICATION_SCENE_CONTENT_JPW_NTECHNIUM
#define APPLICATION_SCENE_CONTENT_JPW_NTECHNIUM
#include <iostream>		
#include <algorithm>
#include <GLFW/glfw3.h>

#include "ntVec3.h"
#include "ntVertex.h"
#include "ntEdge.h"

class AppContent{
	/// TEMPORARY MODEL SPACE GRAPHIC
	ntEdge axisX;
	ntEdge axisY;
	ntEdge axisZ;
	/// BOUNDING BOX PARAMETERS
	ntVec3* BB_min = new ntVec3(0,0,0);
	ntVec3* BB_max = new ntVec3(0,0,0);
	ntVec3* BB_cen = new ntVec3(0,0,0);

protected:
	int appWidth;
	int appHeight;
public:
	GLFWwindow* window = NULL;

	AppContent();

	virtual void init();
	virtual void run();
	virtual void display();

	void set_AppDim(int x, int y);

	void set_Grid();
	void display_Grid();
	void view_Model();
	void view_Orth();

	void set_Bounds();
	void set_Centroid();
	void display_BBox();

	void get_Bounds(ntVec3* min, ntVec3* max);
	void get_Centroid(ntVec3* cen);
};
#endif
