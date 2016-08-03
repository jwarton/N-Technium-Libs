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
ntGLFWsetup::ntGLFWsetup(std::string appTitle, ntBaseApp* baseApp) :
	appTitle(appTitle), baseApp(baseApp) {
	isFullScreen = true;
	init();
	run();
}
ntGLFWsetup::ntGLFWsetup(int appWidth, int appHeight, std::string appTitle, ntBaseApp* baseApp) :
appWidth(appWidth), appHeight(appHeight), appTitle(appTitle), baseApp(baseApp){
	baseApp->setWidth(appWidth);
	baseApp->setHeight(appHeight);
	//baseApp->setSize(Dim2i(appWidth, appHeight));
	isFullScreen = false;
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

	///////////////////////////////////////////////////////////////////
	//////////////////////////////////// SET WINDOW SIZE OR FULL SCREEN
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode *mode = glfwGetVideoMode(monitor);
	int count;
	GLFWmonitor** monitors = glfwGetMonitors(&count);
	if (count > 1) {
		monitor = monitors[1];				//FULL SCREEN ON SECOND MONITOR
		mode = glfwGetVideoMode(monitor);
	}
	if (isFullScreen == true) {
		window = glfwCreateWindow(mode->width, mode->height, appTitle.c_str(), monitor, NULL);
	} else {
		window = glfwCreateWindow(appWidth, appHeight, appTitle.c_str(), NULL, NULL);
		glfwSetWindowPos(window, xpos, ypos);
	}
	glfwGetFramebufferSize(window, &appWidth, &appHeight);
	///////////////////////////////////////////////////////////////////

	baseApp->setWidth(appWidth);
	baseApp->setHeight(appHeight);
	hWin32 = glfwGetWin32Window(window);


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
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
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
	glDisable(GL_DEPTH_CLAMP);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);			//  good for uniform scaling
	glClearStencil(0);				//  clear stencil buffer
	glClearDepth(1.0f);				//  0 is near, 1 is far
	glDepthFunc(GL_LESS);			//  glDepthFunc(GL_LEQUAL);  
	
	///////////////////////////////////////////////////////////////
	/////////////////////////////////////////////// SET VIEW MATRIX
	glLoadIdentity();// clear matrix
	///view_Reset();
	view_Pers();
	view_Update();
	/////////////////////////////////////////////////////////|ACTIVATE INIT FUNCTION IN DERIVED APPLICATIONS|//////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	baseApp->window = window;//////////////////////////////////////////////////////////////////////////////////////
	baseApp->init();///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//view_Save();

	///////////////////////////////////////////////////////////////////
	//////////////////////////////////////// COUT SYSTEM SPECIFICATIONS
	string pc_Id       = getPC_Name();
	string cpu_Spec    = getCPU();
	string thread_Cnt  = getThreadCnt();
	string memory_Size = getRAM();
	
	std::cout << "\n\n";
	std::cout << "///////////////////////////////////////////////////////////////\n";
	std::cout << "WORKSTATION NAME:           " << pc_Id       << endl;
	std::cout << "CPU SPECIFICATION:          " << cpu_Spec    << endl;
	std::cout << "CORES:                      " << thread_Cnt  << endl;
	std::cout << "MEMORY:                     " << memory_Size << " GB\n";
	std::cout << "MONITOR RESOLUTION:         " << appWidth << " x " << appHeight << "\n" << endl;

	////////////////////////////////////////////////////////////////////// 3D CONNEXION TEST AND INITIALIZATION
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	SiOpenData		oData;
	SiInitialize();
	SiOpenWinInit(&oData, hWin32);
	SiSetUiMode(devHdl, SI_UI_ALL_CONTROLS);
	devHdl = SiOpen("NT_FRAMEWORK_02", SI_ANY_DEVICE, SI_NO_MASK, SI_EVENT, &oData);
	
	SiDeviceName devName;
	SiGetDeviceName(devHdl, &devName);
}
void ntGLFWsetup::run(){
	while (!glfwWindowShouldClose(window)) {
		/////////////////////////////////////////////////////////|ACTIVATE DERIVED USER CLASS IMPLEMENTATION|//////
		baseApp->setFrameCount(frameCount);////////////////////////////////////////////////////////////////////////
		baseApp->run();////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////////

		EventHandler_3DX();
		EventHandler_MOUSE();
		EventHandler_KEYBD();

		set_2D_DispView();
		display_HUD();		/// HEADS UP DISPLAY AND GRAPHIC CONTENT

		if (isProjection == false) {
			set_3D_DispView();
			view_Update();
		} else {
			set_3D_Proj();
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
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

void ntGLFWsetup::EventHandler_3DX() {
	MSG				msg;			//INCOMING MESSAGE TO EVALUATE
	BOOL			handled;		//IS MESSAGE HANDLED
	SiSpwEvent		Event;			//EVENT TYPE
	SiGetEventData	EData;			//EVENT DATA
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////// 3D CONNEXION DEVICE FUNCTIONALITY
	GetMessage(&msg, NULL, 0, 0);
	//while (GetMessage(&msg, NULL, 0, 0)) {
	handled = SPW_FALSE;
	SiGetEventWinInit(&EData, msg.message, msg.wParam, msg.lParam);
	hdc = GetDC(hWin32);
	if (SiGetEvent(devHdl, SI_AVERAGE_EVENTS, &EData, &Event) == SI_IS_EVENT) {
		//std::cout << "3DX EVENT RECEIVED" << endl;
		if (Event.type == SI_MOTION_EVENT) {
			Event_3DX_gimbalM(&Event);										///// PROCESS 3DX MOTION EVENT
		} 		
		else if (Event.type == SI_ZERO_EVENT) {
			Event_3DX_gimbalZ();											///// PROCESS 3DX ZERO EVENT
		} 		
		else if (Event.type == SI_BUTTON_PRESS_EVENT) {
			std::cout << "3DX BUTTON PRESS" << Event.u.hwButtonEvent.buttonNumber << endl;
			Event_3DX_buttonP(Event.u.hwButtonEvent.buttonNumber);			///// PROCESS BUTTON PRESS EVENT	
		} 		
		else if (Event.type == SI_BUTTON_RELEASE_EVENT) {
			std::cout << "3DX BUTTON RELEASE" << Event.u.hwButtonEvent.buttonNumber << endl;
			Event_3DX_buttonR(Event.u.hwButtonEvent.buttonNumber);			///// PROCESS BUTTON RELEASE EVENT
		}
		else if (Event.type == SI_BUTTON_EVENT) {
			std::cout << "3DX BUTTON" << Event.u.hwButtonEvent.buttonNumber << endl;
			Event_3DX_buttonR(Event.u.hwButtonEvent.buttonNumber);
		}
		else if (Event.type == SI_DEVICE_CHANGE_EVENT) {
			std::cout << "3DX DEVICE" << endl;
			Event_3DX_dChange(&Event);										///// PROCESS 3DX DEVICE CHANGE EVENT
		} 		
		else if (Event.type == SI_CMD_EVENT) {
			std::cout << "3DX COMMAND" << endl;
			Event_3DX_command(&Event);										///// V3DX COMMAND EVENTS
		}

		handled = SPW_TRUE;

	} else if (SiGetEvent(devHdl, SI_AVERAGE_EVENTS, &EData, &Event) == SPW_ERROR) {
		std::cout << "3DX ERROR" << endl;
	} else if (SiGetEvent(devHdl, SI_AVERAGE_EVENTS, &EData, &Event) == SI_BAD_HANDLE) {
		//std::cout << "3DX BAD DEVICE HANDLE" << endl;
	} else if (SiGetEvent(devHdl, SI_AVERAGE_EVENTS, &EData, &Event) == SI_SKIP_EVENT) {
		std::cout << "3DX SKIP EVENT RECEIVED" << endl;
	} 
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////// NOT A 3DX EVENT | WINDOWS
	if (handled == SPW_FALSE) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
void ntGLFWsetup::EventHandler_KEYBD(){
	// KEYBOARD INPUT FUNCTIONALITY:
	bool CTRL_status = false;
	bool ALT_status = false;
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) {
		CTRL_status = true;
	}
	else {
		CTRL_status = false;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS) {
		ALT_status = true;
	}
	else {
		ALT_status = false;
	}
	// FIT SELECTED
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS && CTRL_status == true) {
		view_Fit();
		view_Update();
	}
	// SAVE CAMERA
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && CTRL_status == true) {
		view_Save();
		view_Update();
	}
	// PERSPECTIVE RESET
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && CTRL_status == true) {
		view_Pers();
		view_Update();
	}
	///////////////////////////////////////////////////////////////
	////////////////////////////////////// CAMERA JOG FUNCTIONALITY
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS & glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		//baseApp->camZ += 1;
		//view_Update();
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS & glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		//baseApp->camZ -= 1;
		//view_Update();
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS & glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		//baseApp->camX += 1;
		//view_Update();
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS & glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		//baseApp->camX -= 1;
		//view_Update();
	}
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS & glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		baseApp->camY += 1;
		view_Update();
	}
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS & glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		baseApp->camY -= 1;
		view_Update();
	}

	if (glfwGetKey(window, GLFW_KEY_F8) == GLFW_PRESS) {
		set_3D_Proj();
	}
	if (glfwGetKey(window, GLFW_KEY_F8) == GLFW_PRESS) {
		isProjection = true;
	}
	if (glfwGetKey(window, GLFW_KEY_F9) == GLFW_PRESS) {
		isProjection = false;
	}
	///////////////////////////////////////////////////////////////
	/////////////////////////////////////////// ORBIT FUNCTIONALITY
	/*
	horizontalAngle = 3.14f;
	verticalAngle = 0.0f;
	glm::vec3 position = glm::vec3(baseApp->camX, baseApp->camY, baseApp->camZ);
	// Compute new orientation
	//horizontalAngle += mouseSpeed * deltaTime * float(1024 / 2 - xpos);
	//verticalAngle += mouseSpeed * deltaTime * float(768 / 2 - ypos);
	horizontalAngle += speed * D_xpos;
	verticalAngle += speed * D_ypos;
	glm::vec3 direction(cos(verticalAngle) * sin(horizontalAngle), sin(verticalAngle), cos(verticalAngle) * cos(horizontalAngle));
	glm::vec3 right = glm::vec3(sin(horizontalAngle - 3.14f / 2.0f), 0, cos(horizontalAngle - 3.14f / 2.0f));
	glm::vec3 up = glm::cross(right, direction);
	*/
	//std::cout << horizontalAngle << endl;
	//std::cout << direction.x << ", " << direction.y << ", " << direction.z << " | " << right.x << ", " << right.y << ", " << right.z << endl;

	//// TRUCK FORWARD
	//if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {

	//	position += direction * speed;
	//	baseApp->camX = position.x;
	//	baseApp->camY = position.y;
	//	baseApp->camZ = position.z;
	//	baseApp->tarX = position.x + direction.x;
	//	baseApp->tarY = position.y + direction.y;
	//	baseApp->tarZ = position.z + direction.z;
	//	baseApp->rolX = up.x;
	//	baseApp->rolY = up.y;
	//	baseApp->rolZ = up.z;
	//	view_Update();
	//}
	//// TRUCK BACKWARD
	//if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {

	//	position -= direction * speed;
	//	baseApp->camX = position.x;
	//	baseApp->camY = position.y;
	//	baseApp->camZ = position.z;
	//	baseApp->tarX = position.x + direction.x;
	//	baseApp->tarY = position.y + direction.y;
	//	baseApp->tarZ = position.z + direction.z;
	//	baseApp->rolX = up.x;
	//	baseApp->rolY = up.y;
	//	baseApp->rolZ = up.z;
	//	view_Update();
	//}

	// TRUCK RIGHT
	//if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {

	//	position += right * speed;
	//	baseApp->camX = position.x;
	//	baseApp->camY = position.y;
	//	baseApp->camZ = position.z;
	//	baseApp->tarX = position.x + direction.x;
	//	baseApp->tarY = position.y + direction.y;
	//	baseApp->tarZ = position.z + direction.z;
	//	baseApp->rolX = up.x;
	//	baseApp->rolY = up.y;
	//	baseApp->rolZ = up.z;
	//	view_Update();
	//}
	//// TRUCK LEFT
	//if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {

	//	position -= right * speed;
	//	baseApp->camX = position.x;
	//	baseApp->camY = position.y;
	//	baseApp->camZ = position.z;
	//	baseApp->tarX = position.x + direction.x;
	//	baseApp->tarY = position.y + direction.y;
	//	baseApp->tarZ = position.z + direction.z;
	//	baseApp->rolX = up.x;
	//	baseApp->rolY = up.y;
	//	baseApp->rolZ = up.z;
	//	view_Update();
	//}

	///////////////////////////////////////////////////////////////
	//////////////////////////////////////////////// SET KEY STATES
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == NULL && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == NULL) {
		state_Shift = false;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		state_Shift = true;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == NULL && glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == NULL) {
		state_Ctrl = false;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) {
		state_Ctrl = true;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == NULL && glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == NULL) {
		state_Alt = false;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS) {
		state_Alt = true;
	}
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		if (baseApp->isGridActive == false) {
			baseApp->isGridActive = true;
		} else {
			baseApp->isGridActive = false;
		}
	}

	///////////////////////////////////////////////////////////////
	///////////////////////////////////////////// KEY FUNCTIONALITY
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		baseApp->rotZ += 0.2;
		view_Update();
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		baseApp->rotZ -= 0.2;
		view_Update();
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		baseApp->rotX += 0.2;
		view_Update();
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		baseApp->rotX -= 0.2;
		view_Update();
	}
	///////////////////////////////////////////////////////////////
	//////////////////////////////////////////////// ZOOM FUNCTIONS
	if (glfwGetKey(window, baseApp->view_ZI) == GLFW_PRESS) {
		baseApp->zoom += 0.1;
		view_Update();
	}
	if (glfwGetKey(window, baseApp->view_ZO) == GLFW_PRESS) {
		baseApp->zoom -= 0.1;
		view_Update();
	}
	///////////////////////////////////////////////////////////////
	////////////////////////////////////////// STANDARD VIEW TOGGLE
	if (glfwGetKey(window, baseApp->view_P) == GLFW_PRESS) {
		button = b0;
		view_Pers();
	}
	if (glfwGetKey(window, baseApp->view_T) == GLFW_PRESS) {
		button = b1;
		view_Top();
	}
	if (glfwGetKey(window, baseApp->view_B) == GLFW_PRESS) {
		button = b1;
		view_Bottom();
	}
	if (glfwGetKey(window, baseApp->view_L) == GLFW_PRESS) {
		button = b1;
		view_Left();
	}
	if (glfwGetKey(window, baseApp->view_R) == GLFW_PRESS) {
		button = b1;
		view_Right();
	}
	if (glfwGetKey(window, baseApp->view_F) == GLFW_PRESS) {
		button = b1;
		view_Front();
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && state_Alt == true) {
		view_Save();
	}
}
void ntGLFWsetup::EventHandler_MOUSE(){

	// MOUSE EVENT FUNCTIONALITY:
	int state_m1 = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	int state_m2 = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	int state_m3 = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
	int state_m4 = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_4);
	int state_m5 = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_5);
	int state_m6 = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_6);
	int state_m7 = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_7);
	int state_m8 = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LAST);
	int state_m9 = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);

	// GLFW SCROLL FUNCTIONALITY
	glfwSetScrollCallback(window, scroll_callback);

	// MOUSE PRESS EVENTS
	glfwSetMouseButtonCallback(window, mouseBtn_callback);

	if (state_m1 == GLFW_PRESS) {
		//std::cout << "LEFT MOUSE BUTTON_1" << endl;
		//baseApp->camZ -= 0.1;
		//view_Update();
	}
	if (state_m2 == GLFW_PRESS) {
		//std::cout << "RIGHT MOUSE BUTTON_2" << endl;
		//baseApp->camZ -= 0.1;
		//view_Update();
	}
	if (state_m3 == GLFW_PRESS) {
		//std::cout << "MIDDLE MOUSE BUTTON_3" << endl;
		//baseApp->camZ -= 0.1;
		//view_Update();
	}
	if (state_m4 == GLFW_PRESS) {
		//std::cout << "BACK MOUSE BUTTON_4" << endl;
		view_Fit();
	}
	if (state_m5 == GLFW_PRESS) {
		//std::cout << "FORWARD MOUSE BUTTON_5" << endl;
	}
	if (state_m6 == GLFW_PRESS) {
		//std::cout << "MOUSE BUTTON_6" << endl;
	}
	if (state_m7 == GLFW_PRESS) {
		//std::cout << "MOUSE BUTTON_7" << endl;
	}
	if (state_m8 == GLFW_PRESS) {
		//std::cout << "MOUSE BUTTON_8" << endl;
	}
	if (state_m9 == GLFW_RELEASE) {
		P_xpos = 0.0;
		P_ypos = 0.0;
	}
	/// ORBIT  ///NEEDS WORK
	if (state_m2 == GLFW_PRESS  && state_Shift == false && state_Ctrl == false) {
		glfwGetCursorPos(window, &M_xpos, &M_ypos);
		if (P_xpos == 0) {
			P_xpos = M_xpos;
			P_ypos = M_ypos;
		}

		D_xpos = M_xpos - P_xpos;
		D_ypos = M_ypos - P_ypos;

		baseApp->rotX += (D_ypos * .01);
		baseApp->rotZ += (D_xpos * .01);

	}
	/// ZOOM
	if (state_m2 == GLFW_PRESS && state_Ctrl == true) {
		glfwGetCursorPos(window, &M_xpos, &M_ypos);
		D_ypos = M_ypos - P_ypos;
		if (P_ypos == 0) {
			P_xpos = M_xpos;
			P_ypos = M_ypos;
		}
		if (M_ypos > P_ypos && baseApp->zoom > 0) {
			baseApp->zoom -= (D_xpos * .1);
			if (baseApp->zoom < 0) {
				baseApp->zoom = 0;
			}
		}
		if (M_ypos < P_ypos && baseApp->zoom < 165 - baseApp->fovA) {
			baseApp->zoom += (D_xpos * .1);
			if (baseApp->zoom > 165 - baseApp->fovA) {
				baseApp->zoom = 165 - baseApp->fovA;
			}
		}
	}
	/// PAN
	if (state_m2 == GLFW_PRESS && state_Shift == true) {
		glfwGetCursorPos(window, &M_xpos, &M_ypos);
		if (P_ypos == 0) {
			P_xpos = M_xpos;
			P_ypos = M_ypos;
		}

		D_xpos = M_xpos - P_xpos;
		D_ypos = M_ypos - P_ypos;

		baseApp->camX -= (D_xpos * .005);
		baseApp->camZ += (D_ypos * .005);
		baseApp->tarX -= (D_xpos * .005);
		baseApp->tarZ += (D_ypos * .005);
	}
}

