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
	content->grid();

	ntVec3* min = new ntVec3();
	ntVec3* max = new ntVec3();
	ntVec3* cen = new ntVec3();
	content->get_Bounds(min, max);
	content->get_Centroid(cen);
	
	fovA = 15;
	tarX = cen->x;
	tarY = cen->y;
	tarZ = cen->z;
	zoom = 100;

	float BS_X = max->x - min->x;
	float BS_Y = max->y - min->y;
	float BS_Z = max->z - min->z;

	float dim_Max = std::max(BS_X, BS_Y);
	dim_Max = std::max(dim_Max, BS_Z);

	float BS_radius = abs((dim_Max) * 0.5);
	float V_magnitude = sin(fovA + zoom) * BS_radius;

	camX = cen->x;
	camY = cen->y -5;
	camZ = cen->z + abs(V_magnitude);

	std::cout << "BOUNDING SPHERE RADIUS:   "<< BS_radius << endl;
	std::cout << "CAMERA | TARGET DISTANCE: "<< V_magnitude << endl;
	std::cout << "CAMERA POSITION:          "<< camX << ", "<< camY << ", "<< camZ << endl;
	
	//fovA = BB_width + (BB_width * 0.25); 
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
	//display();
	content->grid_display();
	content->display_BBox();
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

