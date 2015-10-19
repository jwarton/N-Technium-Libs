// ntTup3i.h
// openGl scratch libs
// created by James Warton on 02/20/2014

#ifndef TUP3i_JPW_NTECHNIUM
#define TUP3i_JPW_NTECHNIUM

#include <GLFW/glfw3.h>
#include <iostream>

namespace jpw{

	class ntTup3i {
	public:
		ntTup3i(int elem0, int elem1, int elem2);
		int elem0, elem1, elem2;
	};

	inline ntTup3i::ntTup3i(int elem0, int elem1, int elem2) :
		elem0(elem0), elem1(elem1), elem2(elem2){
	}
}
#endif