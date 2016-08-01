///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////// ntVec3.h
// openGl scratch libs							///////////////////
// Vector 3 Class								///////////////////
// created by James Warton on 02/20/2014		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef VEC3_JPW_NTECHNIUM
#define VEC3_JPW_NTECHNIUM

#include <GLFW/glfw3.h>

namespace jpw{

	class ntVec3;
	typedef ntVec3 Vec3;

	class ntVec3 {
	private:

	public:
		float x, y, z;

		ntVec3();
		ntVec3(float x, float y, float z);
		//ntVec3(const ntVec3& v);
		//ntVec3& operator=(const ntVec3& v);
		//~ntVec3();

		void add(ntVec3* v);
		void sub(ntVec3* v);
		void mult(float s);
		void div(float s);
		void set(ntVec3* v);
		
		//returns dot product as cosine of angle between vectors
		float dot(ntVec3* v);
		float angle(ntVec3* v);
		//returns cross product as new vector object perpendicular to v and v'
		ntVec3 cross(ntVec3* v);
		//set vector to (0,0,0)
		void clear();
		//reverse direction of vector
		void invert();
		//returns magnitude as float value
		float mag();
		//distance to argument
		float distance(ntVec3* v);
		float distSqrd(ntVec3* v);
		//transormations
		void translate(ntVec3* pos, ntVec3* dir, float dist);
		void scale(ntVec3* pos, float factor);

		void unitize();
		void normalize();
		void display();
		void print();
	};
}
#endif