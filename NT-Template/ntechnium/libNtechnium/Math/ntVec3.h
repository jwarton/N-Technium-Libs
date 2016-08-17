///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////// ntVec3.h
// openGl scratch libs							///////////////////
// Vector 3 Class								///////////////////
// created by James Warton on 02/20/2014		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef VEC3_JPW_NTECHNIUM
#define VEC3_JPW_NTECHNIUM

#include <GLFW/glfw3.h>
#include <math.h>
#include <armadillo>

namespace jpw{
	class ntVec3;
	typedef ntVec3 Vec3;

	class ntVec3 {
	private:

	public:
		float x, y, z;

		ntVec3();
		ntVec3(float x, float y, float z);
		ntVec3(ntVec3* v0, ntVec3* v1);
		//ntVec3(const ntVec3& v);
		//ntVec3& operator=(const ntVec3& v);
		//~ntVec3();

		void add(ntVec3* v);
		void sub(ntVec3* v);
		void mult(float s);
		void div(float s);
		void set(ntVec3* v);

		//RETURNS DOT PRODUCT AS COSINE OF ANGLE BETWEEN VECTORS
		float dot(ntVec3* v);
		float angle(ntVec3* v);
		//RETURNS CROSS PRODUCT AS NEW VECTOR OBJECT PERPENDICULAR TO V AND V'
		ntVec3 cross(ntVec3* v);

		///////////////////////////////////////////////////////////////
		float mag();		////////// RETURNS MAGNITUDE AS FLOAT VALUE
		float magSqrd();	////RETURNS MAGNITUDE SQUARED;  //EFFICENCY
		void round(float t);
		void unitize();
		void invert();		/////////////// REVERSE DIRECTION OF VECTOR
		void clear();							//SET VECTOR TO (0,0,0)

		//DISTANCE TO VECTOR/POINT
		float distance(ntVec3* v);
		float distSqrd(ntVec3* v);
		///////////////////////////////////////////////////////////////
		////////////////////////////////////// EUCLIDIAN TRANSORMATIONS
		void translate(ntVec3* pos, ntVec3* dir, float dist);
		void scale(ntVec3* pos, float factor);
		void align(ntVec3* axis_T);
		void orient(ntVec3* axis_T, ntVec3* axis_S);
		void rotate(ntVec3* axis_R);

		///////////////////////////////////////////////////////////////
		////////////////////////////////////////////// DO VECTORS ALIGN
		bool compare(ntVec3* vec);

		void display();
		void print();
		void print(std::string s);

		///////////////////////////////////////////////////////////////
		//////////////////////////////////////////// OPERATOR OVERLOADS
		ntVec3 operator+(Vec3 v);
		ntVec3 operator-(Vec3 v);
		ntVec3 operator*(Vec3 v);
		ntVec3 operator/(Vec3 v);

		void operator= (Vec3 v);
		void operator+=(Vec3 v);
		void operator-=(Vec3 v);
		void operator*=(Vec3 v);
		void operator/=(Vec3 v);
		bool operator==(Vec3 v);

		///////////////////////////////////////////////////////////////
		ntVec3* operator+(Vec3* v);
		ntVec3* operator-(Vec3* v);
		ntVec3* operator*(Vec3* v);
		ntVec3* operator/(Vec3* v);

		void operator= (Vec3* v);
		bool operator==(Vec3* v);
	};

	static double distance(ntVec3* v0, ntVec3* v1) {
		double dX = v0->x - v1->x;
		double dY = v0->y - v1->y;
		double dZ = v0->z - v1->z;
		double dist = sqrt(pow(dX, 2) + pow(dY, 2) + pow(dZ, 2));
		return dist;
	}
	static double distSqrd(ntVec3* v0, ntVec3* v1) {
		double dX = v0->x - v1->x;
		double dY = v0->y - v1->y;
		double dZ = v0->z - v1->z;
		double distSqrd = (pow(dX, 2) + pow(dY, 2) + pow(dZ, 2));
		return distSqrd;
	}
}
#endif