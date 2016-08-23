#ifndef NORMAL_JPW_NTECHNIUM
#define NORMAL_JPW_NTECHNIUM

#include <iostream>
#include "ntVertex.h"

using namespace jpw;

class ntNormal;
typedef ntNormal ntLine;

class ntNormal{
private:

public:
	//parameters
	ntVec3 pos, end, vec;
	float mag;

	//display properties
	ntColor4f colS;	//color start
	ntColor4f colE;	//color end
	float w;		//line weight

	ntNormal();
	ntNormal(ntVec3 pos, ntVec3 vec);
	ntNormal(ntVec3 pos, ntVec3 vec, float mag);

	void init();
	void setLength(float length);
	void set_color(ntColor4f col);
	void display(float size);
	void display();
};
#endif
