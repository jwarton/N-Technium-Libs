#include "nt_11_c3-App-v00.h"
#include <GL/glu.h>

void tokyoApp00::init() {
	///////////////////////////////////////////////////////////////
	path_Out = path_Out + pathExtension;

	for (int i = 0; i < 70; i++) {

		stringstream ss;
		ss << std::setw(4) << std::setfill('0');
		ss << i;
		fileName = ss.str();

		url_IMG = path_IMG + pathExtension_IMG + fileName + fileExt;
		
		img = ntImage(url_IMG);

		img.set_Dim(1000);
		img.save(path_Out, fileName + "_a" + fileExt);

		img.sort(0);
		img.save(path_Out, fileName + "_b" + fileExt);

		img.add();
		img.save(path_Out, fileName + "_c" + fileExt);

		img.reset();
		img.sort(0);
		img.invert();
		img.sub();
		img.save(path_Out, fileName + "_d" + fileExt);

		//img.reset();
		//img.blur();
		//img.save(path_Out, fileName + "_e" + fileExt);

	}
}

void tokyoApp00::run(){
	display();
}

void tokyoApp00::display(){
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

	img.display();
	///////////////////////////////////////////////////////////////
}