void ntGLFWsetup::Event_3DX_gimbalM(SiSpwEvent *pEvent){
	int tx = pEvent->u.spwData.mData[SI_TX];
	int ty = pEvent->u.spwData.mData[SI_TY];
	int tz = pEvent->u.spwData.mData[SI_TZ];
	int rx = pEvent->u.spwData.mData[SI_RX];
	int ry = pEvent->u.spwData.mData[SI_RY];
	int rz = pEvent->u.spwData.mData[SI_RZ];

		// STANDARD ORBIT
		if (button == b0) {
			baseApp->camX += tx * .0001 + (tx * .0001);
			baseApp->camY += ty * .0001 + (ty * .0001);
			baseApp->zoom += tz * .001;
			baseApp->rotX += rx * .001;
			baseApp->rotY += ry * .001;
			baseApp->rotZ -= rz * .001;
		}
		// 3D PAN
		else if (button == b1) {
			baseApp->tarX += tx * .0001;
			baseApp->tarY += ty * .0001;
			baseApp->tarZ += tz * .0001;
			baseApp->camX += tx * .0001;
			baseApp->camY += ty * .0001;
			baseApp->camZ += tz * .0001;
		}
		else if (button == b2) {
		// RETARGET TO ORIGIN
			if (baseApp->tarX != 0) {
				if (baseApp->tarX < .02 & baseApp->tarX > -.02) {
					baseApp->tarX = 0;
				}
				else if (baseApp->tarX < 0) {
					baseApp->tarX += abs(ty * .0005);
				}
				else if (baseApp->tarX > 0) {
					baseApp->tarX -= abs(ty * .0005);
				}
			}
			if (baseApp->tarY != 0) {
				if (baseApp->tarY < .02 & baseApp->tarY > -.02) {
					baseApp->tarY = 0;
				}
				else if (baseApp->tarY < 0) {
					baseApp->tarY += abs(ty * .0005);
				}
				else if (baseApp->tarY> 0) {
					baseApp->tarY -= abs(ty * .0005);
				}
			}
			if (baseApp->tarZ != 0) {
				if (baseApp->tarZ < .02 & baseApp->tarZ > -.02) {
					baseApp->tarZ = 0;
				}
				else if (baseApp->tarZ < 0) {
					baseApp->tarZ += abs(ty * .0005);
				}
				else if (baseApp->tarZ > 0) {
					baseApp->tarZ -= abs(ty * .0005);
				}
			}
		}
		else if (button == b3) {
			baseApp->fovA += tz * .0001;
		}

	view_Update();
	ReleaseDC(hWin32, hdc);
}
void ntGLFWsetup::Event_3DX_gimbalZ(){
	//std::cout << "3DX ZERO EVENT" << endl;
	ReleaseDC(hWin32, hdc);
}
void ntGLFWsetup::Event_3DX_buttonR(int num){
	if (num == 2) {
		if (button == b0) {
			button = b1;
		} else {
			button = b0;
		}
	}
	if (num == 4) {
		if (button == b2) {
			button = b3;
		} else {
			button = b2;
		}
	}
	ReleaseDC(hWin32, hdc);
}
void ntGLFWsetup::Event_3DX_buttonP(int num){
	std::cout << "3DX BUTTON PRESS" << endl;
	ReleaseDC(hWin32, hdc);
}
void ntGLFWsetup::Event_3DX_dChange(SiSpwEvent *pEvent) {
	TCHAR buf[100];
	hdc = GetDC(hWin32);

	switch (pEvent->u.deviceChangeEventData.type)
	{
	case SI_DEVICE_CHANGE_CONNECT:
		_stprintf(buf, _T("Device ID %d connected"), pEvent->u.deviceChangeEventData.devID);
		TextOut(hdc, 0, 20, buf, (int)_tcslen(buf));
		break;
	case SI_DEVICE_CHANGE_DISCONNECT:
		_stprintf(buf, _T("Device ID %d disconnected"), pEvent->u.deviceChangeEventData.devID);
		TextOut(hdc, 0, 20, buf, (int)_tcslen(buf));
		break;
	default:
		_stprintf(buf, _T("Unknown deviceChangeEvent type: %d"), pEvent->u.deviceChangeEventData.type);
		TextOut(hdc, 0, 20, buf, (int)_tcslen(buf));
		break;
	}
	ReleaseDC(hWin32, hdc);
}
void ntGLFWsetup::Event_3DX_command(SiSpwEvent *pEvent) {
	TCHAR buf[100];
	hdc = GetDC(hWin32);
	switch (pEvent->u.cmdEventData.functionNumber)
	{
	case V3DCMD_VIEW_QZ_IN:
		_stprintf(buf, _T("V3DCMD_VIEW_QZ_IN(%s)"), pEvent->u.cmdEventData.pressed ? L"Pressed" : L"Released");
		TextOut(hdc, 0, 0, buf, (int)_tcslen(buf));
		break;
	case V3DCMD_VIEW_QZ_OUT:
		_stprintf(buf, _T("V3DCMD_VIEW_QZ_OUT(%s)"), pEvent->u.cmdEventData.pressed ? L"Pressed" : L"Released");
		TextOut(hdc, 0, 0, buf, (int)_tcslen(buf));
		break;
	case V3DCMD_MOTIONMACRO_ZOOM:
		_stprintf(buf, _T("V3DCMD_MOTIONMACRO_ZOOM(ZoomFrom=%d, ZoomTo=%d, Scale=%f)"), pEvent->u.cmdEventData.iArgs[0], pEvent->u.cmdEventData.iArgs[1], pEvent->u.cmdEventData.fArgs[0]);
		TextOut(hdc, 0, 0, buf, (int)_tcslen(buf));
		break;
	case V3DCMD_MOTIONMACRO_ZOOMIN_CENTERTOCENTER:
		_stprintf(buf, _T("V3DCMD_MOTIONMACRO_ZOOMIN_CENTERTOCENTER(Scale=%f)"), pEvent->u.cmdEventData.fArgs[0]);
		TextOut(hdc, 0, 0, buf, (int)_tcslen(buf));
		break;
	case V3DCMD_MOTIONMACRO_ZOOMIN_CURSORTOCENTER:
		_stprintf(buf, _T("V3DCMD_MOTIONMACRO_ZOOMIN_CURSORTOCENTER(Scale=%f)"), pEvent->u.cmdEventData.fArgs[0]);
		TextOut(hdc, 0, 0, buf, (int)_tcslen(buf));
		break;
	case V3DCMD_MOTIONMACRO_ZOOMIN_CURSORTOCURSOR:
		_stprintf(buf, _T("V3DCMD_MOTIONMACRO_ZOOMIN_CURSORTOCURSOR(Scale=%f)"), pEvent->u.cmdEventData.fArgs[0]);
		TextOut(hdc, 0, 0, buf, (int)_tcslen(buf));
		break;
	case V3DCMD_MOTIONMACRO_ZOOMOUT_CENTERTOCENTER:
		_stprintf(buf, _T("V3DCMD_MOTIONMACRO_ZOOMOUT_CENTERTOCENTER(Scale=%f)"), pEvent->u.cmdEventData.fArgs[0]);
		TextOut(hdc, 0, 0, buf, (int)_tcslen(buf));
		break;
	case V3DCMD_MOTIONMACRO_ZOOMOUT_CURSORTOCENTER:
		_stprintf(buf, _T("V3DCMD_MOTIONMACRO_ZOOMOUT_CURSORTOCENTER(Scale=%f)"), pEvent->u.cmdEventData.fArgs[0]);
		TextOut(hdc, 0, 0, buf, (int)_tcslen(buf));
		break;
	case V3DCMD_MOTIONMACRO_ZOOMOUT_CURSORTOCURSOR:
		_stprintf(buf, _T("V3DCMD_MOTIONMACRO_ZOOMOUT_CURSORTOCURSOR(Scale=%f)"), pEvent->u.cmdEventData.fArgs[0]);
		TextOut(hdc, 0, 0, buf, (int)_tcslen(buf));
		break;
	}
	ReleaseDC(hWin32, hdc);

}
/////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////// STANDARD VIEW FUNCTIONS

