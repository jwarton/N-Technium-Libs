///////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////// ntGLFWsetup.cpp
// openGl scratch libs							///////////////////
// GLFW setup and context parameters			///////////////////
// created by James Warton on 04/02/2014		///////////////////
///////////////////////////////////////////////////////////////////

#include "nt_GLFWsetup.h"

namespace jpw{
	//GLFWscrollfun 
	void mouseBtn_callback(GLFWwindow* window, int button, int action, int mods) {
			ntBaseApp* ba = (ntBaseApp*)glfwGetWindowUserPointer(window);
			ba->setMouseButton(action, button, mods);
	}

	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
			ntBaseApp* ba = (ntBaseApp*)glfwGetWindowUserPointer(window);
			ba->setMouseScroll(xoffset, yoffset);
	}
}


using namespace jpw;
ntGLFWsetup::ntGLFWsetup(){}

ntGLFWsetup::ntGLFWsetup(ntBaseApp* baseApp) :
appWidth(1920), appHeight(1080), appTitle("NTECHNIUM"), baseApp(baseApp){
	init();
	run();
}

ntGLFWsetup::ntGLFWsetup(int appWidth, int appHeight, std::string appTitle, ntBaseApp* baseApp) :
appWidth(appWidth), appHeight(appHeight), appTitle(appTitle), baseApp(baseApp){
	baseApp->setWidth(appWidth);
	baseApp->setHeight(appHeight);
	//baseApp->setSize(Dim2i(appWidth, appHeight));
	init();
	run();
}

void ntGLFWsetup::init(){
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	baseApp->setFrameCount(0);/////////////////////////////////////////////////////////////////////////////////////
	baseApp->setFrameRate(60.0f);//////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	///srand (static_cast <unsigned> (time(0))); //time undefined

	glfwSetErrorCallback(error_callback);
	(glfwInit()) ? NULL : exit(EXIT_FAILURE);

	///////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////// ANTI-ALIASING
	glfwWindowHint(GLFW_SAMPLES, 8);

	window = glfwCreateWindow(appWidth, appHeight, appTitle.c_str(), NULL, NULL);
	///DEFINE FUNCTION FOR POSITON
	glfwSetWindowPos(window, xpos, ypos); ///added 12.13.2014
	//FULL SCREEN:  
	//window = glfwCreateWindow(appWidth, appHeight, appTitle.c_str(), glfwGetPrimaryMonitor(), NULL);
	(!window) ? glfwTerminate() : NULL;
	(!window) ? exit(EXIT_FAILURE) : NULL;

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	///////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////// SET GL STATES
	glClearColor(0, 0, 0, 1.0f);
	glShadeModel(GL_SMOOTH);

	///////////////////////////////////////////////////////////////////
	///////////////////////// DEFINE OPTIONAL GL RENDER PARAMETERS HERE
	///EXPLORE THESE PARAMETERS FOR EFFECT
	
	// enable specualrity on textures
	glLightModelf(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
	///glEnable(GL_LIGHTING);
	//glFrontFace(GL_CCW); // default
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glDisable(GL_CULL_FACE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	//glShadeModel(GL_FLAT); // option
	glEnable(GL_COLOR_MATERIAL); // incorporates per vertex color with lights
	
	// global ambient unrelated to lights
	float globalAmbient[4] = { .3, .3, .3, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

	// let glColor control diffues and ambient material values
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	//
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_DST_COLOR,GL_ZERO);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_DST_COLOR,GL_ZERO);
	//glBlendFunc (GL_ONE, GL_ONE);
	
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.0f);
	glEnable(GL_POLYGON_STIPPLE); //back door transperancy 

	///////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////footnote *01
	/////////////////////////////////// ANTIALIASING SPECIFICATIONS
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);			//  good for uniform scaling
	glClearStencil(0);				//  clear stencil buffer
	glClearDepth(1.0f);				//  0 is near, 1 is far
	glDepthFunc(GL_LESS);			//  glDepthFunc(GL_LEQUAL);  
	
	///
	/////////////////////////////////////////////////////////|ACTIVATE INIT FUNCTION IN DERIVED APPLICATIONS|//////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	baseApp->window = window;//////////////////////////////////////////////////////////////////////////////////////
	baseApp->init();///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
