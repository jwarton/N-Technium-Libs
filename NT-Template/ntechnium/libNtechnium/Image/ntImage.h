///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////// ntImage.h
// openGl scratch libs							///////////////////
// Image Manager Class						    ///////////////////
// created by James Warton on 06/08/2016		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef IMAGE_CONTENT_JPW_NTECHNIUM
#define IMAGE_CONTENT_JPW_NTECHNIUM
#define _USE_MATH_DEFINES

#include <iostream>
#include <map>
#include "math.h"

#include <armadillo>
#include <arrayfire.h>
#include <cstdio>
#include <cstdlib>

/// NT-libs
#include "ntVec3.h"
#include "ntColor4f.h"
#include "ntMath.h"

using namespace jpw;
class ntImage {
private:

	std::string url_IN;
	af::array img_IN;
	
	std::string fileName;
	std::string prefix;
	std::string path_OUT;

	af::array img_2D;
	arma::fmat img_2d;

	arma::fmat img_00;

	int img_X = 0;
	int img_Y = 0;

	////IMAGE PARAMETERS
	ntVec3*		pos = new ntVec3(0, 0, 0);
	int			dpi =	72;
	float		alpha = 1;
	ntColor4f	col =	Col4(1, 1, 1, 1);

	int img_W;
	int img_H;

	bool isImgLoaded;
	bool isImgLocal = false;
	float img_T = 1.0;

	////FUNCTIONS
	void init();
	void save(std::string path, std::string fileName, af::array  matrix);
	void save(std::string path, std::string fileName, arma::fmat matrix);

	void set_Host();
	void set_Local();
	//arma::mat get_Mat(arma::mat matrix);
	//void set_Mat(arma::mat matrix);

	//af::array blur(const af::array& img, const af::array mask = af::gaussianKernel(3, 3));

public:
	ntImage();
	ntImage(int width, int height, float col = 255);
	ntImage(std::string url);
	ntImage(std::string url, std::string prefix);
	ntImage(ntVec3* pos, std::string url);
	//ntImage(arma::mat image);

	void load_IMG(std::string url);
	void save(std::string url);
	void save(std::string path, std::string fileName);

	void set_Pos(ntVec3* pos);
	void set_Pos(int x, int y);
	void set_Dim(int width, int hieght = 1, bool isUniform = true);
	void set_Col(float col);

	arma::fmat* getMatrix();

	void scale_2D(float scFx);

	//filters
	void reset();
	void sort(int dim, bool isAscending = false);
	void blur(int iterations = 9);
	void add();
	void sub();
	void invert();
	void contrast();
	void crop(int x_0, int x_1, int y_0, int y_1);
	void zoom(int x, int y, int w, int h);
	void edges();
	
	//stats
	int dims(int dim);
	double min_val();
	double max_val();
	double mean();

	void tile(int cols, int rows, bool doSave = false);
	void tile(int cols, int rows, std::string path);
	void tile_CPU(int cols, int rows);
	void tile_GPU(int cols, int rows);

	int getPixel(double u, double v);
	void writePixels(std::vector<int> values, std::string url);

	void run();
	void display();
};
#endif