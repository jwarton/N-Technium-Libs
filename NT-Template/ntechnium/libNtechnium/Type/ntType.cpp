#include "ntType.h"

ntType::ntType(){}

ntType::ntType(std::string type):type(type) {
	this->type = type;
	init();
}

ntType::ntType(ntVec3* pos, float point, std::string type)
	:pos(pos), point(point), type(type){
	init();
}
ntType::ntType(ntVec3* pos, float point, std::string font, std::string type)
	:pos(pos), point(point), font(font), type(type){
	init();
}
ntType::ntType(ntVec3* pos, float point, std::string font, std::string type, ntColor4f col)
	:pos(pos), point(point), font(font), type(type), col(col) {
	init();
}
ntType::ntType(ntVec3* pos, float point, ntColor4f col, std::string type)
	:pos(pos), point(point), col(col), type(type) {
	init();
}

void ntType::init() {

	const char * filename = url.c_str();
	num_chars = type.length();
	//angle -= 90;
	//angle = toRadians(angle);

	/// // PURE FREE TYPE
	error = FT_Init_FreeType(&library);
	if (error) {
		std::cout << "ERROR:  FREETYPE LIBRARY UNABLE TO INITIALIZE" << std::endl;
	}
	error = FT_New_Face(library, filename, 0, &face);
	if (error) {
		std::cout << "ERROR:  FREETYPE | NEW FACE: CONFIRM PATH TO FONT FILE" << std::endl;
	}
	error = FT_Set_Char_Size(face, 0, point * 64, dpi, 0);
	if (error) {
		std::cout << "ERROR:  FREETYPE | SET CHARACTER SIZE" << std::endl;
	}
	error = FT_Set_Pixel_Sizes(face, 0, point);
	if (error) {
		std::cout << "ERROR:  FREETYPE | SET PIXEL SIZE" << std::endl;
	}

	slot = face->glyph;

	//matrix.xx = (FT_Fixed)( cos(angle) * 0x10000L);
	//matrix.xy = (FT_Fixed)(-sin(angle) * 0x10000L);
	//matrix.yx = (FT_Fixed)( sin(angle) * 0x10000L);
	//matrix.yy = (FT_Fixed)( cos(angle) * 0x10000L);

	pen.x = 0;
	pen.y = (target_height * 64);

	error = FT_Load_Char(face, type['T'], FT_LOAD_RENDER);
	FT_Int max_Top = slot->bitmap_top;
	FT_Int baseline = -face->descender;
	FT_Int base_Height = face->descender;

	//error = FT_Load_Char(face, type['I'], FT_LOAD_RENDER);
	//FT_Int space = slot->bitmap.width;

	///std::cout << "origin to top" << max_Top << std::endl;
	init_Bitmap();

	for (int i = 0; i < num_chars; i++)
	{
		//FT_Set_Transform(face, &matrix, &pen);
		///LOAD GLYPH IMAGE INTO THE SLOT (ERASE PREVIOUS ONE)
		error = FT_Load_Char(face, type[i], FT_LOAD_RENDER);
		if (error) {
			std::cout << "ERROR:  FREETYPE | LOAD CHARACTER FAILED --- " << type[i] << std::endl;
			continue;
		}
		pen.y = (target_height - max_Top) * 64 - slot->metrics.height - (max_Top - slot->metrics.horiBearingY);
		draw_Bitmap(&slot->bitmap, pen.x/64, pen.y/64);

		// INCREMENT PEN POSITION
		pen.x += slot->advance.x;// +space;
	}

	FT_Done_Face(face);
	FT_Done_FreeType(library);
}
void ntType::init_Char(FT_Bitmap*  bitmap, FT_Int x, FT_Int y) {
	FT_Int  p, q;
	FT_Int  x_max = x + bitmap->width;
	FT_Int  y_max = y + bitmap->rows;

	for (q = bitmap->rows-1; q > -1; q--) {
		for (p = 0; p < bitmap->width;  p++) {
			float val = mapRange(0, 1, 0, 255, (int)bitmap->buffer[q * bitmap->width + p]);
			std::cout << (int)val;
		}
		std::cout << "\n";
	}
	std::cout << "\n";
	for (int i = 0; i < bitmap->rows * bitmap->width; i++) {
			float val = mapRange(0, 1, 0, 255, (int)bitmap->buffer[i]);
			//std::cout << (int)val;
	}
	std::cout << (bitmap->rows * bitmap->width) <<"\n";
}
void ntType::init_Bitmap() {
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			image[j][i] = 0;
		}
	}
}
void ntType::draw_Bitmap(FT_Bitmap*  bitmap, FT_Int x, FT_Int y) {
	FT_Int  i, j, p, q;
	FT_Int  y_max = y + bitmap->rows;
	FT_Int  x_max = x + bitmap->width;

	for (i = y, q = bitmap->rows - 1; i < y_max; i++, q--) {
		for (j = x, p = 0; j < x_max; j++, p++) {

			if (i < 0 || j < 0 || i >= HEIGHT || j >= WIDTH) continue;
			float val = mapRange(0, 1, 0, 255, (int)bitmap->buffer[q * bitmap->width + p]);
			image[j][i] = bitmap->buffer[q * bitmap->width + p];
		}
	}
}

