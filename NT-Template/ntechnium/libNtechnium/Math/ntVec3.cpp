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
	//RETURNS COS THETA
	return ((x * v->x) + (y * v->y) + (z * v->z));
}
float ntVec3::angle(ntVec3* v) {
	//RETURNS THETA [RADIANS]
	double phi = acos(dot(v));
	return phi;
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
	return sqrt(mag2());
}
float ntVec3::mag2() {
	return x*x + y*y + z*z;
}

void ntVec3::unitize(){
	float m = mag();
	div(m);
}

void ntVec3::normalize() {
	float m = mag();
	div(abs(m));
}

void ntVec3::translate(ntVec3* pos, ntVec3* dir, float dist){
	set(dir);	//set end pt orientation from world coord
	mult(dist);	//scale by distance
	add(pos);	//translate from world coord to local
}
void ntVec3::scale(ntVec3* pos, float factor){
	sub(pos);
	x *= factor;
	y *= factor;
	z *= factor;
	add(pos);
}
void ntVec3::align(ntVec3* axis_T) {
	if (compare(axis_T) == false) {
		ntVec3* axis_A = new ntVec3(x, y, z);
		axis_A->unitize();

		int rows = 3;
		int cols = 3;
		arma::mat R = arma::eye<arma::mat>(rows, cols);

		ntVec3 v = axis_T->cross(axis_A);
		double c = axis_T->dot(axis_A);							///angle cos [rad]		//dot(A,B)
		double s = (v.mag2());									///norm of vector		//||v||
		arma::mat ssc = arma::zeros<arma::mat>(rows, cols);		///ssc = [0, -v(3), v(2); v(3), 0, -v(1); -v(2), v(1), 0];

		ssc << 0 << -v.z << v.y << arma::endr
			<< v.z << 0 << -v.x << arma::endr
			<< -v.y << v.x << 0 << arma::endr;

		if (c == -1) {
			R = -R;
		} else if (c != 1) {
			R = R + ssc + (ssc * ssc) * (1 - c) / s;	///R = eye(3) + ssc + ssc^2*(1-dot(A,B))/(mag(v))^2;
														///R = I + [v] + [v]^2((1-c)/s)
		}
		arma::mat vec = arma::zeros<arma::mat>(rows, 1);
		vec << x << arma::endr
			<< y << arma::endr
			<< z << arma::endr;
		vec = arma::solve(R, vec);				//arma::solve(A, B); | matlab- A\B or inv(A)*B

		x = vec(0, 0);
		y = vec(1, 0);
		z = vec(2, 0);

		round(.001);
	}
	//http://math.stackexchange.com/questions/180418/calculate-rotation-matrix-to-align-vector-a-to-vector-b-in-3d
}
void ntVec3::align2v(ntVec3* axis_T, ntVec3* axis_S) {

	if (axis_S->compare(axis_T) == false) {
		ntVec3 axis_A, axis_B;
		axis_A.set(axis_T);
		axis_B.set(axis_S);
		axis_A.unitize();
		axis_B.unitize();

		int rows = 3;
		int cols = 3;
		arma::mat R = arma::eye<arma::mat>(rows, cols);

		ntVec3 v = axis_A.cross(&axis_B);
		double c = axis_A.dot(&axis_B);							///angle cos [rad]		//dot(A,B)
		double s = (v.mag2());									///norm of vector		//||v||
		arma::mat ssc = arma::zeros<arma::mat>(rows, cols);		///ssc = [0, -v(3), v(2); v(3), 0, -v(1); -v(2), v(1), 0];

		ssc << 0 << -v.z << v.y << arma::endr
			<< v.z << 0 << -v.x << arma::endr
			<< -v.y << v.x << 0 << arma::endr;

		if (c == -1) {
			R = -R;
		} else if (c != 1) {
			R = R + ssc + (ssc * ssc) * (1 - c) / s;	///R = eye(3) + ssc + ssc^2*(1-dot(A,B))/(mag(v))^2;
														///R = I + [v] + [v]^2((1-c)/s)
		}
		arma::mat vec = arma::zeros<arma::mat>(rows, 1);
		vec << x << arma::endr
			<< y << arma::endr
			<< z << arma::endr;
		vec = arma::solve(R, vec);				//arma::solve(A, B); | matlab- A\B or inv(A)*B

		x = vec(0, 0);
		y = vec(1, 0);
		z = vec(2, 0);

		round(.001);
	}
	//http://math.stackexchange.com/questions/180418/calculate-rotation-matrix-to-align-vector-a-to-vector-b-in-3d
}
void ntVec3::round(float t) {
		if (x < t && x > -t) {
			x = std::round(x);
		}

		if (y < t && y > -t) {
			y = std::round(y);
		}

		if (z < t && z > -t) {
			z = std::round(z);
		}
}
bool ntVec3::compare(ntVec3* vec) {
	ntVec3* dir = new ntVec3(x,y,z);
	dir->unitize();

	bool isEq = false;
	if (vec->x == dir->x && vec->y == dir->y && vec->z == dir->z) {
		isEq = true;
	}
	return isEq;
}
void ntVec3::display(){
	glPointSize(2.f);
	glBegin(GL_POINTS);
	glColor3f(1.f, 1.f, 0.f);
	glVertex3f(x, y, z);
	glEnd();
}
void ntVec3::print() {
	std::cout << "VEC: (" << x << ", " << y << ", " << z <<  " )" << endl;
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
