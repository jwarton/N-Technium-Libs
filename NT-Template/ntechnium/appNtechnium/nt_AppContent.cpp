
#include "nt_AppContent.h"
//#include <GL/glu.h>

AppContent::AppContent(){}

void AppContent::init(){}
void AppContent::run(){}
void AppContent::display(){}
void AppContent::display_BBox() {

	glColor4f(1, 1, 1, 0.1);
	glBegin(GL_LINES);
	/// SIDES
	glVertex3f(BB_min->x, BB_max->y, BB_min->z);
	glVertex3f(BB_min->x, BB_max->y, BB_max->z);

	glVertex3f(BB_max->x, BB_max->y, BB_min->z);
	glVertex3f(BB_max->x, BB_max->y, BB_max->z);

	glVertex3f(BB_min->x, BB_min->y, BB_min->z);
	glVertex3f(BB_min->x, BB_min->y, BB_max->z);

	glVertex3f(BB_max->x, BB_min->y, BB_min->z);
	glVertex3f(BB_max->x, BB_min->y, BB_max->z);
	/// TOP
	glVertex3f(BB_min->x, BB_min->y, BB_max->z);
	glVertex3f(BB_min->x, BB_max->y, BB_max->z);

	glVertex3f(BB_max->x, BB_min->y, BB_max->z);
	glVertex3f(BB_max->x, BB_max->y, BB_max->z);

	glVertex3f(BB_min->x, BB_min->y, BB_max->z);
	glVertex3f(BB_max->x, BB_min->y, BB_max->z);

	glVertex3f(BB_min->x, BB_max->y, BB_max->z);
	glVertex3f(BB_max->x, BB_max->y, BB_max->z);
	/// BOTTOM
	glVertex3f(BB_min->x, BB_min->y, BB_min->z);
	glVertex3f(BB_min->x, BB_max->y, BB_min->z);

	glVertex3f(BB_max->x, BB_min->y, BB_min->z);
	glVertex3f(BB_max->x, BB_max->y, BB_min->z);

	glVertex3f(BB_min->x, BB_min->y, BB_min->z);
	glVertex3f(BB_max->x, BB_min->y, BB_min->z);

	glVertex3f(BB_min->x, BB_max->y, BB_min->z);
	glVertex3f(BB_max->x, BB_max->y, BB_min->z);
	///// CENTER
	//glVertex3f(BB_cen->x, BB_cen->y, BB_cen->z);
	//glVertex3f(BB_min->x, BB_max->y, BB_max->z);

	//glVertex3f(BB_cen->x, BB_cen->y, BB_cen->z);
	//glVertex3f(BB_max->x, BB_max->y, BB_max->z);

	//glVertex3f(BB_cen->x, BB_cen->y, BB_cen->z);
	//glVertex3f(BB_min->x, BB_min->y, BB_max->z);

	//glVertex3f(BB_cen->x, BB_cen->y, BB_cen->z);
	//glVertex3f(BB_max->x, BB_min->y, BB_max->z);
	glEnd();
}
void AppContent::set_AppDim(int x, int y) {
	this->appWidth = x;
	this->appHeight = y;
}

void AppContent::set_Grid(){
	float BS_X = BB_max->x - BB_min->x;
	float BS_Y = BB_max->y - BB_min->y;
	float BS_Z = BB_max->z - BB_min->z;

	float dim_Max = std::max(BS_X, BS_Y);
	dim_Max = std::max(dim_Max, BS_Z);

	float BS_radius = abs((dim_Max) * 0.5);

	/// TEMPORARY MODEL SPACE GRAPHIC
	float v = BS_radius * 0.1;
	Vec3* vO = new Vec3(0,0,0);
	//vO->set(BB_cen);

	Vec3* vX = new Vec3(vO->x + v, vO->y, vO->z);
	Vec3* vY = new Vec3(vO->x, vO->y + v, vO->z);
	Vec3* vZ = new Vec3(vO->x, vO->y, vO->z + v);
	
	axisX = ntEdge(vO, vX, Col4(1, 0, 0), 2);
	axisY = ntEdge(vO, vY, Col4(0.5, 0.5, 0.5, 0.5), 2);
	axisZ = ntEdge(vO, vZ, Col4(0.5, 0.5, 0.5, 0.5), 1);
	///
}
void AppContent::display_Grid() {
	glBegin(GL_LINES);
	axisX.display();
	axisY.display();
	glLineStipple(1, 0x0101);
	axisZ.display();
	glDisable(GL_LINE_STIPPLE);
}

void AppContent::view_Model() {
	///////////////////////////////////////////////////////////////
	////////////////////////////////////////////  3D DISPLAY CONENT
}
void AppContent::view_Orth() {
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	///#include <GL/glu.h> causes confict with GLFWsetup????
	//gluOrtho2D(0, appWidth, 0, appHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void AppContent::set_Bounds() {
	ntVertex v;
	v.getBounds(BB_min, BB_max);
	BB_min->print();
	BB_max->print();
}
void AppContent::set_Centroid() {

	float x = ((BB_max->x - BB_min->x) * 0.5) + BB_min->x;
	float y = ((BB_max->y - BB_min->y) * 0.5) + BB_min->y;
	float z = ((BB_max->z - BB_min->z) * 0.5) + BB_min->z;

	BB_cen->x = x;
	BB_cen->y = y;
	BB_cen->z = z;
}
void AppContent::get_Bounds(ntVec3* min, ntVec3* max){
	min->set(BB_min);
	max->set(BB_max);
}
void AppContent::get_Centroid(ntVec3* cen) {
	cen->set(BB_cen);
}