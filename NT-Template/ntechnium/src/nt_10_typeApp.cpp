#include "nt_10_typeApp.h"

void typeApp::init() {
	///////////////////////////////////////////////////////////////
	Vec3 pt0 = Vec3(0, 0, 0);
	cubeA = ntCube(pt0, 2);


	const char * filename = "C:\\verdana.ttf";// url.c_str();
	///
	//monochrome = 0; // new OGLFT::Monochrome(filename);
	////grayscale = new OGLFT::Grayscale(filename, 12, 100);

	//if (monochrome == 0 || !monochrome->isValid()) {
	//	cerr << "\nCOULD NOT CONSTRUCT FACE FROM: " << filename << "\n" << endl;
	//	return;
	//}
	////monochrome->setForegroundColor(1., 0., 0.);

	//// FOR THE RASTER STYLES, IT IS ESSENTIAL THAT THE PIXEL STORE
	//// UNPACKING ALIGNMENT BE SET TO 1
	////glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	/////

	//// CREATE A PIXMAP FONT FROM A TRUETYPE FILE
	//FTGLPixmapFont message("C:\\verdana.ttf");
	////message.FaceSize(72);
	////message.Render("Hello World!");

	///// PURE FREE TYPE

	//FT_Face face;
	//FT_Init_FreeType(&library);
	//FT_New_Face(library, "fonts/arial.ttf", 0, &face);

	//if (FT_Init_FreeType(&library))
	//	std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	//if (FT_New_Face(library, "fonts/arial.ttf", 0, &face))
	//	std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
}
void typeApp::run(){
	display();
}

void typeApp::display(){
	///////////////////////////////////////////////////////////////
	//cubeA.display();
	//cubeA.displayVerts(2);
	cubeA.displayEdges(.1);
	//cubeA.displayNorms(.12);
	///////////////////////////////////////////////////////////////

	//monochrome->draw(0., 250., "Hello, World!");
	//font.Render("Hello World!");
}