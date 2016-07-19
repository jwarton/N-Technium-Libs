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
#include <string>
#include <sstream>
#include <iomanip>

#include <vector>
#include <math.h>

#include <ctime>
#include <time.h>

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

static std::string format_SEC(clock_t time) {
	double sec = ((double)time) / CLOCKS_PER_SEC;
	std::stringstream ss;
	ss << std::setw(10) << std::setfill('_');
	ss << std::to_string(sec);
	std::string t = ss.str() + "  SEC";
	return t;
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

/*void ntImage::blur() {
//af::array kernel = af::array(3, 3);

//kernel(0, 0) = 1;
//kernel(0, 1) = 2;
//kernel(0, 2) = 1;
//kernel(1, 0) = 2;
//kernel(1, 1) = 4;
//kernel(1, 2) = 2;
//kernel(2, 0) = 1;
//kernel(2, 1) = 2;
//kernel(2, 2) = 1;

//float dx = 0.0625;
//kernel *= dx;

///
af::array kernel = af::array(7, 7);

kernel(0, 0) = 0.00000067;
kernel(0, 1) = 0.00002292;
kernel(0, 2) = 0.00019117;
kernel(0, 3) = 0.00038771;
kernel(0, 4) = 0.00019117;
kernel(0, 5) = 0.00002292;
kernel(0, 6) = 0.00000067;

kernel(1, 0) = 0.00002292;
kernel(1, 1) = 0.00078634;
kernel(1, 2) = 0.00655965;
kernel(1, 3) = 0.01330373;
kernel(1, 4) = 0.00655965;
kernel(1, 5) = 0.00078634;
kernel(1, 6) = 0.00002292;

kernel(2, 0) = 0.00019117;
kernel(2, 1) = 0.00655965;
kernel(2, 2) = 0.05472157;
kernel(2, 3) = 0.11098164;
kernel(2, 4) = 0.05472157;
kernel(2, 5) = 0.00655965;
kernel(2, 6) = 0.00019117;

kernel(3, 0) = 0.00038771;
kernel(3, 1) = 0.01330373;
kernel(3, 2) = 0.11098164;
kernel(3, 3) = 0.22508352;
kernel(3, 4) = 0.11098164;
kernel(3, 5) = 0.01330373;
kernel(3, 6) = 0.00038771;

kernel(4, 0) = 0.00019117;
kernel(4, 1) = 0.00655965;
kernel(4, 2) = 0.05472157;
kernel(4, 3) = 0.11098164;
kernel(4, 4) = 0.05472157;
kernel(4, 5) = 0.00655965;
kernel(4, 6) = 0.00019117;

kernel(5, 0) = 0.00002292;
kernel(5, 1) = 0.00078634;
kernel(5, 2) = 0.00655965;
kernel(5, 3) = 0.01330373;
kernel(5, 4) = 0.00655965;
kernel(5, 5) = 0.00078634;
kernel(5, 6) = 0.00002292;

kernel(6, 0) = 0.00000067;
kernel(6, 1) = 0.00002292;
kernel(6, 2) = 0.00019117;
kernel(6, 3) = 0.00038771;
kernel(6, 4) = 0.00019117;
kernel(6, 5) = 0.00002292;
kernel(6, 6) = 0.00000067;

img_LOAD = af::convolve(img_LOAD, kernel);
///
}*/

#endif

