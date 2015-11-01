///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////// ntGLFWsetup.h
// openGl scratch libs							///////////////////
// GLFW setup and context parameters			///////////////////
// created by James Warton on 04/02/2014		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef GLFW_SETUP_JPW_NTECHNIUM
#define GLFW_SETUP_JPW_NTECHNIUM
#define GLFW_INCLUDE_GLU

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#define _CRT_SECURE_NO_WARNINGS ///ONLY NEEDED FOR _stprintf

//////////////////////////////////////////////////// GLFW WINDOWING 

#include <GL/glew.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "nt_BaseApp.h"
#include <memory>

////3d connecion library includes
//#include <windows.h>
//#include <tchar.h>
//#include <stdio.h>
#include <math.h>
#include <algorithm>
//#include <float.h>
//#include <stdlib.h>
//#include <crtdbg.h>

#include "spwmacro.h"
#include "si.h"
#include "siapp.h"
//#include "virtualkeys.hpp"

#define GLFW_INCLUDE_GLCOREARB//MAY NOT BE REQUIRED

namespace jpw{
	///////////////////////////////////////////////////////////////////
	static void error_callback(int error, const char* description)
	{
		fputs(description, stderr);
	}
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
	}

	//static void scroll_callback(GLFWwindow* window, float xoffset, float yoffset){
	//	std::cout << "Y OFFSET:  " << yoffset << endl;
	//}

	///////////////////////////////////////////////////////////////////
	class ntGLFWsetup{
	private:

		void init();
		void run();

		GLFWwindow* window;
		///////////////////////////////////////////////////////////////
		///////////////////// BASE APP CONST FOR USER DEFINED APP CLASS
		ntBaseApp* baseApp;

		int appWidth;
		int appHeight;
		std::string appTitle;
		///DEFINE SEPARATE FUNCTION FOR WINDOW POSITION
		int xpos = 910;
		int ypos = 28;

		HDC				hdc;
		HWND			hWin32;
		SiHdl			devHdl;
		TCHAR  devicename[100];


		enum V_mode {vT, vB, vF, vL, vR, vP, c0, c1, c2, c3, c4};
		V_mode view = vP;
		bool is_vOrtho = false;

		enum B_mode {b0, b1, b2, b3};	// BUTTON MODES
		B_mode button = b0;

	public:

		int frameCount;
		float frameRate;

		void EventHandler_3DX();
		void EventHandler_MOUSE();
		void EventHandler_KEYBD();

		void Event_3DX_gimbalM(SiSpwEvent *pEvent);
		void Event_3DX_gimbalZ();
		void Event_3DX_buttonR(int num);
		void Event_3DX_buttonP(int num);
		void Event_3DX_dChange(SiSpwEvent *pEvent);
		void Event_3DX_command(SiSpwEvent *pEvent);

		void view_Reset();
		void view_Update();
		void view_Fit();
		void view_Top();
		void view_Bottom();
		void view_Right();
		void view_Left();
		void view_Front();
		//void view_Back();

		void view_Pers();
		void view_Orth();
		void view_Switch();

		void view_Save();

		void display_HUD();
		///////////////////////////////////////////////////////////////
		////////////////////////////////////////////////// CONSTRUCTORS
		explicit ntGLFWsetup();
		explicit ntGLFWsetup(ntBaseApp* baseApp);
		//ntGLFWsetup(int appWidth, int appHeight);
		//ntGLFWsetup(int appWidth, int appHeight, ntBaseApp* baseApp);
		ntGLFWsetup(int appWidth, int appHeight, std::string appTitle, ntBaseApp* baseApp);
	};
}
#endif

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////