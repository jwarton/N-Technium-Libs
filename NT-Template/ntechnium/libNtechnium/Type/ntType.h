///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////// ntType.h
// openGl scratch libs							///////////////////
// Typography Manager Class						///////////////////
// created by James Warton on 10/05/2015		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef TYPE_CONTENT_JPW_NTECHNIUM
#define TYPE_CONTENT_JPW_NTECHNIUM
#define _USE_MATH_DEFINES

#include <iostream>
#include <map>
#include <string>
/// GLEW
//#define GLEW_STATIC
//#include <GL/glew.h>

/// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include "Shader.h"

/// FREETYPE2
#include <ft2build.h>
#include FT_FREETYPE_H                                           
/// uses free type with simplified implementation
//#include <FTGL/ftgl.h>

/// NT-libs
#include "ntVec3.h"
#include "ntColor4f.h"
#include "ntMath.h"
#include "nt_Utility.h"

//#pragma warning(disable: 4786)
#define WIDTH   1200
#define HEIGHT  50

struct character {
	GLuint texture_ID;
	glm::ivec2 size;
	glm::ivec2 bearing;
	GLuint advance;
};

//std::map<GLchar, character> characters;
//void renderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

using namespace jpw;
class ntType {
private:
	std::string path_TTF =	nt_Utility::getPathToResources() + "fonts\\";
	std::string font =		"AGENCYR";
	std::string extention = ".TTF";
	std::string url = path_TTF + font + extention;

	FT_Library		library;
	FT_Face			face;
	FT_GlyphSlot	slot;
	FT_Matrix		matrix;                 // TRANSFORMATION MATRIX
	FT_Vector		pen;                    // UNTRANSFORMED ORIGIN
	FT_Error		error;
	unsigned char	image[WIDTH][HEIGHT];

	std::string		type;
	char*			text; //may be unnecessary
	int				num_chars;
	
	//Shader shader;
	//GLuint * textures;					// TEXTURE ID
	//GLuint list_base;						// FIRST DISPLAY LIST ID

	////TYPE POSITION UPPER RIGHT
	double			angle = 0;				
	int				target_height = HEIGHT;

	////TYPE PARAMETERS
	ntVec3*		dim;
	float		dimX;
	float		dimY;
	float		dimZ;
	int			w =		1;
	int			point = 12;
	int			dpi =	72;
	ntColor4f	col =	Col4(1, 1, 1, 1);

	////FUNCTIONS
	void init();
	void draw_Bitmap(FT_Bitmap*  bitmap, FT_Int x, FT_Int y);
	void draw_Bitmap_X(FT_Bitmap*  bitmap, FT_Int x, FT_Int y);
	void init_Bitmap();
	void init_Char(FT_Bitmap*  bitmap, FT_Int x, FT_Int y);
	void render_Text();

public:
	ntVec3* pos = new ntVec3 (0, 0, 0);

	ntType();
	ntType(std::string type);
	ntType(ntVec3* pos, float point, std::string type);
	ntType(ntVec3* pos, float point, std::string font, std::string type);
	ntType(ntVec3* pos, float point, std::string font, std::string type, ntColor4f col);
	ntType(ntVec3* pos, float point, ntColor4f col, std::string type);

	//void set_Type(std::string type);
	//void set_Font(std::string font);
	void set_Weight(int w);
	void set_Color(ntColor4f col);
	void set_Pos(ntVec3* pos);

	void run();
	void display();
};
#endif