#include "ntType.h"

using namespace jpw;

ntType::ntType(){}

void ntType::init(){

	if (FT_Init_FreeType(&library)){
		fprintf(stderr, "Could not init freetype library\n");
	}

	if (FT_New_Face(library, "FreeSans.ttf", 0, &face)) {
		fprintf(stderr, "Could not open font\n");
	}

	FT_Set_Pixel_Sizes(face, 0, 48);

}