void ntType::draw_Bitmap_X(FT_Bitmap*  bitmap, FT_Int x, FT_Int y){
	FT_Int  i, j, p, q;
	FT_Int  x_max = x + bitmap->width;
	FT_Int  y_max = y + bitmap->rows;

	//std::cout << bitmap->width << " | " << bitmap->rows << std::endl;
	//std::cout << x_max << " | " << y_max << std::endl;

	for (i = x, p = 0; i < x_max; i++, p++)	{
		for (j = y, q = 0; j < y_max; j++, q++) {

			if (i < 0 || j < 0 || i >= HEIGHT || j >= WIDTH) continue;

			if (bitmap->buffer[q * bitmap->width + p] == 0) {
				image[j][i] = 0;
			} else if (bitmap->buffer[q * bitmap->width + p] < 128) {
				image[j][i] = 0;
			} else {
				image[j][i] = 128;
			}
			float val = mapRange(0, 1, 0, 255, (int)bitmap->buffer[q * bitmap->width + p]);
			std::cout << (int)val;
			image[j][i] = val;

			image[j][i] |= bitmap->buffer[q * bitmap->width + p];
		}
		std::cout << "\n";
	}
	//GLuint texture;
	//glGenTextures(1, &texture);
	//glBindTexture(GL_TEXTURE_2D, texture);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, bitmap->width, bitmap->rows, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap->buffer);
}
void ntType::set_Pos(ntVec3* pos) {
	this->pos = pos;
}
void ntType::set_Color(ntColor4f col) {
	this->col = col;
}
void ntType::set_Weight(int w) {
	this->w = w;
}

void ntType::render_Text(){
	//// Activate corresponding render state	
	////shader.Use();
	////glUniform3f(glGetUniformLocation(shader.Program, "textColor"), color.x, color.y, color.z);
	//glActiveTexture(GL_TEXTURE0);
	//glBindVertexArray(VAO);

	//// Iterate through all characters
	//std::string::const_iterator c;
	//for (c = type.begin(); c != type.end(); c++)
	//{
	//	character ch = characters[*c];

	//	GLfloat xpos = x + ch.Bearing.x * scale;
	//	GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

	//	GLfloat w = ch.Size.x * scale;
	//	GLfloat h = ch.Size.y * scale;
	//	// Update VBO for each character
	//	GLfloat vertices[6][4] = {
	//		{ xpos,     ypos + h,   0.0, 0.0 },
	//		{ xpos,     ypos,       0.0, 1.0 },
	//		{ xpos + w, ypos,       1.0, 1.0 },

	//		{ xpos,     ypos + h,   0.0, 0.0 },
	//		{ xpos + w, ypos,       1.0, 1.0 },
	//		{ xpos + w, ypos + h,   1.0, 0.0 }
	//	};
	//	// Render glyph texture over quad
	//	glBindTexture(GL_TEXTURE_2D, ch.TextureID);
	//	// Update content of VBO memory
	//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

	//	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//	// Render quad
	//	glDrawArrays(GL_TRIANGLES, 0, 6);
	//	// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
	//	x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	//}
	//glBindVertexArray(0);
	//glBindTexture(GL_TEXTURE_2D, 0);
}
void ntType::run() {

}
void ntType::display() {
	glPointSize(1);
	glBegin(GL_POINTS);

	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			float col_A = mapRange(0, 0.75, 0, 255, image[i][j]);
			glColor4f(col.r, col.g, col.b, col_A);
			glVertex2f(pos->x + i, pos->y + j - HEIGHT);
		}
	}
	glEnd();
}