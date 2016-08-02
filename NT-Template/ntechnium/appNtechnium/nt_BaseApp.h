///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////// nt_BaseApp.h
// openGl scratch libs							///////////////////
// BaseApp Class for all derived applications	///////////////////
// created by James Warton on 04/01/2014		///////////////////
///////////////////////////////////////////////////////////////////
#ifndef BASEAPP_JPW_NTECHNIUM
#define BASEAPP_JPW_NTECHNIUM

#include <Windows.h>
#include <GL/glew.h>
#include <GL/glu.h>			
#include <GLFW/glfw3.h>

///////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <memory>
///////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////// INCLUDE GLM
#include "glm/gtc/type_ptr.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform2.hpp"
///////////////////////////////////////////////////////////////////
//////////////////////////// INCLUDE NTECHNIUM OBJECT CLASS LIBRARY
#include "ntVec3.h"
#include "ntColor4f.h"
#include "ntMatrix4.h"
#include "ntShader.h"
#include "ntLight.h"
//#include "nt_CoutTemp.h"
//#include "ntPtsTxt.h"
///////////////////////////////////////////////////////////////////
////////////////////////////// INCLUDE NTECHNIUM GEOM CLASS OBJECTS
#include "ntVertex.h"
#include "ntEdge.h"
#include "ntFace3.h"
#include "ntTetra.h"
#include "ntSphere.h"
#include "ntBaseShape.h"
///////////////////////////////////////////////////////////////////
////////////////////// PREPROCESSING DIRECTORY FOR RESOURCE LOADING
#if defined (_WIN32) || defined (_WIN64)///
	#include <stdio.h> ////////////////////////////defines FILENAME_MAX
	#include <direct.h>
	#define GetCurrentDir _getcwd///
#endif///
#define BUFFER_OFFSET(i) ((void*)(i))

namespace jpw{

	class ntGLFWsetup;///////////////////////////////// FORWARD DECLARE
	class ntBaseApp{
		friend class ntGLFWsetup;
	public:
		//GLFW MOUSE EVENTS
		void setMouseButton(int mouseAction, int mouseButton, int mouseMods);
		void setMouseScroll(float xoffset, float yoffset);
		
		//SAVED CAMERA SETTINGS
		/// ADD FUNCTIONS TO ACCESS AND UPDATE ACTIVE CAMERA
		GLfloat cam00[14];
		GLfloat cam01[14];
		GLfloat cam02[14];
		GLfloat cam03[14];
		GLfloat cam04[14];
		GLfloat cam05[14];

		//ACTIVE CAMERA SETTINGS
		GLfloat rotX;
		GLfloat rotY;		
		GLfloat rotZ;		//ROTATES MODEL MATRIX

		GLfloat camX;
		GLfloat camY;
		GLfloat camZ;
		GLfloat tarX;
		GLfloat tarY;
		GLfloat tarZ;
		GLfloat rolX;
		GLfloat rolY;
		GLfloat rolZ;
		GLfloat fovA;
		GLfloat zoom;

		float width = 1.0;
		float height = 1.0;
		float zNear = 0.1;
		float zFar = 100000;

		int view_T = GLFW_KEY_T;
		int view_B = GLFW_KEY_B;
		int view_L = GLFW_KEY_L;
		int view_R = GLFW_KEY_R;
		int view_F = GLFW_KEY_F;
		int view_P = GLFW_KEY_P;

		int view_ZI = GLFW_KEY_LEFT_BRACKET;
		int view_ZO = GLFW_KEY_RIGHT_BRACKET;

		///////////////////////////////////////////////////CONSTRUCTORS
		ntBaseApp();

	private:
		void setWidth(int canvasWidth);
		void setHeight(int canvasHeight);
		///DIM CLASS UNDEFINED
		//void setSize(const Dim2i& canvasSize);

	protected:
		GLFWwindow* window;

		std::string title;
		int appWidth;
		int appHeight;
		int canvasWidth;
		int canvasHeight;
		///DIM CLASS UNDEFINED
		//Dim2i canvasSize;
		//int width;
		//int height;

		ntColor4f bgColor;
		int frameCount;
		float frameRate;

		///////////////////////////////////////////////////////////////
		//////////////////////////////////////// GL DEPENDENT VARIABLES
		///////////////////////////////////////////// PER GL LIMITS 0-7
		GLint glLights[8];
		enum Light {
		LIGHT_0,
		LIGHT_1,
		LIGHT_2,
		LIGHT_3,
		LIGHT_4,
		LIGHT_5,
		LIGHT_6,
		LIGHT_7
		};

		//ntLight light0, light1, light2, light3, light4, light5, light6, light7;
		//std::shared_ptr<ntLight> lights[8];
		std::vector<ntLight> lights;

