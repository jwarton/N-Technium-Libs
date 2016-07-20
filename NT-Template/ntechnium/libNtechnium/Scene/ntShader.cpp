///////////////////////////////////////////////////////////////////
#include "ntShader.h"

using namespace jpw;
///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////// CONSTRUCTORS
ntShader::ntShader(){
	init();
}
ntShader::ntShader(const std::string& vShader, const std::string& fShader):
vShader(vShader), fShader(fShader){
	init();
}
ntShader::ntShader(const ntShader& shader){
	this->vShader = shader.vShader;
	this->fShader = shader.fShader;
	this->shader_vp = shader.shader_vp;
	this->shader_fp = shader.shader_fp;
	init();
}
///////////////////////////////////////////////// COPY CONSTRUCTORS
ntShader& ntShader::operator=(const ntShader& shader){
	if (this != &shader){
		glDetachShader(shader_id, shader_fp);
		glDetachShader(shader_id, shader_vp);

		glDeleteShader(shader_fp);
		glDeleteShader(shader_vp);
		glDeleteShader(shader_id);

		this->vShader = shader.vShader;
		this->fShader = shader.fShader;
		this->shader_vp = shader.shader_vp;
		this->shader_fp = shader.shader_fp;
		init();
	}
	return *this;
}
//////////////////////////////////////////////////////// DESTRUCTOR
ntShader::~ntShader(){
	/////////////////////////////////////////////// MEMORY CLEAN UP
	if (attribLocs.size() > 0){
		attribLocs.clear();
	}
	if (uniformLocs.size() > 0){
		uniformLocs.clear();
	}
	glDetachShader(shader_id, shader_fp);
	glDetachShader(shader_id, shader_vp);

	glDeleteShader(shader_fp);
	glDeleteShader(shader_vp);
	glDeleteProgram(shader_id);
}
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
void ntShader::init(){
	GLenum err = glewInit();
	
	if (GLEW_OK != err){
		//PROBLEM: glewInit FAIL!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!////
		///fprintf(stderr, "ERROR: %s\n", glewGetErrorString(err));
	}
	///fprintf(stdout, "STATUS:  USING GLEW %s\n", glewGetString(GLEW_VERSION));
	
	///std::string url = nt_Utility::getPathToResources() + "\shaders\\";  
	//nt_Utility::getBuildPath();
	///std::cout << "PATH TO SHADERS URL:  " << url << std::endl;
	std::string vShaderURL;
	std::string fShaderURL;
}

void ntShader::printLog(GLuint program){
	int maxLength = 0;
	int length = 0;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
	char* log = new char[maxLength];
	glGetProgramInfoLog(program, maxLength, &length, log);
	std::cout << "PROGRAM LOG: " << log << std::endl;
}

///////////////////////////////////////////////////////////////////
///////////////////////////////////////// FOR ATTRIBUTE AND UNIFORM
void ntShader::addAttribute(std::string name){
	///	WHY ARE THESE BLANK?! ASK IRA?
}
void ntShader::addUniform(std::string name){
	///	WHY ARE THESE BLANK?!  ASK IRA?
}
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////