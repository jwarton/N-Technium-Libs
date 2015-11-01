///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////// nt_Main.h
// openGl scratch libs							///////////////////
// Main Execute Progam FILE						///////////////////
// created by James Warton on 04/01/2014		///////////////////
///////////////////////////////////////////////////////////////////

#include "nt_GLFWsetup.h"
#include "nt_genApp.h"
#include "nt_simpleApp.h"
//
#include "nt_00_meshApp.h"
#include "nt_01_meshApp.h"
#include "nt_02_treeApp-v01.h"
#include "nt_02_treeApp-v02.h"
#include "nt_02_treeApp-v03.h"
//#include "nt_03_swarmApp.h"
#include "nt_04_emitterApp.h"
#include "nt_05_shapeApp.h"
#include "nt_05_cubeApp.h"
#include "nt_07_trussApp-v01.h"
#include "nt_07_trussApp-v02.h"
#include "nt_07_trussApp-v03.h"
//#include "nt_07_beamApp-v01.h"

//kuka source code applications
#include "nt_06_krlApp.h"
#include "krl_focalPlane.h"
#include "krl_scanLines.h"
#include "krl_xaxisDemo.h"
#include "krl_rhinoTemp.h"
#include "krl_textTemp.h"

////image processing prototypes
#include "nt_08_afireApp-v01.h"
#include "nt_08_afireApp-v02.h"

////professional projects
#include "nt_09_ovisApp.h"
//
#include <iostream>
#include <array>
#include <arrayfire.h>

int main(int argc, char const** argv){
	////////////////////////////////////////////////////////////////// SELECT DEVICE AND DISPLAY ARRAYFIRE INFO
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	printf("\n");
	int device = argc > 1 ? atoi(argv[1]) : 0;
    af::setDevice(device);
    af::info();
	printf("///////////////////////////////////////////////////////////////\n\n");

	/////////////////////////////////////////////////////////////////////////// INSTANTIATE DERIVED APPLICATION
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	///GRAPHICS
	//AppContent* c = new meshImportApp();
	//AppContent* c = new meshApp();
	AppContent* c = new treeApp01();
	//AppContent* c = new treeApp02();
	//AppContent* c = new treeApp03();
	//AppContent* c = new swarmApp();
	//AppContent* c = new emitterApp(10000,.1);
	//AppContent* c = new shapeApp();
	//AppContent* c = new cubeApp();

	///FEM
	//AppContent* c = new trussApp01();		//VALIDATION 3D TRUSS ELEMENTS
	//AppContent* c = new trussApp02();		//SPACE FRAME CELL
	//AppContent* c = new trussApp03();		//SPACE FRAME FROM COMPOUND CURVED SURFACE
	//AppContent* c = new beamApp01();		//VALIDATION FOR 3D BEAM ELEMENTS
	///IMAGE PROCESSING
	//AppContent* c = new afApp01();		//ARRAY FIRE IMPELEMENTATION PROTOTYPE
	//AppContent* c = new afApp02();		

	///OVIS
	//AppContent* c = new ovisApp();

	///KUKA SOURCE CODE APPLICATIONS
	//AppContent* c = new krlApp();
	//AppContent* c = new krlFPlane;
	//AppContent* c = new krlSLines;
	//AppContent* c = new krlXAxis;
	//AppContent* c = new krlRhino;
	//AppContent* c = new krlMatLab;

	//jpw::ntGLFWsetup program(100, 100, "NT-INTERFACE", new genApp(c));
	jpw::ntGLFWsetup program(1900,1000, "NT-INTERFACE", new genApp(c)); 
	//simple app minimal ogl setup/ test
	//jpw::ntSimpleApp simpleApp = ntSimpleApp(900, 900,"SIMPLE APP_TITLE");

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	return EXIT_SUCCESS;
}

///FUNCTIONALITY TODO LIST
//EXTEND BASE CLASS WITH GEOM SHADER FUNCTIONALITY
//RESOLVE TIME STEP IN RK4 INTEGRATION METHOD
//OPERATOR OVERLOADS IN VECTOR CLASS
//STRUCTURED MESH INHERITANCE
//RESOLVE SWARM BEHAVIOR 

///ADD TO LIBRARY
//BASIC ROBOTIC PATH GENERATOR
//NODE DEFINITION FROM VB
//RAY TRACING RENDER MODE
//MIGRATE LATTICE FROM ROBARCH WORKSHOP SCRIPTS

///RESEARCH TOPICS
//COMPUTE SHADER CLASS
//EXAMINE DISTANCE CHECKING METHODS
//SUBDIVISION CATMULL CLARK AND MODIFIED CC