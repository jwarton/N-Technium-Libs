#include "ntMatrix4.h"

using namespace jpw;

ntMatrix4::ntMatrix4(){
}
ntMatrix4::ntMatrix4(ntVec3 *vector):
vector(vector){
	init();
}
void ntMatrix4::init(){
	//row 1
	a1 = 1;
	b1 = 0;
	c1 = 0;
	d1 = 0;
	//row 2
	a2 = 0;
	b2 = 1;
	c2 = 0;
	d2 = 0;
	//row 3
	a3 = 0;
	b3 = 0;
	c3 = 1;
	d3 = 0;
	//row 4
	a4 = 0;
	b4 = 0;
	c4 = 0;
	d4 = 1;
	//vector
	x = vector->x;
	y = vector->y;
	z = vector->z;
	w = 1;
}

void ntMatrix4::scale3d(float factor){
	a1 = factor;
	b2 = factor;
	c3 = factor;
	run();
}
void ntMatrix4::scale2d(float factor, ntVec3 axis){
	///TODO
	run();
}
void ntMatrix4::scale1d(float factor, ntVec3 axis){
	///TODO
	run();
}

void ntMatrix4::rotate3d(float theta, ntVec3 axis){
	//	| a1 a2 a3 a4 |
	//	| b1 b2 b3 b4 |
	//	| c1 c2 c3 c4 |
	//	| d1 d2 d3 d4 |

	a1 = cos(theta) + (axis.x * axis.x * (1 - cos(theta)));
	a2 = axis.x * axis.y * (1 - cos(theta)) - (axis.z * sin(theta));
	a3 = axis.x * axis.z * (1 - cos(theta)) + (axis.y * sin(theta));

	b1 = axis.y * axis.x * (1 - cos(theta)) + (axis.z * sin(theta));
	b2 = cos(theta) + (axis.y * axis.y * (1 - cos(theta)));
	b3 = axis.y * axis.z * (1 - cos(theta)) - (axis.x * sin(theta));

	c1 = axis.z * axis.x * (1 - cos(theta)) - (axis.y * sin(theta));
	c2 = axis.z * axis.y * (1 - cos(theta)) + (axis.x * sin(theta));
	c3 = cos(theta) + (axis.z * axis.z * (1 - cos(theta)));

	run();
}
void ntMatrix4::align3d(ntVec3 vec) {

	ntVec3 v = vec.cross(vector);	//axis of rotation
	float  s = 0;					//angle sine of rotation
	float  c = v.dot(vector);		//angle cosine of rotation in radians
	
	a2 = -v.z;
	a3 =  v.y;
	b1 =  v.z;
	b3 = -v.x;
	c1 = -v.y;
	c2 =  v.x;

	run();
}

void ntMatrix4::rotateX(float theta){
	b2 = cos(theta);
	c2 = sin(theta);
	b3 = -sin(theta);
	c3 = cos(theta);
	run();
}
void ntMatrix4::rotateY(float theta){
	a1 = cos(theta);
	c1 = sin(theta);
	a3 = -sin(theta);
	c3 = cos(theta);
	run();
}
void ntMatrix4::rotateZ(float theta){
	a1 = cos(theta);
	b1 = sin(theta);
	a2 = -sin(theta);
	b2 = cos(theta);
	run();
}

void ntMatrix4::translate(ntVec3 delta){
	d1 = delta.x;
	d2 = delta.y;
	d3 = delta.z;
	run();
}

void ntMatrix4::run(){
	vector->x = (a1*x) + (b1*y) + (c1*z) + (d1*w);
	vector->y = (a2*x) + (b2*y) + (c2*z) + (d2*w);
	vector->z = (a3*x) + (b3*y) + (c3*z) + (d3*w);
	//TRANSFORMATIONS UPDATE VALUES WITHIN EXISTING MATRIX VECTOR
}

ntVec3* ntMatrix4::getVector(){
	return vector;
}

//http://en.wikipedia.org/wiki/Transformation_matrix

