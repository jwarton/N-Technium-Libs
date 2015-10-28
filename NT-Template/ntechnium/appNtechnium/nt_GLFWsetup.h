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
//#include <math.h>
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
		void reset();
		void update();
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

		//GLfloat rotX = 0.0f;
		//GLfloat rotY = 0.0f;
		//GLfloat rotZ = 0.0f;

		//GLfloat camX = -3.0f;
		//GLfloat camY = -3.0f;
		//GLfloat camZ =  3.0f;
		//GLfloat tarX =  0.0f;
		//GLfloat tarY =  0.0f;
		//GLfloat tarZ =  0.0f;
		//GLfloat rolX =  0.0f;
		//GLfloat rolY =  0.0f;
		//GLfloat rolZ =  1.0f;
		//GLfloat focal =50.0f;
		//GLfloat zoom =  0.0f;

		//float width = 1.0;
		//float height = 1.0;
		//float zNear = 1.0;
		//float zFar = 100.0;
		//

		//int view_T = GLFW_KEY_T;
		//int view_B = GLFW_KEY_B;
		//int view_L = GLFW_KEY_L;
		//int view_R = GLFW_KEY_R;
		//int view_F = GLFW_KEY_F;
		//int view_P = GLFW_KEY_P;

		//int view_ZI = GLFW_KEY_LEFT_BRACKET;
		//int view_ZO = GLFW_KEY_RIGHT_BRACKET;

		//int key = GLFW_KEY_RIGHT;
		//int action = GLFW_PRESS;
		//int state = glfwGetKey(window, key);
		HDC				hdc;
		HWND			hWin32;
		SiHdl			devHdl;
		TCHAR devicename[100];

	public:
		int frameCount;
		float frameRate;

		friend void mouseBtn_callback(GLFWwindow* window, int button, int action, int mods);
		friend void scroll_callback(GLFWwindow* window, float xoffset, float yoffset);

		void EventHandler_3DX();
		void EventHandler_MOUSE();
		void EventHandler_KEYBD();

		void Event_3DX_gimbalM(SiSpwEvent *pEvent);
		void Event_3DX_gimbalZ();
		void Event_3DX_buttonR(int num);
		void Event_3DX_buttonP(int num);
		void Event_3DX_dChange(SiSpwEvent *pEvent);
		void Event_3DX_command(SiSpwEvent *pEvent);

		///////////////////////////////////////////////////////////////
		////////////////////////////////////////////////// CONSTRUCTORS
		explicit ntGLFWsetup();
		explicit ntGLFWsetup(ntBaseApp* baseApp);
		//ntGLFWsetup(int appWidth, int appHeight);
		//ntGLFWsetup(int appWidth, int appHeight, ntBaseApp* baseApp);
		ntGLFWsetup(int appWidth, int appHeight, std::string appTitle, ntBaseApp* baseApp);

		//void scroll_callback(GLFWwindow* window, float xoffset, float yoffset);
		HWND getWindow();
	};
}
#endif

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////