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
	hWin32 = glfwGetWin32Window(window);

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



	////////////////////////////////////////////////////////////////////// 3D CONNEXION TEST AND INITIALIZATION
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	SiOpenData		oData;
	SiInitialize();
	SiOpenWinInit(&oData, hWin32);
	SiSetUiMode(devHdl, SI_UI_ALL_CONTROLS);								///// CONFIG SOFTBUTTON WIN DISPLAY
	devHdl = SiOpen("NT_FRAMEWORK", SI_ANY_DEVICE, SI_NO_MASK, SI_EVENT, &oData);
	
	//SiGrabDevice(devHdl, SPW_TRUE);
	SiDeviceName devName;
	SiGetDeviceName(devHdl, &devName);
	//std:cout << "\n" << "////////////////////////////////////////////////////" << endl;
	//sprintf_s(devicename, _T("%S"), devName.name);
	//std::cout << "DEVICE:  " << devicename <<endl;

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
	while (!glfwWindowShouldClose(window)) {
		/////////////////////////////////////////////////////////|ACTIVATE DERIVED USER CLASS IMPLEMENTATION|//////
		baseApp->setFrameCount(frameCount);////////////////////////////////////////////////////////////////////////
		baseApp->run();////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		EventHandler_3DX();
		EventHandler_MOUSE();
		EventHandler_KEYBD();
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

HWND ntGLFWsetup::getWindow() {
	return hWin32;
}
///////////////////////////////////////////////////footnote *01
//reference: http://bankslab.berkeley.edu/members/chris/AntiAliasing/AntiAliasingInOpenGL.html


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
		std::cout << "3DX EVENT RECEIVED" << endl;
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
		std::cout << "3DX BAD DEVICE HANDLE" << endl;
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
	///KEYBOARD INPUT FUNCTIONALITY:
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS & glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		reset();
		update();
	}

	//CAMERA JOG FUNCTIONALITY
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS & glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		baseApp->camZ += 0.1;
		update();
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS & glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		baseApp->camZ -= 0.1;
		update();
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS & glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		baseApp->camX += 0.1;
		update();
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS & glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		baseApp->camX -= 0.1;
		update();
	}
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS & glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		baseApp->camY += 0.1;
		update();
	}
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS & glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		baseApp->camY -= 0.1;
		update();
	}
	//ORBIT FUNCTIONALITY
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		baseApp->rotZ += 0.2;
		update();
		//glRotatef(.2, 0.f, 0.f, 1.f);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		baseApp->rotZ -= 0.2;
		update();
		//glRotatef(-.2, 0.f, 0.f, 1.f);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		baseApp->rotX += 0.2;
		update();
		//glRotatef(.2, 1.f, 0.f, 0.f);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		baseApp->rotX -= 0.2;
		update();
		//glRotatef(-.2, 1.f, 0.f, 0.f);
	}
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
		//baseApp->rotY += 0.2;
		update();
		//glRotatef(.2, 0.f, 1.f, 0.f);
	}
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
		//baseApp->rotY -= 0.2;
		update();
		//glRotatef(-.2, 0.f, 1.f, 0.f);
	}

	//ZOOM FUNCTIONS
	if (glfwGetKey(window, baseApp->view_ZI) == GLFW_PRESS) {
		baseApp->zoom += 0.1;
		update();
	}
	if (glfwGetKey(window, baseApp->view_ZO) == GLFW_PRESS) {
		baseApp->zoom -= 0.1;
		update();
	}
	//STANDARD VIEW TOGGLE
	if (glfwGetKey(window, baseApp->view_P) == GLFW_PRESS) {
		reset();
		update();
	}
	if (glfwGetKey(window, baseApp->view_T) == GLFW_PRESS) {
		reset();
		baseApp->camX = 0.0f;
		baseApp->camY = 0.0f;
		baseApp->camZ = 3.0f;
		baseApp->rolX = 0.0f;
		baseApp->rolY = 1.0f;
		baseApp->rolZ = 0.0f;
		update();
	}
	if (glfwGetKey(window, baseApp->view_B) == GLFW_PRESS) {
		reset();
		baseApp->camX = 0.0f;
		baseApp->camY = 0.0f;
		baseApp->camZ = -3.0f;
		baseApp->rolX = 0.0f;
		baseApp->rolY = 1.0f;
		baseApp->rolZ = 0.0f;
		update();
	}
	if (glfwGetKey(window, baseApp->view_L) == GLFW_PRESS) {
		reset();
		baseApp->camX = 3.0f;
		baseApp->camY = 0.0f;
		baseApp->camZ = 0.0f;
		baseApp->rolX = 0.0f;
		baseApp->rolY = 0.0f;
		baseApp->rolZ = 1.0f;
		update();
	}
	if (glfwGetKey(window, baseApp->view_R) == GLFW_PRESS) {
		reset();
		baseApp->camX = 0.0f;
		baseApp->camY = 3.0f;
		baseApp->camZ = 0.0f;
		baseApp->rolX = 0.0f;
		baseApp->rolY = 0.0f;
		baseApp->rolZ = 1.0f;
		update();
	}
	if (glfwGetKey(window, baseApp->view_F) == GLFW_PRESS) {
		reset();
		baseApp->camX = 0.0f;
		baseApp->camY = -3.0f;
		baseApp->camZ = 0.0f;
		baseApp->rolX = 0.0f;
		baseApp->rolY = 0.0f;
		baseApp->rolZ = 1.0f;
		update();
	}

}
void ntGLFWsetup::EventHandler_MOUSE(){

	///MOUSE EVENT FUNCTIONALITY:
	int state_m1 = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	int state_m2 = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	int state_m3 = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
	int state_m4 = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_4);
	int state_m5 = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_5);
	int state_m6 = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_6);
	int state_m7 = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_7);
	int state_m8 = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LAST);

	///	GLFW SCROLL FUNCTIONALITY
	glfwSetScrollCallback(window, scroll_callback);

	// mouse press events
	glfwSetMouseButtonCallback(window, mouseBtn_callback);
	///

	if (state_m1 == GLFW_PRESS) {
		//std::cout << "LEFT MOUSE BUTTON_1" << endl;
		//baseApp->camZ -= 0.1;
		//update();
	}
	if (state_m2 == GLFW_PRESS) {
		//std::cout << "RIGHT MOUSE BUTTON_2" << endl;
		//baseApp->camZ -= 0.1;
		//update();
	}
	if (state_m3 == GLFW_PRESS) {
		//std::cout << "MIDDLE MOUSE BUTTON_3" << endl;
		//baseApp->camZ -= 0.1;
		//update();
	}
	if (state_m4 == GLFW_PRESS) {
		//std::cout << "BACK MOUSE BUTTON_4" << endl;
		update();
	}
	if (state_m5 == GLFW_PRESS) {
		//std::cout << "FORWARD MOUSE BUTTON_5" << endl;
		update();
	}
	if (state_m6 == GLFW_PRESS) {
		//std::cout << "MOUSE BUTTON_6" << endl;
		update();
	}
	if (state_m7 == GLFW_PRESS) {
		//std::cout << "MOUSE BUTTON_7" << endl;
		update();
	}
	if (state_m8 == GLFW_PRESS) {
		//std::cout << "MOUSE BUTTON_8" << endl;
		update();
	}
}

void ntGLFWsetup::Event_3DX_gimbalM(SiSpwEvent *pEvent){
	int tx = pEvent->u.spwData.mData[SI_TX];
	int ty = pEvent->u.spwData.mData[SI_TY];
	int tz = pEvent->u.spwData.mData[SI_TZ];
	int rx = pEvent->u.spwData.mData[SI_RX];
	int ry = pEvent->u.spwData.mData[SI_RY];
	int rz = pEvent->u.spwData.mData[SI_RZ];

	baseApp->camX += tx * .0001;
	baseApp->camY += ty * .0001;
	baseApp->camZ += tz * .0001;
	baseApp->rotX += rx * .001;
	baseApp->rotY += ry * .001;
	baseApp->rotZ += rz * .001;
	update();

	ReleaseDC(hWin32, hdc);
}
void ntGLFWsetup::Event_3DX_gimbalZ(){
	//std::cout << "3DX ZERO EVENT" << endl;
	ReleaseDC(hWin32, hdc);
}
void ntGLFWsetup::Event_3DX_buttonR(int num){
	std::cout << "3DX BUTTON RELEASES" << endl;
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