#include "nt_05_triApp.h"
//#include <GL/glu.h>

void triApp::init() {
	///////////////////////////////////////////////////////////////
	////////////////////////////////////////////// OBJECT POSITIONS
	Vec3 pt0 = Vec3(0, 0, 0);

	v0 = new ntVec3(-1, 0, 0);
	v1 = new ntVec3( 1, 0, 0);
	v2 = new ntVec3( 0, 1.5, 0);

	///////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////// OBJECTS
	face = new ntFace3(v0, v1, v2);
	cell = ntTriCell(face);

	cell.setScale(.75);
	///////////////////////////////////////////////////////////////
	/////////////////////////////////////// OBJECTS COLOR BY VERTEX
}

void triApp::run(){

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		mode = perf_01;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		mode = perf_02;
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
		mode = perf_03;
	}
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
		mode = perf_04;
	}
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
		mode = perf_05;
	}
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
		mode = perf_06;
	}
	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
		mode = perf_07;
	}
	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
		mode = perf_08;
	}
	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
		mode = perf_09;
	}
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
		mode = perf_00;
	}
	display();
}

void triApp::display(){
	///////////////////////////////////////////////////////////////
	//perf_Style mode = perf_00;		// FULL FACE
	//perf_Style mode = perf_01;		// 3 POINT STAR OPEN CELL
	//perf_Style mode = perf_02;		// 3 POINT STAR OPEN CENTER
	//perf_Style mode = perf_03;		// 3 POINT STAR 
	//perf_Style mode = perf_04;		// CENTER FACES
	//perf_Style mode = perf_05;		// OUTER TRI OUTLINE
	//perf_Style mode = perf_06;		// BISECTOR EDGES
	//perf_Style mode = perf_07;		// BISECTOR ANGLES
	//perf_Style mode = perf_08;		// INNER TRI OUTLINE
	//perf_Style mode = perf_09;		// 3 POINT STAR OUTLINE
	//perf_Style mode = perf_10;		// RIGHT HAND 3 PT

	//cell.display();
	cell.displaySubD(mode);
	cell.display_Verts();
	cell.display_Edges();
	cell.display_Centroid();
	cell.display_Normal();

}