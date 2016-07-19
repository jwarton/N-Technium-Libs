///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////// nt_11_c3-App-v01.h
// openGl scratch libs							///////////////////
// Derived Content Class: Image Mosaic		    ///////////////////
// created by James Warton on 06/13/2016		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef APPLICATION_C3_TOKYO_V01_JPW_NTECHNIUM
#define APPLICATION_C3_TOKYO_V01_JPW_NTECHNIUM

#include <iostream>
#include <iomanip>
#include "ntMath.h"
#include "ntMatrix4.h"
#include "nt_AppContent.h"
#include "nt_Utility.h"
#include "ntImage.h"
#include "ntMosaic.h"

using namespace jpw;
class tokyoApp01 : public AppContent {
private:

	string url_IMG;
	string url_OUT;

	string path_IN = nt_Utility::getPathToResources();
	string path_TXT = path_IN + "txt\\";
	string path_IMG = path_IN + "imgs\\c3\\01\\";


	string fileName;
	string fileExt = ".jpg";

	string path_Out = nt_Utility::getPathToOutput();
	string pathExtension = "c3\\01\\";
	
	ntImage img_H;		// GLOBAL IMAGE
	ntImage img_C;		// CONVOLUTION FILTERS
	ntImage img_T;		// SUBDIVIDE DATABASE OF TILES
	ntImage img_M;		// MOSAIC IMAGE

	ntMosaic mosaic;

public:
	////////////////////////////////////////////////// CONSTRUCTORS
	//////////////////////////////// DEFAULT NOT NEEDED FOR DERIVED
	///////////////////////////////////////////////////////////////

	void read_DATA();
	//////////////////////////////////////// REQUIRED CLASS METHODS
	///////////////////////////////////////////////////////////////
	void init();
	void run();
	/////////////////////////////////////////////////// CONVIENENCE
	///////////////////////////////////////////////////////////////
	void display();
};
#endif
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////