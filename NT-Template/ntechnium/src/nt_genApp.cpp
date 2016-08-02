#include "nt_genApp.h"
genApp::genApp(AppContent* c):
content(c){
}

void genApp::init() {
	///////////////////////////////////////////////////////////////
	////////////////////////////////////////////// BACKGROUND COLOR
	glClearColor(0, 0, 0, 1);

	//globalAmbient = ntLight(ntColor4f(.25, .19, .27, 1));
	//globalAmbient = ntLight(ntColor4f(.25, .5, .5, 1));

	///////////////////////////////////////////////////////////////
	////////////////////////////////////////// SCENE LIGHTING:  0-7 
	/////////////////////////////////////////// ogpen gl supports 8
	light0.setPos(ntVec3(0, 0, 1));
	light0.setDiffuse(ntColor4f(1, 1, 1, 1.0f));
	light0.setAmbient(ntColor4f(.3, .3, .3, 1.0));
	light0.setSpecular(ntColor4f(1, 1, 1, 1.0));
	light0.on();

	//shader = ntShader("shader1.vert", "shader1.frag");

	///////////////////////////////////////////////////////////////
	////////////////////////////////////// GET INFO ABOUT GLSL SATE
	//GLSLInfo(&shader);

	///////////////////////////////////////////////////////////////
	///////////////////////////////////////////////// SCENE CONTENT

	///
	content->window = window;
	content->init();
	
	content->set_Bounds();
	content->set_Centroid();
	content->set_Grid();

	ntVec3* min = new ntVec3();
	ntVec3* max = new ntVec3();
	ntVec3* cen = new ntVec3();
	content->get_Bounds(min, max);
	content->get_Centroid(cen);

	fovA = 15;
	zoom = 100;

	///REFINE FIT FUNCTION AND CAMERA POSITON
	float BS_X = max->x - min->x;
	float BS_Y = max->y - min->y;
	float BS_Z = max->z - min->z;

	float dim_Max = std::max(BS_X, BS_Y);
	dim_Max = std::max(dim_Max, BS_Z);

	float BS_radius = abs((dim_Max) * 0.5);
	float V_magnitude = sin(fovA + zoom) * BS_radius;
	float d = abs(V_magnitude);

	//INITIALIZE DEFAULT CAMERA
	rotX = 0.0f;
	rotY = 0.0f;
	rotZ = 0.0f;
	camX = min->x;
	camY = min->y;
	camZ = cen->z + d;
	tarX = cen->x;
	tarY = cen->y;
	tarZ = cen->z;
	rolX = 0.0f;
	rolY = 0.0f;
	rolZ = 1.0f;

	//PERSPECTIVE VIEW  | FIT TO APPCONTENT
	cam00[0]  = 0.0;		//rotX;  
	cam00[1]  = 0.0;		//rotY;  
	cam00[2]  = 0.0;		//rotZ;  
	cam00[3]  = min->x;		//camX;  
	cam00[4]  = min->y;		//camY;  
	cam00[5]  = cen->z + d;	//camZ;  
	cam00[6]  = cen->x;		//tarX;  
	cam00[7]  = cen->y;		//tarY;  
	cam00[8]  = cen->z;		//tarZ;  
	cam00[9]  = 0.0;		//rolX;  
	cam00[10] = 0.0;		//rolY;  
	cam00[11] = 1.0;		//rolZ;  
	cam00[12] = fovA;		//fovA;  
	cam00[13] = zoom;		//zoom; 

	//TOP VIEW
	cam01[0]  = 0.0;		//rotX;  
	cam01[1]  = 0.0;		//rotY;  
	cam01[2]  = 0.0;		//rotZ;  
	cam01[3]  = cen->x;		//camX;  
	cam01[4]  = cen->y;		//camY;  
	cam01[5]  = max->z + d;	//camZ;  
	cam01[6]  = cen->x;		//tarX;  
	cam01[7]  = cen->y;		//tarY;  
	cam01[8]  = cen->z;		//tarZ;  
	cam01[9]  = 0.0;		//rolX;  
	cam01[10] = 1.0;		//rolY;  
	cam01[11] = 0.0;		//rolZ;  
	cam01[12] = fovA;		//fovA;  
	cam01[13] = zoom;		//zoom;  

	//FRONT VIEW
	cam02[0]  = 0.0;		//rotX;  
	cam02[1]  = 0.0;		//rotY;  
	cam02[2]  = 0.0;		//rotZ;  
	cam02[3]  = cen->x;		//camX;  
	cam02[4]  = min->y - d;	//camY;  
	cam02[5]  = cen->z;		//camZ;  
	cam02[6]  = cen->x;		//tarX;  
	cam02[7]  = cen->y;		//tarY;  
	cam02[8]  = cen->z;		//tarZ;  
	cam02[9]  = 0.0;		//rolX;  
	cam02[10] = 0.0;		//rolY;  
	cam02[11] = 1.0;		//rolZ;  
	cam02[12] = fovA;		//fovA;  
	cam02[13] = zoom;		//zoom;  

	//RIGHT VIEW
	cam03[0]  = 0.0;		//rotX;  
	cam03[1]  = 0.0;		//rotY;  
	cam03[2]  = 0.0;		//rotZ;  
	cam03[3]  = max->x + d;	//camX;  
	cam03[4]  = cen->y;		//camY;  
	cam03[5]  = cen->z;		//camZ;  
	cam03[6]  = cen->x;		//tarX;  
	cam03[7]  = cen->y;		//tarY;  
	cam03[8]  = cen->z;		//tarZ;  
	cam03[9]  = 0.0;		//rolX;  
	cam03[10] = 0.0;		//rolY;  
	cam03[11] = 1.0;		//rolZ;  
	cam03[12] = fovA;		//fovA;  
	cam03[13] = zoom;		//zoom;  

	//LEFT VIEW
	cam04[0]  = 0.0;		//rotX;  
	cam04[1]  = 0.0;		//rotY;  
	cam04[2]  = 0.0;		//rotZ;  
	cam04[3]  = min->x - d;	//camX;  
	cam04[4]  = cen->y;		//camY;  
	cam04[5]  = cen->z;		//camZ;  
	cam04[6]  = cen->x;		//tarX;  
	cam04[7]  = cen->y;		//tarY;  
	cam04[8]  = cen->z;		//tarZ;  
	cam04[9]  = 0.0;		//rolX;  
	cam04[10] = 0.0;		//rolY;  
	cam04[11] = 1.0;		//rolZ;  
	cam04[12] = fovA;		//fovA;  
	cam04[13] = zoom;		//zoom;  

	//BOTTOM VIEW
	cam05[0]  = 0.0;		//rotX;  
	cam05[1]  = 0.0;		//rotY;  
	cam05[2]  = 0.0;		//rotZ;  
	cam05[3]  = cen->x;		//camX;  
	cam05[4]  = cen->y;		//camY;  
	cam05[5]  =	min->z - d;	//camZ;  
	cam05[6]  = cen->x;		//tarX;  
	cam05[7]  = cen->y;		//tarY;  
	cam05[8]  = cen->z;		//tarZ;  
	cam05[9]  = 0.0;		//rolX;  
	cam05[10] =-1.0;		//rolY;  
	cam05[11] = 0.0;		//rolZ;  
	cam05[12] = fovA;		//fovA;  
	cam05[13] = zoom;		//zoom;  

	///
	shader = ntShader("shader1.vert", "shader1.frag"); //ORIGINAL FUNCTION CALL LOCATION

	///////////////////////////////////////////////////////////////
	/////////////////////// START STANDARD TRANSFOMMATION MATRICES:
	/////////////////////////////// MODELVIEW | PROJECTION | NORMAL
	M = glm::mat4(1.0f); // set to identity
	V = glm::lookAt(glm::vec3(0.0, 0.0, 10.0f), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	MV = V * M;
	N = glm::transpose(glm::inverse(glm::mat3(MV)));
	///////////////////////////////////////////////////////////////
	////////////////////////////// PROJECTION MATRIX AND MVP MATRIX
	float viewAngle = 65.0f;
	float aspect = width / height;
	float nearDist = 1.0f;
	float farDist = 1000.f;

	P = glm::perspective(viewAngle, aspect, nearDist, farDist);
	MVP = P * MV;
	///////////////////////////// END MODEL | VIEW |PROJECTION DATA

	///////////////////////////////////////////////////////////////
	////////////////////////////////// CALL TRANSFORMATION MATRICES
	T = glm::mat4(1.0f);
	R = glm::mat4(1.0f);
	S = glm::mat4(1.0f);
	
	///////////////////////////////////////////////////////////////
	////////////////////// INITIALIZE UNIFORM VARIABLES FOR SHADERS
	initUniforms();
	///////////////////////////////////////////////////////////////
	////////////////////////////////////// GET INFO ABOUT GLSL SATE
	/// GLSLInfo(&shader); //ORIGINAL FUNCTION CALL LOCATION
	///////////////////////////////////////////////////////////////
	/////////////////////////////////////// PERMANENTLY BIND SHADER
	shader.bind();
}
void genApp::initUniforms(){
	///////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////// LIGHTING
	lightPos0_U = glGetUniformLocation(shader.getID(), "light0Position");
	lightDiffuse0_U = glGetUniformLocation(shader.getID(), "light0Diffuse");
	lightAmbient0_U = glGetUniformLocation(shader.getID(), "light0Ambient");
	lightSpecular0_U = glGetUniformLocation(shader.getID(), "light0Specular");

	///////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////// MATRICES
	MV_U = glGetUniformLocation(shader.getID(), "modelViewMatrix");
	MVP_U = glGetUniformLocation(shader.getID(), "modelViewProjectionMatrix");
	N_U = glGetUniformLocation(shader.getID(), "normalMatrix");
}
void genApp::run(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glViewport(0, 0, width, height);
	
	///////////////////////////////////////////////////////////////
	//////////////////////////////////////// UPDATE SHADER UNIFORMS
	////////////////////////////////////////////////////// LIGHTING
	glUniform3fv(lightPos0_U, 1, &light0.getPos().x);
	glUniform4fv(lightDiffuse0_U, 1, &light0.getDiffuse().r);
	glUniform4fv(lightAmbient0_U, 1, &light0.getAmbient().r);
	glUniform4fv(lightSpecular0_U, 1, &light0.getSpecular().r);

	///
	display();
	if (isGridActive == true) {
		content->display_Grid();
		content->display_BBox();
	}
	content->run();
	///
}
void genApp::display(){
	///////////////////////////////////////////////////////////////
	//////////////////////////////////////////////// RESET MATRICES
	R = glm::mat4(1.0);
	T = glm::mat4(1.0);
	S = glm::mat4(1.0f);

	T = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -8));
	R = glm::rotate(glm::mat4(1.0f), float(frameCount*.06), glm::vec3(.75f, 1, .25f));

	V = glm::lookAt(glm::vec3(0.0, 0.0, 8.0f), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	///////////////////////////////////////////////////////////////
	//////////////////////////////// UPDATE MVP MATRICES FOR SHADER
	M = T * R * S;
	MV = V * M;
	MVP = P * MV;
	///////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////footnote *01
	////////////////////////////////// UPDATE SHADER UNIVERSAL VARS
	N = glm::transpose(glm::inverse(glm::mat3(MV)));
	glUniformMatrix4fv(MV_U, 1, GL_FALSE, &MV[0][0]);
	glUniformMatrix4fv(MVP_U, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix3fv(N_U, 1, GL_FALSE, &N[0][0]);
	///////////////////////////////////////////////////////////////
}
///////////////////////////////////////////////////footnote *01
//reference: http://www.opengl.org/discussion_boards/showthread.php/171184-GLM-to-create-gl_NormalMatrix