		enum Matrix {MODEL_VIEW, PROJECTION};
		///////////////////////////////////////////////////////////////
		///////////////////////////////////////////////// GLSL UNIFORMS
		////////////////////////////// using initials ONLY for matrices
		/////////////////////////////////////// Uniform Camera Matrices
		glm::mat4 M, V, MV, P, MVP;
		///////////////////////////////////////////////////////////////
		/////////////////////////////// Uniform Transformation Matrices
		glm::mat4 T, R, S;
		///////////////////////////////////////////////////////////////
		/////////////////////////////////// Uniform Shadow Map Matrices
		glm::mat4 L_V, L_MV, L_P, L_B, L_BP, L_MVP;
		///////////////////////////////////////////////////////////////
		///////////////////////////////////////// Uniform Normal Matrix
		glm::mat3 N;
		///////////////////////////////////////////////////////////////
		//////////////////////////////////// flags for shader locations
		GLuint M_U, V_U, MV_U, P_U, MVP_U, N_U;
		GLuint T_U, R_U, S_U;
		GLuint L_MVP_U;					  // only for light perspective
		GLuint shadowMapTex;                // id for shadermap texture
		
		//////////////////////////////// Uniform Lighting location vars
		GLuint lightPos0_U, lightPos1_U, lightPos2_U, lightPos3_U, lightPos4_U, lightPos5_U, lightPos6_U, lightPos7_U;
		GLuint lightDiffuse0_U, lightDiffuse1_U, lightDiffuse2_U, lightDiffuse3_U, lightDiffuse4_U, lightDiffuse5_U, lightDiffuse6_U, lightDiffuse7_U;
		GLuint lightSpecular0_U, lightSpecular1_U, lightSpecular2_U, lightSpecular3_U, lightSpecular4_U, lightSpecular5_U, lightSpecular6_U, lightSpecular7_U;
		GLuint lightAmbient0_U, lightAmbient1_U, lightAmbient2_U, lightAmbient3_U, lightAmbient4_U, lightAmbient5_U, lightAmbient6_U, lightAmbient7_U;
		GLuint lightEmissive0_U, lightEmissive1_U, lightEmissive2_U, lightEmissive3_U, lightEmissive4_U, lightEmissive5_U, lightEmissive6_U, lightEmissive7_U;
		GLuint lightReflectionConst0_U, lightReflectionConst1_U, lightReflectionConst2_U, lightReflectionConst3_U, lightReflectionConst4_U, lightReflectionConst5_U, lightReflectionConst6_U, lightReflectionConst7_U;
		GLuint lightIntensity0_U, lightIntensity1_U, lightIntensity2_U, lightIntensity3_U, lightIntensity4_U, lightIntensity5_U, lightIntensity6_U, lightIntensity7_U;
		
		//////////////////////////////////////////////////// Shadow Map
		GLuint shadowMap_U;
		
		///////////////////////////////////////////////////////////////
		//////////////////// PURE VIRTUAL FUNCTIONS- OVERIDE IN DERIVED
		virtual void init() = 0;					//void init();/////
		virtual void run() = 0;						//void run();//////

		///void runWorld();

		///////////////////////////////////////////////////////////////
		void setFrameRate(float frameRate);
		void setFrameCount(float frameCount);
		float getFrameRate()const;
		int getFrameCount()const;

		void setBackground(float r, float g, float b);
		void setBackground(float c);
		void setBackground(const ntColor4f& col);

		int getWidth()const;
		int getHeight()const;
		///////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////  LOAD IMAGE
		//void loadImage(std::string url);
		///////////////////////////////////////////////////////////////
		//////////////////////////////////////  PRINT SYS GRAPHICS INFO
		void GLSLInfo(ntShader* shader);
		
		///virtual void initUniforms(); // virtual for now

		///////////////////////////////////////////////////////////////
		////////////////////////////////////////// GLOBAL LIGHT CONTROL
		void lightsOn();
		void lightsOff();

		///CAMERAS TODO

		///
		void printMatrix(Matrix m = MODEL_VIEW);
	};
	//////////////////////////////////////////////////// INLINE METHODS
	///////////////////////////////////////////////////////////////////
	inline void ntBaseApp::setFrameRate(float frameRate){
		this->frameRate = frameRate;
	}
	inline void ntBaseApp::setFrameCount(float frameCount){
		this->frameCount = frameCount;
	}

	inline float ntBaseApp::getFrameRate() const{
		return frameRate;
	}
	inline int ntBaseApp::getFrameCount() const{
		return frameCount;
	}

///VISUALIZATION SETTING IMPLEMENTED FROM DERIVED 
//#define POINTS ntGeom3::POINTS
//#define WIREFRAME ntGeom3::WIREFRAME
//#define SURFACE ntGeom3::SURFACE

#define pushMatrix glPushMatrix
#define popMatrix glPopMatrix

#define translatef glTranslatef
#define rotatef glRotatef 
#define scalef glScalef 

#define light0 lights.at(0)
#define light1 lights.at(1)
#define light2 lights.at(2)
#define light3 lights.at(3)
#define light4 lights.at(4)
#define light5 lights.at(5)
#define light6 lights.at(6)
#define light7 lights.at(7)
}
#endif
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////