void ntGLFWsetup::reset(){
	baseApp->rotX = 0.0f;
	baseApp->rotY = 0.0f;
	baseApp->rotZ = 0.0f;
	baseApp->camX = -3.0f;
	baseApp->camY = -3.0f;
	baseApp->camZ = 3.0f;
	baseApp->tarX = 0.0f;
	baseApp->tarY = 0.0f;
	baseApp->tarZ = 0.0f;
	baseApp->rolX = 0.0f;
	baseApp->rolY = 0.0f;
	baseApp->rolZ = 1.0f;
	baseApp->focal = 50.0f;
	baseApp->zoom = 0.0f;
}
void ntGLFWsetup::update(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(baseApp->focal + baseApp->zoom, baseApp->width / baseApp->height, baseApp->zNear, baseApp->zFar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(baseApp->camX, baseApp->camY, baseApp->camZ, baseApp->tarX, baseApp->tarY, baseApp->tarZ, baseApp->rolX, baseApp->rolY, baseApp->rolZ);
	glRotatef(baseApp->rotX, 1.f, 0.f, 0.f);
	glRotatef(baseApp->rotY, 0.f, 1.f, 0.f);
	glRotatef(baseApp->rotZ, 0.f, 0.f, 1.f);
}
void ntGLFWsetup::run(){
	while (!glfwWindowShouldClose(window))
	{
		/////////////////////////////////////////////////////////|ACTIVATE DERIVED USER CLASS IMPLEMENTATION|//////
		baseApp->setFrameCount(frameCount);////////////////////////////////////////////////////////////////////////
		baseApp->run();////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		///MOUSE EVENT FUNCTIONALITY:
		int state_m1 = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
		int state_m2 = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
		int state_m3 = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
		int state_m4 = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_4);
		int state_m5 = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_5);
		int state_m6 = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_6);
		int state_m7 = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_7);
		int state_m8 = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LAST);

		///	GLFWscrollfun
		glfwSetScrollCallback(window, scroll_callback);

		// mouse press events
		glfwSetMouseButtonCallback(window, mouseBtn_callback);
		///


		if (state_m1 == GLFW_PRESS){
			//std::cout << "LEFT MOUSE BUTTON_1" << endl;
			baseApp->camZ -= 0.1;
			update();
		}
		if (state_m2 == GLFW_PRESS){
			//std::cout << "RIGHT MOUSE BUTTON_2" << endl;
			baseApp->camZ -= 0.1;
			update();
		}
		if (state_m3 == GLFW_PRESS){
			//std::cout << "MIDDLE MOUSE BUTTON_3" << endl;
			baseApp->camZ -= 0.1;
			update();
		}
		if (state_m4 == GLFW_PRESS){
			//std::cout << "BACK MOUSE BUTTON_4" << endl;
			update();
		}
		if (state_m5 == GLFW_PRESS){
			//std::cout << "FORWARD MOUSE BUTTON_5" << endl;
			update();
		}
		if (state_m6 == GLFW_PRESS){
			//std::cout << "MOUSE BUTTON_6" << endl;
			update();
		}
		if (state_m7 == GLFW_PRESS){
			//std::cout << "MOUSE BUTTON_7" << endl;
			update();
		}
		if (state_m8 == GLFW_PRESS){
			//std::cout << "MOUSE BUTTON_8" << endl;
			update();
		}
		///

		///KEYBOARD INPUT FUNCTIONALITY:
		if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS & glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
			reset();
			update();
		}

		//CAMERA JOG FUNCTIONALITY
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS & glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
			baseApp->camZ += 0.1;
			update();
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS & glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
			baseApp->camZ -= 0.1;
			update();
		}
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS & glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
			baseApp->camX += 0.1;
			update();
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS & glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
			baseApp->camX -= 0.1;
			update();
		}
		if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS & glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
			baseApp->camY += 0.1;
			update();
		}
		if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS & glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
			baseApp->camY -= 0.1;
			update();
		}
		//ORBIT FUNCTIONALITY
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
			baseApp->rotZ += 0.2;
			update();
			//glRotatef(.2, 0.f, 0.f, 1.f);
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
			baseApp->rotZ -= 0.2;
			update();
			//glRotatef(-.2, 0.f, 0.f, 1.f);
		}
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
			baseApp->rotX += 0.2;
			update();
			//glRotatef(.2, 1.f, 0.f, 0.f);
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
			baseApp->rotX -= 0.2;
			update();
			//glRotatef(-.2, 1.f, 0.f, 0.f);
		}
		if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS){
			//baseApp->rotY += 0.2;
			update(); 
			//glRotatef(.2, 0.f, 1.f, 0.f);
		}
		if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS){
			//baseApp->rotY -= 0.2;
			update(); 
			//glRotatef(-.2, 0.f, 1.f, 0.f);
		}

		//ZOOM FUNCTIONS
		if (glfwGetKey(window, baseApp->view_ZI) == GLFW_PRESS){
			baseApp->zoom += 0.1;
			update();
		}
		if (glfwGetKey(window, baseApp->view_ZO) == GLFW_PRESS){
			baseApp->zoom -= 0.1;
			update();
		}
		//STANDARD VIEW TOGGLE
		if (glfwGetKey(window, baseApp->view_P) == GLFW_PRESS){
			reset();
			update();
		}
		if (glfwGetKey(window, baseApp->view_T) == GLFW_PRESS){
			reset();
			baseApp->camX = 0.0f;
			baseApp->camY = 0.0f;
			baseApp->camZ = 3.0f;
			baseApp->rolX = 0.0f;
			baseApp->rolY = 1.0f;
			baseApp->rolZ = 0.0f;
			update();
		}
		if (glfwGetKey(window, baseApp->view_B) == GLFW_PRESS){
			reset();
			baseApp->camX = 0.0f;
			baseApp->camY = 0.0f;
			baseApp->camZ = -3.0f;
			baseApp->rolX = 0.0f;
			baseApp->rolY = 1.0f;
			baseApp->rolZ = 0.0f;
			update();
		}
		if (glfwGetKey(window, baseApp->view_L) == GLFW_PRESS){
			reset();
			baseApp->camX = 3.0f;
			baseApp->camY = 0.0f;
			baseApp->camZ = 0.0f;
			baseApp->rolX = 0.0f;
			baseApp->rolY = 0.0f;
			baseApp->rolZ = 1.0f;
			update();
		}
		if (glfwGetKey(window, baseApp->view_R) == GLFW_PRESS){
			reset();
			baseApp->camX = 0.0f;
			baseApp->camY = 3.0f;
			baseApp->camZ = 0.0f;
			baseApp->rolX = 0.0f;
			baseApp->rolY = 0.0f;
			baseApp->rolZ = 1.0f;
			update();
		}
		//if (glfwGetKey(window, baseApp->view_F) == GLFW_PRESS){
		//	reset();
		//	baseApp->camX = 0.0f;
		//	baseApp->camY = -3.0f;
		//	baseApp->camZ = 0.0f;
		//	baseApp->rolX = 0.0f;
		//	baseApp->rolY = 0.0f;
		//	baseApp->rolZ = 1.0f;
		//	update();
		//}

		/////HANDLE GLFW ENVENTS
		glfwSwapBuffers(window);
		glfwPollEvents();
		frameCount++;
	}
	
	glfwSetKeyCallback(window, key_callback);

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////footnote *01
//reference: http://bankslab.berkeley.edu/members/chris/AntiAliasing/AntiAliasingInOpenGL.html
