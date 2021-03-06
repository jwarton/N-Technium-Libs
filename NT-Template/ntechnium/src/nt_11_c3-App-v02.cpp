#include "nt_11_c3-App-v02.h"
#include <GL/glu.h>

void tokyoApp02::init() {

	std::cout << "\n\n";
	std::cout << "///////////////////////////////////////////////////////////////\n";
	std::cout << "/////////////////////////////////////////////// PROGRAM RUNNING\n\n";
	///////////////////////////////////////////////////////////////

	std::string url_IMG = path_IMG_IN + file_IMG;
	img_00 = ntImage(url_IMG + ".jpg");
	std::string url_TXT = path_TXT_IN + file_TXT;

	skin_00 = ntTriSkin(url_TXT, url_IMG, "C3");
	skin_00.setPathOut(path_Out);
	skin_00.set_Parameters(TRI, DOT, 1.5);
	skin_00.set_PerfStyle(perf_03);
	skin_00.set_Gen(1);
	//skin_00.set_ImgMosaic(path_IMG_IN + "0-255\\");
	//skin_00.set_Fastener(float min, float max);
	//skin_00.set_Parallel();

	//skin_00.save_IMG();
	//skin_00.save_TXT();
	skin_00.init();
	///
	//ntTriSkin skin_01 = ntTriSkin(url_TXT, url_IMG, "C3");
	//skin_01.setPathOut(path_Out);
	//skin_01.set_Parameters(TRI, DOT, 1.5);
	//skin_01.set_PerfStyle(perf_03);
	//skin_01.set_Gen(1);
	//skin_01.init();
	//ntTriSkin skin_02 = ntTriSkin(url_TXT, url_IMG, "C3");
	//skin_02.setPathOut(path_Out);
	//skin_02.set_Parameters(TRI, DOT, 1.5);
	//skin_02.set_PerfStyle(perf_03);
	//skin_02.set_Gen(1);
	//skin_02.init();
	//ntTriSkin skin_03 = ntTriSkin(url_TXT, url_IMG, "C3");
	//skin_03.setPathOut(path_Out);
	//skin_03.set_Parameters(TRI, DOT, 1.5);
	//skin_03.set_PerfStyle(perf_03);
	//skin_03.set_Gen(1);
	//skin_03.init();



	///////////////////////////////////////////////////////////////
	std::cout << "\n\n";
	std::cout << "////////////////////////////////////////////// PROGRAM COMPLETE\n";
	std::cout << "///////////////////////////////////////////////////////////////\n\n";
}
void tokyoApp02::run(){

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		skin_00.set_GenDisplay(0);
		skin_00.set_PerfStyle(perf_01);
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		skin_00.set_GenDisplay(1);
		skin_00.set_PerfStyle(perf_02);
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
		skin_00.set_GenDisplay(2);
		skin_00.set_PerfStyle(perf_03);
	}
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
		skin_00.set_GenDisplay(3);
		skin_00.set_PerfStyle(perf_04);
	}
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
		skin_00.set_GenDisplay(4);
		skin_00.set_PerfStyle(perf_05);
	}
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
		skin_00.set_PerfStyle(perf_06);
		skin_00.set_GenDisplay(5);
	}
	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
		skin_00.set_PerfStyle(perf_07);
	}
	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
		skin_00.set_PerfStyle(perf_08);
	}
	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
		skin_00.set_PerfStyle(perf_09);
	}
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
		skin_00.set_PerfStyle(perf_10);
	}
	
	///////////////////////////////////////////////////////////////
	/////////////////////////////////////////  WORLD MAPPED DISPLAY
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
		skin_00.display_Next();
	}
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
		skin_00.display_Prev();
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		skin_00.set_Mode(vQ);
	}

	///////////////////////////////////////////////////////////////
	/////////////////////////////////////////  IMAGE MAPPED DISPLAY
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		skin_00.set_Mode(vA);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		skin_00.set_Mode(vS);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		skin_00.set_Mode(vD);
	}

	///////////////////////////////////////////////////////////////
	///////////////////////////////////////////  WIRE FRAME DISPLAY
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		skin_00.set_Mode(vW);
	}

	///////////////////////////////////////////////////////////////
	//////////////////////////////////////  LOCAL FACE DISPLAY MODE
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		skin_00.set_PerfType(TRICELL);
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		skin_00.set_PerfType(DOT);
	}

	display();
}

void tokyoApp02::display(){
	///////////////////////////////////////////////////////////////
	skin_00.display();

	///////////////////////////////////////////////////////////////
	////////////////////////////////////////////  2D DISPLAY CONENT
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, appWidth, 0, appHeight);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	skin_00.display_2D();
	//img_00.display();
	///////////////////////////////////////////////////////////////
}
