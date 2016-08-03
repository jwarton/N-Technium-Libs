#include "nt_11_c3-App-v01.h"
#include <GL/glu.h>

void tokyoApp01::init() {

	bool doHostImage		= false;
	bool doFilter			= false;
	bool doSubTiles			= false;
	bool doMosaic			= false;
	bool doWritePixels		= false;
	bool doWriteAvgTiles	= false;
	///////////////////////////////////////////////////////////////
	//////// GRID IMAGE AND COMPUTE MATRIX MANIPULATION PERFORMANCE
	path_Out = path_Out + pathExtension;

	///////////////////////////////////////////////////////////////
	///////////////////////////////////////////// LOAD SOURCE IMAGE
	if(doHostImage == true){

		/// READ SOURCE IMAGE PATH
		fileName = "profile_001";
		url_IMG = path_IMG + "profile\\" + fileName + fileExt;
		img_H = ntImage(url_IMG);

		/// WRITE IMAGE PATH
		url_OUT = path_Out + fileName + "_TEST" + fileExt;
 
		//img_H.tile(20, 10, path_Out);
		//img_H.tile_CPU(20,  10);
		//img_H.tile_CPU(50,  25);
		//img_H.tile_CPU(100, 50);
		img_H.tile_CPU(200, 100);
		//img_H.tile_GPU(50,  25);
		//img_H.tile_GPU(100, 50);
		//img_H.tile_GPU(200, 100);
		img_H.save(url_OUT);
	}
	///////////////////////////////////////////////////////////////
	/////////////////////////////// SAVE CONVULUTIONS FILTERS- BLUR
	if (doFilter == true) {
		fileName = "0061_a";
		string url = path_IMG + "swatch\\" + fileName + fileExt;
		img_C = ntImage(url);

		img_C.reset();
		img_C.blur(35);
		img_C.save(path_Out, fileName + "_blur.jpg");

		img_C.reset();
		img_C.edges();
		img_C.save(path_Out, fileName + "_edges.jpg");

		img_C.reset();
		img_C.invert();
		img_C.save(path_Out, fileName + "_invert.jpg" );

		img_C.reset();
		img_C.sort(0);
		img_C.save(path_Out, fileName + "_sort.jpg");

		img_C.add();
		img_C.save(path_Out, fileName + "_add.jpg");

		img_C.sub();
		img_C.save(path_Out, fileName + "_sub.jpg");
	}
	
	///////////////////////////////////////////////////////////////
	///////////////////////// TILE TEXTURE IMAGES FOR MOSAIC SOURCE
	if (doSubTiles == true) {
		for (int i = 0; i < 70; i++) {
			//LOAD SOURCE IMAGES
			stringstream ss;
			ss << std::setw(4) << std::setfill('0');
			ss << i;
			string prefix = ss.str();				// 0000 - 0069
			fileName = prefix + "_b";
			string url_in = path_IMG + "swatch\\" + fileName + fileExt;

			img_T = ntImage(url_in, prefix);		// REFACTOR PREFIX INITIALIZATION
			img_T.set_Dim(999, 666);				// RESIZE IMAGE 

			//SUBDIVIDE AND SAVE IMAGE
			img_T.tile(3, 2, path_Out);
		}
	}

	// INITIALIZE MOSAIC GENERATOR
	if (doMosaic == true) {
		std::string path_Tiles = path_Out + "tiles_current\\"; ///REVISE PATH TO RESOURCES
		string url_out = path_Out + "mosaic_000.jpg";
		
		mosaic = ntMosaic(url_IMG, path_Tiles);
		mosaic.save(url_out);

		std::cout << "\n	////	PROGRAM EXECUTION COMPLETE\n" << endl;
	}

	///////////////////////////////////////////////////////////////
	////////////////////// MAP PIXEL VALUES TO UV SURFACE PARAMETER
	if (doWritePixels == true) {

		/// READ SOURCE IMAGE PATH
		fileName = "profile_002";
		url_IMG = path_IMG + "profile\\" + fileName + fileExt;
		img_H = ntImage(url_IMG);

		/// WRITE IMAGE PATH
		string url_01 = path_Out + "pixels_001" + ".txt";
		std::vector<int> vals;
		int cnt = 1400;

		for (int i = 0; i < cnt; i++) {

			double u = (rand() % 1000) * 0.0001;
			double v = (rand() % 1000) * 0.0001;

			int pixel = img_H.getPixel(u, v);
			vals.push_back(pixel);
		}
		/// WRITE VALUES TO TEXT FILE
		img_H.writePixels(vals, url_01);
	}

	///////////////////////////////////////////////////////////////
	//////////////////////////////////////////// WRITE IMAGES 0-255
	if (doWriteAvgTiles = true) {
		ntImage image = ntImage(333, 333);
		string url;
		for (int i = 0; i < 256; i++) {
			url = path_Out + "0-255\\"+ to_string(i) + ".jpg";
			float col = mapRange(0, 1, 0, 255, i);
			image.set_Col(col);
			image.save(url);
		}
	}
}
void tokyoApp01::run(){
	display();
}

void tokyoApp01::display(){
	///////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////
	////////////////////////////////////////////  2D DISPLAY CONENT
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, appWidth, 0, appHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//img_C.display();
	//img_H.display();
	//img_M.display();
	///////////////////////////////////////////////////////////////
}