void ntGLFWsetup::print_Camera() {
	std::cout << "\n CAMERA SAVED:  \n "
		<< " rotX " << baseApp->rotX << " \n "
		<< " rotY " << baseApp->rotY << " \n "
		<< " rotZ " << baseApp->rotZ << " \n "
		<< " camX " << baseApp->camX << " \n "
		<< " camY " << baseApp->camY << " \n "
		<< " camZ " << baseApp->camZ << " \n "
		<< " tarX " << baseApp->tarX << " \n "
		<< " tarY " << baseApp->tarY << " \n "
		<< " tarZ " << baseApp->tarZ << " \n "
		<< " rolX " << baseApp->rolX << " \n "
		<< " rolY " << baseApp->rolY << " \n "
		<< " rolZ " << baseApp->rolZ << " \n "
		<< " fovA " << baseApp->fovA << " \n "
		<< " zoom " << baseApp->zoom << " \n " << endl;
}

void ntGLFWsetup::view_Reset() {
	//baseApp->rotX = 0.0f;
	//baseApp->rotY = 0.0f;
	//baseApp->rotZ = 0.0f;
	//baseApp->camX = -1.0f;
	//baseApp->camY = -5.0f;
	//baseApp->camZ = 2.0f;
	//baseApp->tarX = 0.0f;
	//baseApp->tarY = 0.0f;
	//baseApp->tarZ = 0.0f;
	//baseApp->rolX = 0.0f;
	//baseApp->rolY = 0.0f;
	//baseApp->rolZ = 1.0f;
	//baseApp->fovA = 40.0f;
	//baseApp->zoom = 20.0f;
}
void ntGLFWsetup::view_Fit(){

	//Vec3 target = Vec3(baseApp->tarX, baseApp->tarY, baseApp->tarZ);
	//Vec3 eye    = Vec3(baseApp->camX, baseApp->camY, baseApp->camZ);
	//Vec3 dir_V; 

	//eye.sub(&target);
	//dir_V.set(&eye);

	//double fov_X = 2;		///REPLACE WITH POINT CLOUD RADIUS FOR OBJECTS WITHIN SCENE
	//double fov_Y = 2;		///REPLACE WITH POINT CLOUD RADIUS FOR OBJECTS WITHIN SCENE
	//double rad = min(fov_X, fov_Y) * 0.5;
	//double theta = sin(rad);
	//double dist = (rad / theta);

	//dir_V.mult(dist);
	//target.add(&dir_V);
	//eye.set(&target);

	//baseApp->camX = eye.x;
	//baseApp->camY = eye.y;
	//baseApp->camZ = eye.z;

	//view_Update();
}
void ntGLFWsetup::view_Top(){
	if (view == vP) {
		view_Save();
	}
	view = vT;
	is_vOrtho = true;

	baseApp->rotX = baseApp->cam01[0];
	baseApp->rotY = baseApp->cam01[1];
	baseApp->rotZ = baseApp->cam01[2];
	baseApp->camX = baseApp->cam01[3];
	baseApp->camY = baseApp->cam01[4];
	baseApp->camZ = baseApp->cam01[5];
	baseApp->tarX = baseApp->cam01[6];
	baseApp->tarY = baseApp->cam01[7];
	baseApp->tarZ = baseApp->cam01[8];
	baseApp->rolX = baseApp->cam01[9];
	baseApp->rolY = baseApp->cam01[10];
	baseApp->rolZ = baseApp->cam01[11];
	baseApp->fovA = baseApp->cam01[12];
	baseApp->zoom = baseApp->cam01[13];

	view_Update();
}
void ntGLFWsetup::view_Bottom(){
	if (view == vP) {
		view_Save();
	}
	view = vB;
	is_vOrtho = true;

	baseApp->rotX = baseApp->cam05[0];
	baseApp->rotY = baseApp->cam05[1];
	baseApp->rotZ = baseApp->cam05[2];
	baseApp->camX = baseApp->cam05[3];
	baseApp->camY = baseApp->cam05[4];
	baseApp->camZ = baseApp->cam05[5];
	baseApp->tarX = baseApp->cam05[6];
	baseApp->tarY = baseApp->cam05[7];
	baseApp->tarZ = baseApp->cam05[8];
	baseApp->rolX = baseApp->cam05[9];
	baseApp->rolY = baseApp->cam05[10];
	baseApp->rolZ = baseApp->cam05[11];
	baseApp->fovA = baseApp->cam05[12];
	baseApp->zoom = baseApp->cam05[13];
	baseApp->rolZ = 0.0f;
	view_Update();
}
void ntGLFWsetup::view_Right(){
	if (view == vP) {
		view_Save();
	}
	view = vR;
	is_vOrtho = true;

	baseApp->rotX = baseApp->cam03[0];
	baseApp->rotY = baseApp->cam03[1];
	baseApp->rotZ = baseApp->cam03[2];
	baseApp->camX = baseApp->cam03[3];
	baseApp->camY = baseApp->cam03[4];
	baseApp->camZ = baseApp->cam03[5];
	baseApp->tarX = baseApp->cam03[6];
	baseApp->tarY = baseApp->cam03[7];
	baseApp->tarZ = baseApp->cam03[8];
	baseApp->rolX = baseApp->cam03[9];
	baseApp->rolY = baseApp->cam03[10];
	baseApp->rolZ = baseApp->cam03[11];
	baseApp->fovA = baseApp->cam03[12];
	baseApp->zoom = baseApp->cam03[13];
	view_Update();
}
void ntGLFWsetup::view_Left(){
	if (view == vP) {
		view_Save();
	}
	view = vL;
	is_vOrtho = true;

	baseApp->rotX = baseApp->cam04[0];
	baseApp->rotY = baseApp->cam04[1];
	baseApp->rotZ = baseApp->cam04[2];
	baseApp->camX = baseApp->cam04[3];
	baseApp->camY = baseApp->cam04[4];
	baseApp->camZ = baseApp->cam04[5];
	baseApp->tarX = baseApp->cam04[6];
	baseApp->tarY = baseApp->cam04[7];
	baseApp->tarZ = baseApp->cam04[8];
	baseApp->rolX = baseApp->cam04[9];
	baseApp->rolY = baseApp->cam04[10];
	baseApp->rolZ = baseApp->cam04[11];
	baseApp->fovA = baseApp->cam04[12];
	baseApp->zoom = baseApp->cam04[13];
	view_Update();
}
void ntGLFWsetup::view_Front(){
	if (view == vP) {
		view_Save();
	}
	view = vF;
	is_vOrtho = true;

	baseApp->rotX = baseApp->cam02[0];
	baseApp->rotY = baseApp->cam02[1];
	baseApp->rotZ = baseApp->cam02[2];
	baseApp->camX = baseApp->cam02[3];
	baseApp->camY = baseApp->cam02[4];
	baseApp->camZ = baseApp->cam02[5];
	baseApp->tarX = baseApp->cam02[6];
	baseApp->tarY = baseApp->cam02[7];
	baseApp->tarZ = baseApp->cam02[8];
	baseApp->rolX = baseApp->cam02[9];
	baseApp->rolY = baseApp->cam02[10];
	baseApp->rolZ = baseApp->cam02[11];
	baseApp->fovA = baseApp->cam02[12];
	baseApp->zoom = baseApp->cam02[13];
	view_Update();
}
void ntGLFWsetup::view_Pers(){
	view = vP;
	is_vOrtho = false;

	baseApp->rotX =  baseApp->cam00[0];
	baseApp->rotY =  baseApp->cam00[1];
	baseApp->rotZ =  baseApp->cam00[2];
	baseApp->camX =  baseApp->cam00[3];
	baseApp->camY =  baseApp->cam00[4];
	baseApp->camZ =  baseApp->cam00[5];
	baseApp->tarX =  baseApp->cam00[6];
	baseApp->tarY =  baseApp->cam00[7];
	baseApp->tarZ =  baseApp->cam00[8];
	baseApp->rolX =  baseApp->cam00[9];
	baseApp->rolY =  baseApp->cam00[10];
	baseApp->rolZ =  baseApp->cam00[11];
	baseApp->fovA =  baseApp->cam00[12];
	baseApp->zoom =  baseApp->cam00[13];
	view_Update();
}
void ntGLFWsetup::view_Save() {
	/// MODIFY TO ACCEPT INDEX FOR SPECIFIC CAMERA
	/// ADD FUNCTION TO RETRIEVE SAVED CAMERAS
	baseApp->cam00[0]  = baseApp->rotX;
	baseApp->cam00[1]  = baseApp->rotY;
	baseApp->cam00[2]  = baseApp->rotZ;
	baseApp->cam00[3]  = baseApp->camX;
	baseApp->cam00[4]  = baseApp->camY;
	baseApp->cam00[5]  = baseApp->camZ;
	baseApp->cam00[6]  = baseApp->tarX;
	baseApp->cam00[7]  = baseApp->tarY;
	baseApp->cam00[8]  = baseApp->tarZ;
	baseApp->cam00[9]  = baseApp->rolX;
	baseApp->cam00[10] = baseApp->rolY;
	baseApp->cam00[11] = baseApp->rolZ;
	baseApp->cam00[12] = baseApp->fovA;
	baseApp->cam00[13] = baseApp->zoom;

	print_Camera();
}

