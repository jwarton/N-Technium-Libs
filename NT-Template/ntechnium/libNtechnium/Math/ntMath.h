///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////// ntMath.h
// openGl scratch libs							///////////////////
// Algorithm Class for convenience functions	///////////////////
// created by James Warton on 04/10/2014		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef MATH_JPW_NTECHNIUM
#define MATH_JPW_NTECHNIUM
#define _USE_MATH_DEFINES

#include <iostream>
#include <vector>
#include <math.h>

enum D_mode { vQ, vF, vP, vS, vW, vA, vD };
enum L_mode { vX, vC, vV };

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

static float toRadians(double theta) {
	theta = theta / 180 * M_PI;
	return theta;
}

static float toDegrees(double theta) {
	theta = theta / M_PI * 180;
	return theta;
}

//static bool pt_isInside(ntVec3 point, std::vector <ntVec3*> polyVecs){
//
//	int i, j, nvert = polyVecs.size();
//	bool c = false;
//
//	for (i = 0, j = nvert - 1; i < nvert; j = i++) {
//		if (((polyVecs[i]->y >= point.y) != (polyVecs[j]->y >= point.y)) &&
//			(point.x <= (polyVecs[j]->x - polyVecs[i]->x) * (point.y - polyVecs[i]->y) / (polyVecs[j]->y - polyVecs[i]->y) + polyVecs[i]->x)
//			)
//			c = !c;
//	}
//	return c;
//}
#endif

