///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////// nt_SRCwriter.h
// KUKA Robot Language Generator				///////////////////
// SRC File Writer								///////////////////
// created by James Warton on 05/23/2014		///////////////////
///////////////////////////////////////////////////////////////////
#ifndef SRC_CODE_GENERATOR_JPW_NTECHNIUM
#define SRC_CODE_GENERATOR_JPW_NTECHNIUM

#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <time.h>
#include <vector>

#include "nt_KRLfunctions.h"
#include "nt_Utility.h"
#include "ntVec3.h"

using namespace std;
using namespace jpw;

class SRCwriter;
typedef SRCwriter SRCgen;

class SRCwriter{
private:
	///////////////////////////////////////////////////////////////
	///////////////////////////////////// write .src specifications
	string url;
	string path;
	string pathExtension;
	string fileName;
	string fileExt = ".src";

	struct tm timeData;
	time_t timeStamp;

	///////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////SRC components
	string header;	//define KRL variables and setup
	string body;	//path and operation run sequence
	string footer;	//define KRL functions0

	///////////////////////////////////////////////////////////////
	///////////////////////////////////////////////RHINO components
	vector<Vec3*> positions; ///move to simulation generator class?

public:

	string settings;	//define motion parameters
	string variables;	//define program constants
	string functions;	//declare program functions

	string instructions;
	string declarations;
	string initializations;
	string endPos;
	string startPos;

	SRCwriter();
	SRCwriter(string fileName);
	SRCwriter(string path, string fileName);

	void init();
	void writeFile();

	void writeHeader();
	void writeBody();
	void writeFooter();

	void setProgram();
};
#endif

///THESE COMPONENTS ARE MANAGED ON A PER PROJECT BASIS
///////////////////////////////////////////////////////////////
//////////////////////////////////////// fabrication components
///positions class
//formats vec3 to tcp positions
//derived position maintains compostion of powder ratios to be deposited
///motion path class (generic path could accept vec3 or position objects)
//generates LIN commands from collection of positions
///operation class | writeFunction
//defines generic operations to any workcell/program
//lead in | lead out ...
//defines operations specific to workcell
///program class
//manages collection of paths and operations
///end effector class
//defines operations specific to end effector
///work cell specification class
//manages robot and i/o specifications
