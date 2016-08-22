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
	public:
		float x, y, z;

		ntVec3();								//0,0,0
		ntVec3(float x, float y, float z);		//STANDARD
		ntVec3(ntVec3* v);						//COPY CONSTRUCTORS
		ntVec3(const ntVec3& v);				//COPY CONSTRUCTORS
		ntVec3(ntVec3* pt0, ntVec3* pt1);		//CONSTRUCTS VECTOR BETWEEN POINTS
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
		void unitize();		////////////////// NORMALIZE VECTOR -1 TO 1
		void invert();		/////////////// REVERSE DIRECTION OF VECTOR
		void clear();		///////////////////// SET VECTOR TO (0,0,0)
		void round(float t);

		///////////////////////////////////////////////////////////////
		float mag();		////////// RETURNS MAGNITUDE AS FLOAT VALUE
		float magSqrd();	////RETURNS MAGNITUDE SQUARED;  //EFFICENCY
		float distance(ntVec3* v);			// DISTANCE TO VECTOR/POINT
		float distSqrd(ntVec3* v);			// DISTANCE SQUARED
		///////////////////////////////////////////////////////////////
		////////////////////////////////////// EUCLIDIAN TRANSORMATIONS
		void translate(ntVec3* pos, ntVec3* dir, float dist);
		void scale(ntVec3* pos, float factor);
		void align(ntVec3* axis_T);
		void orient(ntVec3* axis_T, ntVec3* axis_S);
		void rotate(ntVec3* axis_R, float theta);

		///////////////////////////////////////////////////////////////
		////////////////////////////////////////// ARE VECTORS PARALLEL
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
		bool operator<=(Vec3 v);
		bool operator>=(Vec3 v);
		bool operator<(Vec3 v);
		bool operator>(Vec3 v);

		///////////////////////////////////////////////////////////////
		ntVec3* operator+(Vec3* v);
		ntVec3* operator-(Vec3* v);
		ntVec3* operator*(Vec3* v);
		ntVec3* operator/(Vec3* v);

		void operator= (Vec3* v);
		void operator+=(Vec3* v);
		void operator-=(Vec3* v);
		void operator*=(Vec3* v);
		void operator/=(Vec3* v);

		bool operator==(Vec3* v);
		bool operator<=(Vec3* v);
		bool operator>=(Vec3* v);
		bool operator<(Vec3* v);
		bool operator>(Vec3* v);
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