#include "nt_simpleApp.h"

using namespace jpw;
ntSimpleApp::ntSimpleApp(){
	this->width = 900;
	this->height = 480;

	init();
	run();
}

ntSimpleApp::ntSimpleApp(int width,int height):
width(width), height(height)
{
	init();
	run();
}

ntSimpleApp::ntSimpleApp(int width, int height, std::string title) :
width(width), height(height), title(title)
{
	init();
	run();
}

static void error_callback(int error, const char* description)
	{
		fputs(description, stderr);
	}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
	}

void ntSimpleApp::init(){
	
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	window = glfwCreateWindow(this->width, this->height, this->title.c_str(), NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	//glRotatef(0.f, 0.f, 90.f, 0.f);

	///////////////////////////////////////////////////////////////
	////////////////////////////////////////////// OBJECT POSITIONS
	Vec3 pt0 = Vec3(-1.25, 0, 0);
	Vec3 pt1 = Vec3(-.375, 0, 0);
	Vec3 pt2 = Vec3(.375, 0, 0);
	Vec3 pt3 = Vec3(0, 0, 0); //Vec3 pt3 = Vec3(1.25, 0, 0);

	Mat4(&pt2).translate(Vec3(0, 0, .5));
	Mat4(&pt1).translate(Vec3(0, 0, -.5));

	///////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////// OBJECTS
	ntVertex vertex0 = ntVertex(&pt1,2,Col4f(1,0,0,1));
	ntVertex vertex1 = ntVertex(&pt2,2,Col4f(1,0,0,1));
	ntVertex vertex2 = ntVertex(&pt0,2,Col4f(1,1,0,1));

	//ntEdge edge = ntEdge(&Vec3(5,0,0),&Vec3(0,0,0));
	//edge.w = 5;
	//edge.vert0->col = Col4f(1,1,1,1);
	//edge.vert1->col = Col4f(1,0,0,1);

	//ntFace3 face = ntFace3(*vertex0,*vertex1,*vertex2);
	//ntFace3 face = ntFace3(pt0,pt1,pt2,ntColor4f(0,0,0,1));
	//ntFace3 face = ntFace3(pt0,pt1,pt2);
	//ntTetra tetra = ntTetra(pt0,ntVec3(0,0,0),ntVec3(.2,.5,.5),ntColor4f(1,1,1,1),ntColor4f(1,1,1,1),"name");

	sphereA = ntSphere(pt1, Vec3(0, 0, 0), Vec3(.25, .5, .5), 50, 50);
	sphereB = ntSphere(pt2, Vec3(0, 0, 0), Vec3(.25, .5, .5), 50, 50);
	sphereC = ntSphere(pt2, Vec3(0, 0, 0), Vec3(.25, .5, .5), 10, 10);
	sphereD = ntSphere(pt3, Vec3(0, 0, 0), Vec3(.25, .5, .5), 25, 25);

	///////////////////////////////////////////////////////////////
	/////////////////////////////////////// OBJECTS COLOR BY VERTEX
	int val = sphereA.verts.size();
	for (int i = 0; i<val; i++){
		float r = 1 / (val / ((float)i + 1));
		float g = 0;
		float b = .5 - (.25 / (val / ((float)i + 1)));
		float a = 1 / (val / ((float)i + 1));
		sphereA.verts.at(i)->set_color(Col4f(r, g, b, a));
	}
	val = sphereC.verts.size();
	for (int i = 0; i<val; i++){
		float r = 1 / (val / ((float)i + 1));
		float g = 0;
		float b = .5 - (.25 / (val / ((float)i + 1)));
		float a = 1 / (val / ((float)i + 1));
		sphereC.verts.at(i)->set_color(Col4f(r, g, b, a));
	}
	val = sphereD.verts.size();
	for (int i = 0; i<val; i++){
		float r = 1 / (val / ((float)i + 1));
		float g = .5 + (.5 / (val / ((float)i + 1)));
		float b = 1 - (1 / (val / ((float)i + 1)));
		float a = 1 / (val / ((float)i + 1));
		sphereD.verts.at(i)->set_color(Col4f(r, g, b, a));
	}
}
 
void ntSimpleApp::run(){
	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glRotatef(270.f, 1.f, 0.f, 0.f);
		//glRotatef(90.f, 0.f, 0.f, 1.f);
		glRotatef((float)glfwGetTime() * 20.f, 1.f, 0.f, 1.f);  //ROTATION ABOUT Z
		
		sphereA.display_verts(2);
		//sphereB.display_norms(1);
		//sphereC.display();
		sphereC.display_edges();
		sphereD.display();
		sphereD.display_norms(1);
		//tetra.display();
		//tetra.display_norms(0);
		//edge.display();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}