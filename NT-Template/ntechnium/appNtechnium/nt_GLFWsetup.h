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
#include <math.h>
#include <algorithm>

#include "spwmacro.h"
#include "si.h"
#include "siapp.h"

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
		int xpos = 0;  //int xpos = 910;
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


		bool isProjection = false;

		///////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////// MOUSE EVENT
		double M_xpos = 0;
		double M_ypos = 0;
		double P_xpos = 0;
		double P_ypos = 0;
		double D_xpos = 0;
		double D_ypos = 0;

		bool state_Ctrl = false;
		bool state_Shift = false;
		bool state_Alt = false;

		double xoffset = 0;
		double yoffset = 0;
		double lastTime = 0;

		double K_xpos = .01;						// position
		double K_ypos = .01;
		float initialFoV = 45.0f;					// Initial Field of View
		float speed = .01f;							// 3 units / second
		float mouseSpeed = 0.00000001f;
		float horizontalAngle = 3.14f;				// horizontal angle : toward -Z
		float verticalAngle = 0.0f;					// vertical angle : 0, look at the horizon

		void print_Camera();

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

		void set_3D_DispView();	//3D MODEL SPACE
		void set_2D_DispView();	//2D SCREEN SPACE
		void set_3D_Proj();		//2D PROJECTION VIEW OF MODEL SPACE

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