// ntVertex.h
// openGl scratch libs
// created by James Warton on 02/20/2014

#ifndef VERTEX_JPW_NTECHNIUM
#define VERTEX_JPW_NTECHNIUM

#include <GLFW/glfw3.h>
#include <iostream>
#include "ntVec3.h"
#include "ntColor4f.h"

using namespace std;
using namespace jpw;
class ntVertex;
typedef ntVertex vertex;
typedef ntVertex point;

class ntVertex {
private:
	ntVec3 norm;
	//float U,V;
	float size;
	
public:
	ntVec3* pos;
	ntVec3  posF;
	ntColor4f col;

	ntVertex();
	ntVertex(ntVec3  pos);
	ntVertex(ntVec3* pos);
	ntVertex(ntVec3* pos,float size);
	ntVertex(ntVec3* pos,ntColor4f col);
	ntVertex(ntVec3* pos,float size,ntColor4f col);

	void setPos(ntVec3 posF);
	void setSize(float size);
	void setColor(ntColor4f col);
	ntColor4f getColor();
	void display(float size);
	void display();
};
#endif
