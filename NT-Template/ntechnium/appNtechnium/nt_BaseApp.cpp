///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////// nt_BaseApp.cpp
// openGl scratch libs							///////////////////
// BaseApp Class for all derived applications	///////////////////
// created by James Warton on 04/01/2014		///////////////////
///////////////////////////////////////////////////////////////////
#include "nt_BaseApp.h"
using namespace jpw;

///////////////////////////////////////////////////////////////////
//////////////////////////////////////////// CALL BEFORE GL CONTEXT
ntBaseApp::ntBaseApp(){

	glLights[0] = GL_LIGHT0;
	glLights[1] = GL_LIGHT1;
	glLights[2] = GL_LIGHT2;
	glLights[3] = GL_LIGHT3;
	glLights[4] = GL_LIGHT4;
	glLights[5] = GL_LIGHT5;
	glLights[6] = GL_LIGHT6;
	glLights[7] = GL_LIGHT7;

	lights.push_back(ntLight());
	lights.push_back(ntLight());
	lights.push_back(ntLight());
	lights.push_back(ntLight());
	lights.push_back(ntLight());
	lights.push_back(ntLight());
	lights.push_back(ntLight());
	lights.push_back(ntLight());
}

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////// ENABLE SCENE LIGHTING
void ntBaseApp::lightsOn(){
	glEnable(GL_LIGHTING);

}
void ntBaseApp::lightsOff(){
	glDisable(GL_LIGHTING);
}
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////// GENERAL FUNCTIONS
///////////////////////////////////////////////// OVERLOAD BASE APP
void ntBaseApp::setBackground(float r, float g, float b){
	bgColor.r = r;
	bgColor.g = g;
	bgColor.b = b;
}
void ntBaseApp::setBackground(float c){
	setBackground(c, c, c);
}
void ntBaseApp::setBackground(const ntColor4f& col){
	setBackground(col.r, col.g, col.b);
}

///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////// WINDOW PARAMETER
void ntBaseApp::setWidth(int canvasWidth){
	this->width = canvasWidth;
	width = canvasWidth;
}
void ntBaseApp::setHeight(int canvasHeight){
	this->height = canvasHeight;
	height = canvasHeight;
}
void ntBaseApp::printMatrix(Matrix m){
	GLfloat matrix[16];
	if (m == MODEL_VIEW) {
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
		std::cout << "\nGL ModelView Matrix" << std::endl;
	}
	else {
		glGetFloatv(GL_PROJECTION_MATRIX, matrix);
		std::cout << "\nGL Projection Matrix" << std::endl;
	}
	std::cout << matrix[0] << " | " << matrix[4] << " | " << matrix[8] << " | " << matrix[12] << "\n";
	std::cout << matrix[1] << " | " << matrix[5] << " | " << matrix[9] << " | " << matrix[13] << "\n";
	std::cout << matrix[2] << " | " << matrix[6] << " | " << matrix[10] << " | " << matrix[14] << "\n";
	std::cout << matrix[3] << " | " << matrix[7] << " | " << matrix[11] << " | " << matrix[15] << std::endl;
}
///TODO LOW PRIORITY
/*
void ntBaseApp::ntSetSize(Dim2i canvasSize){
	this->canvasSize = canvasSize;
}
*/
///////////////////////////////////////////////////////////////////
//////////////////////////////////////////// KEYBOARD | MOUSE INPUT
void ntBaseApp::setMouseButton(int mouseAction, int mouseButton, int mouseMods){
	if (mouseAction == 1){

		//std::cout << "MOUSE PRESSED 1" << endl;
		//isMousePressed = true;

		//// arcball
		//mouseXIn = mouseX;
		//mouseYIn = mouseY;

		//mousePressed();
	}
	else if (mouseAction == 0){

		//std::cout << "MOUSE PRESSED 0" << endl;
		//isMousePressed = false;

		//// arcball
		//arcballRotXLast = arcballRotX;
		//arcballRotYLast = arcballRotY;

		//mouseReleased();
	}
}
void ntBaseApp::setMouseScroll(float xoffset, float yoffset){

	testFun();

	//std::cout << "X OFFSET:  " << xoffset << endl;
	//std::cout << "Y OFFSET:  " << yoffset << endl;
}
void ntBaseApp::testFun(){
	//zoom += yoffset;
	//std::cout << "ZOOM:  " << zoom << endl;
	//std::cout << zFar << endl;
}
int ntBaseApp::getWidth()	const{
	return width;
}
int ntBaseApp::getHeight()	const{
	return height;
}

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////// LOAD BACKGROUNG IMAGE
///TODO
///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// LOAD IMAGE
///TODO
///////////////////////////////////////////////////////////////////
/////////////////////////////////////////// PRINT SYS GRAPHICS INFO
void ntBaseApp::GLSLInfo(ntShader* shader){
	///////////////////////////////////////////////////////////////
	///////////////////////////////////// START GET UNIFORM SHADERS
	GLint nUniforms, maxLen;
	glGetProgramiv(shader->getID(), GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLen);
	glGetProgramiv(shader->getID(), GL_ACTIVE_UNIFORMS, &nUniforms);

	GLchar* name = (GLchar*)malloc(maxLen);

	GLint size, location;
	GLsizei written;
	GLenum type;
	printf("\n");
	printf("///////////////////////////////////////////////////////////////\n");
	printf("/////////////////////////////////////////////// LOCATION | NAME\n");

	
	for (int i = 0; i < nUniforms; ++i){
		glGetActiveUniform(shader->getID(), i, maxLen, &written, &size, &type, name);
		location = glGetUniformLocation(shader->getID(), name);
		///printf(" %-8d | %s\n", location, name);
	}

	free(name);
	/////////////////////////////////// END GET ALL UNIFORM SHADERS

	///////////////////////////////////////////////////////////////
	////////////////////////////////////// GET RENDERER INFORMATION
	trace("renderer	:	", glGetString(GL_RENDERER));
	trace("vendor		:	", glGetString(GL_VENDOR));
	trace("version		:	", glGetString(GL_VERSION));
	trace("glslVersion	:	", glGetString(GL_SHADING_LANGUAGE_VERSION));

	trace("vertexPosition Location	= ", glGetAttribLocation(shader->getID(), "vertexPosition"));
	trace("vertexNormal Location	= ", glGetAttribLocation(shader->getID(), "vertexNormal"));
	trace("vertexColor Location	= ", glGetAttribLocation(shader->getID(), "vertexColor"));
	trace("vertexTexture Location	= ", glGetAttribLocation(shader->getID(), "vertexTexture"));
	}
