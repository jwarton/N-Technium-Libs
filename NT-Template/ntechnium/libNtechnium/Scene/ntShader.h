///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// ntShader.h
// openGl scratch libs							///////////////////
// Shader Class									///////////////////
// created by James Warton on 02/23/2014		///////////////////
///////////////////////////////////////////////////////////////////
#ifndef SHADER_JPW_NTECHNIUM
#define SHADER_JPW_NTECHNIUM

#include <GL/glew.h>
#include <GL/glu.h>
///#include <GLFW/glfw3.h>///

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "nt_Utility.h"

#include <sstream>
#include <fstream>

namespace jpw {

	class ntShader {
	public:
		ntShader();
		ntShader(const std::string& vShader, const std::string& fShader);
		ntShader(const ntShader& shader);
		ntShader& operator=(const ntShader& shader);
		~ntShader();

		void init();

		void bind();
		void unbind();

		GLuint getID();
		GLuint shader_id;

		///////////////////////////////////////////////////////////
		///////////////////// ENCAPSULATE FOR ATTRIBUTE AND UNIFORM
		void addAttribute(std::string name);
		void addUniform(std::string name);
		
	private:
		///////////////////////////////////////////////////////////
		////////////////////////////////////// INITILIZE BEFORE USE
		std::string vShader, fShader;
		GLuint shader_vp;
		GLuint shader_fp;
		void printLog(GLuint program);
		///////////////////////////////////////////////////////////
		////////////////////////// VECTOR FOR ATTRIBUTE AND UNIFORM
		std::vector<GLuint> attribLocs, uniformLocs;

	};
	inline GLuint ntShader::getID(){
		return shader_id;
	}

	inline void ntShader::bind(){
		glUseProgram(shader_id);
	}
	
	inline void ntShader::unbind(){
		glUseProgram(0);
	}

}
#endif
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////