void ntGLFWsetup::display_HUD() {
	///////////////////////////////////////////UPPER BAR
	glColor4f(0.78, 0.95, .98, 0.2);
	glLineWidth(1);
	glBegin(GL_LINES);
	glVertex2f(5, appHeight - 25);
	glVertex2f(appWidth - 5, appHeight - 25);
	glEnd();

	///////////////////////////////////////////MIDDLE BARS
	glLineWidth(.5);
	glBegin(GL_LINES);
	glVertex2f(appWidth - 55, appHeight * 0.5);
	glVertex2f(appWidth - 5, appHeight * 0.5);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(5, appHeight * 0.5);
	glVertex2f(55, appHeight * 0.5);
	glEnd();

	///////////////////////////////////////////LOWER BARS
	glColor4f(1.0, 1.0, 1.0, 0.9);
	glLineWidth(0.5);
	glBegin(GL_LINES);
	glVertex2f(5, 155);
	glVertex2f(appWidth - 5, 155);
	glEnd();

	///////////////////////////////////////////LOWER DIVISIONS
	glColor4f(0.78, 0.95, .98, 0.2);
	for (int i = 0; i < appWidth * 0.1; i++) {
		glLineWidth(1);
		glBegin(GL_LINES);
		glVertex2f((i * 10) + 5, 140); //appHeight * 0.5
		glVertex2f((i * 10) + 5, 150); //appHeight * 0.5
		glEnd();

		
		if (i % 5 == 0 && i < 50) {
			glBegin(GL_LINES);
			glVertex2f((i * 10) + 5, 55); //appHeight * 0.5
			glVertex2f((i * 10) + 5, 132); //appHeight * 0.5
			glEnd();

			//if (i % 5 == 0 && i == 45) {
			//	// VERTICAL SIDE BAR
			//	glBegin(GL_LINES);
			//	glVertex2f((i * 10) + 5, 160); //appHeight * 0.5
			//	glVertex2f((i * 10) + 5, appHeight - 30); //appHeight * 0.5
			//	glEnd();
			//}
		}
		////SLIDER KEYS
		glLineWidth(15);
		if (i % 5 == 0 && i < 45) {
			glBegin(GL_LINES);
			glVertex2f((i * 10) + 8, 80); //appHeight * 0.5
			glVertex2f((i * 10) + 52, 80); //appHeight * 0.5
			glEnd();
		}
	}

	///////////////////////////////////////////LOWER DIVISIONS
	glColor4f(0.78, 0.95, .98, .1);
	glLineWidth(15);
	glBegin(GL_LINES);
	glVertex2f(5, 40); //appHeight * 0.5
	glVertex2f(appWidth - 5, 40); //appHeight * 0.5
	glEnd();

	glLineWidth(1);
	glBegin(GL_LINES);
	glVertex2f(appWidth - 5, 48);
	glVertex2f(5, 52);
	glEnd();

	glColor4f(0.78, 0.95, .98, .2);
	glPointSize(1);
	glBegin(GL_POINTS);
	for (int i = 0; i < appWidth * 0.1; i++) {
		glVertex2f((i * 10) + 5, 135); //appHeight * 0.5
	}
	glEnd();
}
/////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////// SET SPACE FOR DISPLAY MODE
void ntGLFWsetup::set_3D_DispView() {
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}
void ntGLFWsetup::set_2D_DispView() {
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, appWidth, 0, appHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void ntGLFWsetup::set_3D_Proj() {
	//glDisable(GL_DEPTH_TEST);
	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	//glLoadIdentity();
	//glOrtho(0, appWidth, 0, appHeight, -1.5, 1.5);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluLookAt(baseApp->camX, baseApp->camY, baseApp->camZ, baseApp->tarX, baseApp->tarY, baseApp->tarZ, baseApp->rolX, baseApp->rolY, baseApp->rolZ);
}

void ntGLFWsetup::view_Update() {
	gluPerspective(baseApp->fovA + baseApp->zoom, baseApp->width / baseApp->height, baseApp->zNear, baseApp->zFar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(baseApp->camX, baseApp->camY, baseApp->camZ, baseApp->tarX, baseApp->tarY, baseApp->tarZ, baseApp->rolX, baseApp->rolY, baseApp->rolZ);
	glRotatef(baseApp->rotX, 1.f, 0.f, 0.f);
	glRotatef(baseApp->rotY, 0.f, 1.f, 0.f);
	glRotatef(baseApp->rotZ, 0.f, 0.f, 1.f);
}