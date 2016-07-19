///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// ntMosaic.h
// openGl scratch libs							///////////////////
// Image Manager Class						    ///////////////////
// created by James Warton on 06/08/2016		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef IMAGE_MOSAIC_GENERATOR_JPW_NTECHNIUM
#define IMAGE_MOSAIC_GENERATOR_JPW_NTECHNIUM
#define _USE_MATH_DEFINES

#include <iostream>
#include <iomanip>
#include <string>
#include <math.h>
#include <time.h>
#include <map>

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#include <armadillo>
#include <arrayfire.h>
#include <cstdio>
#include <cstdlib>

/// NT-libs
#include "ntVec3.h"
#include "ntColor4f.h"
#include "ntMath.h"
#include "ntImage.h"

using namespace jpw;

//class ntImage;///////////////////////////////// FORWARD DECLARE

class ntMosaic : ntImage {
	//friend class ntImage;
private:

	std::string url_IMG;
	std::string path_Tiles;

	af::array matrix;

	////IMAGE PARAMETERS
	int dim_X;
	int dim_Y;

	////FUNCTIONS
	void init();


	//PERFORMANCE DATA
	//struct tm timeData;
	//time_t timeStamp;

	clock_t t_SAVE = 0;
	clock_t t_LOAD = 0;
	clock_t t_TILE = 0;

	void print_T(clock_t time,std::string message);

public:
	ntMosaic();
	ntMosaic(std::string url_IMG, std::string path_Tiles);

	void tile(int cols, int rows);
	void fun_00(int cols, int rows, int gen);
	af::array  build_01(int cols, int rows, af::array mat_00, int gen);
	af::array  build_02(float seed, af::array mat_00, int gen);

	af::array get_Matrix();

	void save(std::string url);
	void run();

};

inline bool isFile(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

#endif