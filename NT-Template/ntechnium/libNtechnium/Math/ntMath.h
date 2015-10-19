///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////// ntAlgor.h
// openGl scratch libs							///////////////////
// Algorithm Class for convenience functions	///////////////////
// created by James Warton on 04/10/2014		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef MATH_JPW_NTECHNIUM
#define MATH_JPW_NTECHNIUM

#include <iostream>
#include <vector>
//#include <cmath>

static float mapRange(float minA, float maxA, float minB, float maxB, float valB, bool inc = true){
	float rangeA = maxA - minA;
	float rangeB = maxB - minB;
	float perc = valB / rangeB * rangeA;
	//std::abs(valB / rangeB * rangeA);
	if (inc == true){
		return (minA + perc);
	}
	else {
		return (rangeA - perc) + minA;
	}
}
#endif

