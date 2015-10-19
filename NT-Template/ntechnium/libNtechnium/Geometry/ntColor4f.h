///////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////// ntColor4f.h
// openGl scratch libs							///////////////////
// RGB Alpah Color Class						///////////////////
// created by James Warton on 02/20/2014		///////////////////
///////////////////////////////////////////////////////////////////
#ifndef COLOR_JPW_NTECHNIUM
#define COLOR_JPW_NTECHNIUM

#include <GLFW/glfw3.h>
#include <iostream>

namespace jpw {
	class ntColor4f;
	typedef ntColor4f ntCol4;
	typedef ntColor4f Col4f;
	typedef ntColor4f Col4;
	

	class ntColor4f{
	public:
		float r, g, b, a;
		ntColor4f();
		ntColor4f(float r, float g, float b, float a);
		ntColor4f(float r, float g, float b);
		void invert(ntColor4f* col);
	};

	inline ntColor4f::ntColor4f(){
	}

	inline ntColor4f::ntColor4f(float r, float g, float b, float a) :
		r(r), g(g), b(b), a(a){
	}

	inline ntColor4f::ntColor4f(float r, float g, float b) :
		r(r), g(g), b(b){
		a = 1;
	}

	inline void ntColor4f::invert(ntColor4f* col){
		col->r = 1 - col->r;
		col->g = 1 - col->g;
		col->b = 1 - col->b;
	}
}
#endif

