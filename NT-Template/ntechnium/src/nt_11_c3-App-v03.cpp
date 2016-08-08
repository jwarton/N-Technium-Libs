#include "nt_11_c3-App-v03.h"
#include <GL/glu.h>

void tokyoApp03::init() {

	bool doHostImage		= true;
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
		fileName = "hercules_001";
		url_IMG = path_IMG + fileName + fileExt;
		img_H = ntImage(url_IMG);

		/// WRITE IMAGE PATH
		url_OUT = path_Out + fileName + "_TEST" + fileExt;

		int w = 48 * 72;
		int h = 96 * 72;
		float tile_size = 4;// 1;// 0.125;//0.5; // 

		img_H.set_Dim(w, h);				// RESIZE IMAGE 

		float img_w = floor(img_H.get_Width() / 72);
		float img_h = floor(img_H.get_Height() / 72);

		std::cout << "width: " << img_w << " height: " << img_h << endl;

		int img_x = img_w * 72;
		int img_y = img_h * 72;

		std::cout << "dim_x: " << img_x << " dim_y: " << img_y << endl;
		//img_H.crop(1, img_x-1, 1, img_y-1);

		int tiles_x = img_w * (1 / tile_size);
		int tiles_y = img_h * (1 / tile_size);

		std::cout << "tiles_x: " << tiles_x << " tiles_y: " << tiles_y << " total tiles: " << tiles_y * tiles_x << endl;

		//img_H.tile(20, 10, path_Out);
		//img_H.tile_GPU(tiles_x, tiles_y);

		img_H.tile_GPU_02(tiles_x, tiles_y, 10);

		img_H.save(url_OUT);
	}
	///////////////////////////////////////////////////////////////
	/////////////////////////////// SAVE CONVULUTIONS FILTERS- BLUR
	if (doFilter == true) {
		fileName = "hercules_001";
		string url = path_IMG + fileName + fileExt;
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

			int w = 48 * 300;
			int h;

			img_T.set_Dim(w);				// RESIZE IMAGE 

			//SUBDIVIDE AND SAVE IMAGE
			//img_T.tile(48, 2, path_Out);
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
}
void tokyoApp03::run(){
	display();
}

void tokyoApp03::display(){
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
	img_H.display();
	//img_M.display();
	///////////////////////////////////////////////////////////////
}