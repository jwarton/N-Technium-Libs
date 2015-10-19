#include "nt_AppContent.h"

AppContent::AppContent(){}

void AppContent::init(){}
void AppContent::run(){}
void AppContent::display(){}

void AppContent::grid(){
	/// TEMPORARY MODEL SPACE GRAPHIC
	float v = .1;
	Vec3* vO = new Vec3(0, 0, 0);
	Vec3* vX = new Vec3(v, 0, 0);
	Vec3* vY = new Vec3(0, v, 0);
	Vec3* vZ = new Vec3(0, 0, v);
	
	axisX = ntEdge(vO, vX, Col4(1, 0, 0), 2);
	axisY = ntEdge(vO, vY, Col4(0.5, 0.5, 0.5, 0.5), 2);
	axisZ = ntEdge(vO, vZ, Col4(0.5, 0.5, 0.5, 0.5), 1);
	///
}
void AppContent::grid_display(){
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS){
		axisX.display();
		axisY.display();
		glLineStipple(1, 0x0101);
		axisZ.display();
		glDisable(GL_LINE_STIPPLE);
	}
}
