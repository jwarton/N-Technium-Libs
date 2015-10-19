///////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////// nt_KRLprogram.h
// KUKA Robot Language base program				///////////////////
//												///////////////////
// created by James Warton on 05/28/2014		///////////////////
///////////////////////////////////////////////////////////////////
#ifndef KUKA_KRL_PROGRAM_JPW_NTECHNIUM
#define KUKA_KRL_PROGRAM_JPW_NTECHNIUM

#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <time.h>
#include <vector>

#include "nt_SRCwriter.h"
#include "nt_KRLpath.h"
#include "ntVec3.h"
#include "ntVertex.h"
#include "ntColor4f.h"

using namespace std;
using namespace jpw;

class ntKRLprogram;
typedef ntKRLprogram KRLprog;

class ntKRLprogram{
private:
	///PROGRAM VARIABLES
	std::string programName;
	std::string settings;					//define motion parameters
	std::string variables;					//define program constants
	std::string functions;					//declare program functions

	std::string instructions;				//krl instruction sequence
	std::string externals;
	std::string declarations;				//declare variables
	std::string initializations;			//initialize variables

	///POSITIONING
	int	tool = 0;							//predefined tool parameters
	int base = 0;							//predefined base parameters

	ntVec3 basePos = ntVec3(0, 0, 0);		//user defined base coordinate
	ntVec3 baseOri = ntVec3(0, 0, 0);
	ntVec3 homePos;							//default home position
	std::string startPos;					//default start position
	std::string endPos;						//default end position

	ntVec3 extOff = ntVec3(0, 0, 0);		//default workplane offset for external positioner
	ntVec3 extOri = ntVec3(0, 0, 0);		//default workplane orientation for external positioner

	///MOTION
	std::string velocity;					//default velocity / max velocity
	std::vector <KRLpath*> paths;			//collection of paths //should lead-in and lead-out be embedded within path?
	std::string approx = "";

	///WORK CELL SPECIFIC VARIABLES
	std::string vel_trvl = "0";
	std::string vel_scan = "0";
	std::string powderFeed = "0";			//default powder flow in volts
	std::string laserPower = "0";			//default laser power in watts
	std::string beamDelay = "0";			//default beam delay in seconds
	std::string offsetDist = "0";			//hatch offest based on bead thickness

	///I/O CONFIGURATION  //KRC-4
	std::string gasCarrier = "7";
	std::string gasShield = "8";
	std::string laserActive = "9";
	std::string powderRed_01 = "1";
	std::string powderRed_02 = "2";
	std::string powderBlu_01 = "3";
	std::string powderBlu_02 = "4";

	///I/O CONFIGURATION  //KRC-2
	//string gasCarrier   = "33";
	//string gasShield    = "36";
	//string laserActive  = "34";
	//string powderRed_01 = "1";

	///DISPLAY 
	std::vector <ntVec3 *> vecs;
	std::vector <ntVertex *> verts;

public:

	bool isSubProg = false;
	bool isMastProg = false;
	bool isHalt = false;
	int substrate = 0;			//substrate thickness

	ntKRLprogram();
	ntKRLprogram(string name);
	void init();
	void write();
	void display();

	void addSetting(string settingName);
	void addDeclartion(string declarationName);
	void addExternal(string programName);
	void addInitialization();
	void addFunction(string functionName);
	void addVariables(string variableNames);

	string getDeclarations();
	string getInitializations();
	string getFunctions();
	void setDeclarations(string declarations);
	void setInitializations(string initializations);
	void setFunctions(string functions);


	///  GENERIC PATH METHODS
	string LIN(Vec3 pos, Vec3 ori = Vec3(0, 0, 0), float ex1 = 0.0, float ex2 = 0.0, float ex3 = 0.0, float ex4 = 0.0, float ex5 = 0.0, float ex6 = 0.0);
	string PTP(Vec3 pos, Vec3 ori = Vec3(0, 0, 0), float ex1 = 0.0, float ex2 = 0.0, float ex3 = 0.0, float ex4 = 0.0, float ex5 = 0.0, float ex6 = 0.0);
	string CIRC(Vec3 auxPt, Vec3 endPt, float theta);

	string def_FCT(string var);
	string def_PATH(vector <Vec3*> pts);

	void setTool(int tool);
	void setBase(int base);
	void setSubstrate(int substrate);

	void setBasePos(Vec3 base);
	void setHome(Vec3 home);
	void setStart(float A1, float A2, float A3, float A4, float A5, float A6, float E1 = 0, float E2 = 0, float E3 = 0, float E4 = 0, float E5 = 0, float E6 = 0);
	void setEnd(float A1, float A2, float A3, float A4, float A5, float A6, float E1 = 0, float E2 = 0, float E3 = 0, float E4 = 0, float E5 = 0, float E6 = 0);
	void setVel (string velocity);
	void setVel_trvl(string velocity);
	void setVel_scan(string velocity);

	void setExtOffs(Vec3 offset);
	void setExtOri(Vec3 orientation);

	void setApproximation(string method);

	void delay(string seconds);
	void command(string command); //accepts explicit krl
	void comment(string comment); //inline comment 

	///  ADD WORK CELL SPECIFIC METHODS BELOW
	void powderOn();
	void powderOff();
	void powderVel(string voltage);

	void laserOn();
	void laserOff();
	void laserPow(string voltage);

	void leadIn();
	void leadOut();

	//   PATH AND FILL SPECIFIC SETTINGS
	void laserDelay(string seconds);
	void hatchOffset(string offsetDist);

	///  DISPLAY FUNCTIONS
	void addVert(Vec3 * pos);
};
#endif

///SET THIS UP AS BASE CLASS WITH ALL GENERIC FUNCTIONS
///ADDITIONAL WORKCELL SPECIFIC FUNCTIONS INHERIT FROM THIS CLASS