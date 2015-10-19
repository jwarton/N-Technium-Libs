#include "ntVec3.h"
#include <iostream>
using namespace std;
using namespace jpw;

ntVec3::ntVec3(){}

ntVec3::ntVec3(float x, float y, float z){
	this->x = x;
	this->y = y;
	this->z = z;
}

///TODO -COPY CONSTRUCTOR CAUSES OVERWRITE OR DELETION IN SPHERE
//ntVec3::ntVec3(const ntVec3& v){
//	this->x = v.x;
//	this->x = v.y;
//	this->x = v.z;
//}
//
//ntVec3& ntVec3::operator=(const ntVec3& v){
//	if (this != &v){
//		this->x = v.x;
//		this->x = v.y;
//		this->x = v.z;
//	}
//	return *this;
//}
//
//ntVec3::~ntVec3(){
//
//}

void ntVec3::add(ntVec3* v){
	x += v->x;
	y += v->y;
	z += v->z;
}

void ntVec3::sub(ntVec3* v){
	x -= v->x;
	y -= v->y;
	z -= v->z;
}

void ntVec3::mult(float s){
	x *= s;
	y *= s;
	z *= s;
}

void ntVec3::div(float s){
	x /= s;
	y /= s;
	z /= s;
}
	
void ntVec3::set(ntVec3* v){
	x = v->x;
	y = v->y;
	z = v->z;
}

//returns dot product as new vector object
//represented as angle between vectors in 3d space
float ntVec3::dot(ntVec3* v){
	//RETURNS RADIANS
	return ((x * v->x) + (y * v->y) + (z * v->z));
}

//returns cross product as vector perpendicular
ntVec3 ntVec3::cross(ntVec3* v){
	
	float xx=  ((y * v->z) - (z * v->y));
	float yy= -((x * v->z) - (z * v->x));
	float zz=  ((x * v->y) - (y * v->x));

	return ntVec3(xx, yy, zz);
}
//set vector to (0,0,0)
void ntVec3::clear(){
	x = 0;
	y = 0;
	z = 0;
}
//reverse direction of vector
void ntVec3::invert(){
	mult(-1);
}
//returns magnitude as float value
float ntVec3::mag(){
	return sqrt(x*x + y*y + z*z);
}

void ntVec3::unitize(){
	float m = mag();
	div(m);
}

void ntVec3::translate(ntVec3* pos, ntVec3* dir, float dist){
	set(dir);	//set end pt orientation from world coord
	mult(dist);	//scale by distance
	add(pos);	//translate to from world coord to local
}
void ntVec3::scale(ntVec3* pos, float factor){
	///TODO
}

void ntVec3::display(){
	glPointSize(2.f);
	glBegin(GL_POINTS);
	glColor3f(1.f, 1.f, 0.f);
	glVertex3f(x, y, z);
	glEnd();
}
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
float ntVec3::distance(ntVec3* v){
	float dX = this->x - v->x;
	float dY = this->y - v->y;
	float dZ = this->z - v->z;
	float dist = sqrt(pow(dX,2)+pow(dY,2)+pow(dZ,2));
	return dist;
}
float ntVec3::distSqrd(ntVec3* v){
	float dX = this->x - v->x;
	float dY = this->y - v->y;
	float dZ = this->z - v->z;
	float distSqrd = (pow(dX, 2) + pow(dY, 2) + pow(dZ, 2));
	return distSqrd;
}
