///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////// nt_Main.h
// openGl scratch libs							///////////////////
// Main Execute Progam FILE						///////////////////
// created by James Warton on 04/01/2014		///////////////////
///////////////////////////////////////////////////////////////////

#pragma optimize( "Og", off )

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
#include "nt_05_triApp.h"
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
#include "nt_09_ovisApp-v02.h"
#include "nt_10_typeApp.h"
//

////visualizing tokyo projects
#include "nt_11_c3-App-v00.h"
#include "nt_11_c3-App-v01.h"
#include "nt_11_c3-App-v02.h"

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

	int	x = 1920;
	int y = 1080;

	if (0 > 1) {
		x = 3200;
		y = 1800;
	}


	///GRAPHICS
	//AppContent* c = new meshImportApp();
	//AppContent* c = new meshApp();
	//AppContent* c = new treeApp01();
	//AppContent* c = new treeApp02();
	//AppContent* c = new treeApp03();
	//AppContent* c = new swarmApp();
	//AppContent* c = new emitterApp(10000,.1);
	//AppContent* c = new shapeApp();
	//AppContent* c = new cubeApp();
	//AppContent* c = new triApp();

	///FEM
	//AppContent* c = new trussApp01();		//VALIDATION 3D TRUSS ELEMENTS
	//AppContent* c = new trussApp02();		//SPACE FRAME CELL
	//AppContent* c = new trussApp03();		//SPACE FRAME FROM COMPOUND CURVED SURFACE
	//AppContent* c = new beamApp01();		//VALIDATION FOR 3D BEAM ELEMENTS

	///IMAGE PROCESSING
	//AppContent* c = new afApp01();		//ARRAY FIRE IMPELEMENTATION PROTOTYPE TEST
	//AppContent* c = new afApp02();		
	//AppContent* c = new tokyoApp00();		//C3 VISUALIZING TOKYO VIGNETTES
	//AppContent* c = new tokyoApp01();		//C3 PROFILE SUBDIVISION
	AppContent* c = new tokyoApp02();		//C3 ORIGAMI STRUCTURE | IMAGE MAPPING

	///TYPE
	//AppContent* c = new typeApp();
	///OVIS
	//AppContent* c = new ovisApp();
	//AppContent* c = new ovisApp02();

	///KUKA SOURCE CODE APPLICATIONS
	//AppContent* c = new krlApp();
	//AppContent* c = new krlFPlane;
	//AppContent* c = new krlSLines;
	//AppContent* c = new krlXAxis;
	//AppContent* c = new krlRhino;
	//AppContent* c = new krlMatLab;

	c->set_AppDim(x, y);
	//jpw::ntGLFWsetup program(100, 100, "NT-INTERFACE", new genApp(c));
	jpw::ntGLFWsetup program("NT-INTERFACE", new genApp(c));
	//jpw::ntGLFWsetup program(x,y, "NT-INTERFACE", new genApp(c)); 
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
//SUBDIVISION CATMULL CLARK AND MODIFIED CC

///TRISKIN DEVELOPMENT
// TRICELL IMG WRITER FUNCTIONALITY
// ADD FASTENER POSITIONS TO TXT WRITER
// EXCEPTION HANDLER FOR FASTENER FUNCTION
// READ WEIGHTING VARIABLE FROM PANEL INPUT FILE
// READ REGION VARIABLE FROM PANEL INPUT FILE
// MOSAIC | TRISKIN INTEGRATION
// TREE | TRISKIN INTEGRATION
// REFACTOR PANEL CLASS:  PARITY BETWEEN 2D - 3D INSTANCES
// ADD POLYGON DERIVED FROM POLYLINE SHAPE CLASS
// ADD POLYLINE OFFSET FUNCTION
// ADD POLYLINE JOIN FUNCTION