///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////// nt_AppConent.h
// openGl scratch libs							///////////////////
// Base Content Class for all application		///////////////////
// created by James Warton on 04/30/2014		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef APPLICATION_SCENE_CONTENT_JPW_NTECHNIUM
#define APPLICATION_SCENE_CONTENT_JPW_NTECHNIUM
#include <iostream>		
#include <GLFW/glfw3.h>

#include "ntVec3.h"
#include "ntVertex.h"
#include "ntEdge.h"

class AppContent{
	/// TEMPORARY MODEL SPACE GRAPHIC
	ntEdge axisX;
	ntEdge axisY;
	ntEdge axisZ;
	///
public:
	GLFWwindow* window = NULL;

	AppContent();

	virtual void init();
	virtual void run();
	virtual void display();

	void grid();
	void grid_display();
};
#endif
