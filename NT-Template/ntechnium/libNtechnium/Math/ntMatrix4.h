// ntMatrix4.h
// openGl scratch libs
// created by James Warton on 02/23/2014

#ifndef MATRIX4_JPW_NTECHNIUM
#define MATRIX4_JPW_NTECHNIUM

#include <iostream>
#include "ntVec3.h"

using namespace jpw;

class ntMatrix4;
typedef ntMatrix4 Mat4;

class ntMatrix4{
private:
	float a1, b1, c1, d1;
	float a2, b2, c2, d2;
	float a3, b3, c3, d3;
	float a4, b4, c4, d4;

	float x, y, z, w;

	ntVec3* vector;

	void init();
	void run();

public:
	ntMatrix4();
	ntMatrix4(ntVec3 *vector);

	void scale3d(float factor);
	void scale2d(float factor, ntVec3 axis);
	void scale1d(float factor, ntVec3 axis);

	void rotateX(float theta); //theta in radians
	void rotateY(float theta); //theta in radians
	void rotateZ(float theta); //theta in radians

	//rotate existing vector about  axis of rotation
	//using right hand rule
	void rotate3d(float theta, ntVec3 axis); //theta in radians
	void align3d(ntVec3 vec);					//align to input vector [NOT FUNCTIONAL]
	void translate(ntVec3 delta);

	//void transpose();
	//void inverse();
	//void mult(ntMatrix4 matrix);

	ntVec3* getVector();
};
#endif
