#include "krl_rhinoTemp.h"

void krlRhino::init() {
	///////////////////////////////////////////////////////////////////
	////////////////////////////////////////////// DEFINE PARSING RANGE
	int fileIndS = 0;  //index of first file to parse
	int fileIndE = 0;  //index of last  file to parse

	for (int i = fileIndS; i <= fileIndE; i++){
		///////////////////////////////////////////////////////////////
		//////////////////////////////////////////// DEFINE KRL PROGRAM
		string fileName = "TC_000";
		string fileRead = fileName + "_Rhino";
		int subNum = 0;			//KRL SUBPROGRAM INDEX NUMBER
		int subMax = 20;		//MAXIMUM SUBPROGRAM CAPACITY OF CONTROLLER
		int maxLines = 15000;	//MAXIMUM LINES PER SUBPROGRAM  //KRC2= 15000

		program01 = KRLprog(fileName);
	
		////BASE COORDINATE OFFSET [mm]
		float bas_X = 1120;									//work plane x transform
		float bas_Y = 645;									//work plane y transform
		float bas_Z = 967;									//work plane z transform

		////SUBSTRATE DIMENSIONS  [mm]
		float sub_X = 125;									//plate x dimension
		float sub_Y = 125;									//plate y dimension
		float sub_Z = 4;									//plate z dimension (plate thickness)
		float margin = 0;
		float z_step = 0.25;								//build layer increment

		////WORKPLANE OFFSET DIMENSIONS  [mm]
		float off_X = -61.5;								//work plane x transform
		float off_Y = 115.5;								//work plane y transform
		float off_Z = 130.5;								//work plane z transform
		
		off_Y -= sub_Y;
		off_Z += sub_Z;

		///////////////////////////////////////////////////////////////
		//////////////////////////////////// DEFINE WORKCELL PARAMETERS
		program01.setTool(1); 
		program01.setBase(1);
		program01.setSubstrate(sub_Z);						//thickness in mm
		program01.setBasePos(Vec3(bas_X, bas_Y, bas_Z));	//store base coordinate
		program01.setExtOffs(Vec3(off_X, off_Y, off_Z));
		program01.setApproximation(" C_DIS");				///replace with enum for C_VEL & C_ORI

		program01.setVel_trvl("0.100");						//define initial scan speed
		program01.setVel_scan("0.020");						//define initial scan speed
		program01.laserPow(   "0.000");						//define initial laser power
		program01.laserDelay( "3.000");						//define initial laser delay 
		program01.powderVel(  "0.150");						//define initial powder flow
		program01.hatchOffset("0.500");						//offest from perimeter path based on bead width

		///DEFAULT HOME POSITION INACTIVE DEFAULT
		program01.setStart(-30, -90, 90, 0, 45, -50);
		program01.setEnd(  -30, -90, 90, 0, 45, -50);

		///FIRST POSITION
		Vec3 startPos(0, 0, 300);
		Vec3 startOri(-135.0, 0.0, 180.0);
		Vec3 tool1Ori( 135.00, 0, 180.0);

		program01.PTP(startPos, startOri);
		program01.command("BASE_EXT()");					//EXTERNAL AXIS COORDINATION ENABLED

		///RHINO SUBPROGRAM
		program01.command("");
		program01.LIN(Vec3(sub_X, sub_Y, sub_Z), tool1Ori);
		program01.LIN(Vec3(0, sub_Y, sub_Z), tool1Ori);
		program01.command("");
		program01.comment("//////////////////////////////////////////// INSERT PROGRAM INSTRUCTIONS HERE\n");

		program01.addSetting("MOTION CONSTANTS");
		program01.addDeclartion("");
		program01.addInitialization();
		program01.addFunction("");

		program01.leadIn();
		program01.command("$VEL.CP = SCAN_SPEED");
		program01.delay("BEAM_DELAY");

		///READ SRC INSTRUCTION CODE FROM GRASSHOPPER TEXT OUTPUT
		string path = nt_Utility::getPathToOutput() + "src\\" + fileRead + ".src";
		int lineCnt = 1;

		ifstream file(path);
		string lines;
		string line;
		bool maxLinesExceed = false;
		bool isStartFile = false;
		bool isEndSubs = false;

		while (std::getline(file, line) && isEndSubs == false){
			///bool doNext = false;
			bool isSubNext = true;			///temporary based on spiral programs
			bool isEndFile = false;
			bool isLinePTP = false;			//if true, disregard instruction line

			if (line.find(";BEGIN RHINO") != string::npos){
				isStartFile = true;
			}
			if (line.find("PTP") != string::npos){
				isLinePTP = true;
			}
			if (isStartFile == true && isLinePTP == false){
				lines = lines + line + "\n";
				lineCnt += 1;
			}

			///ADD CONDITION TO BREAK SUBPROGRAM AT ASSIGNED BLOCK DELIMITER
			if (lineCnt > maxLines & line.find("NEXT") != string::npos){
				isSubNext = true;
			}
			if (line.find(";END RHINO") != string::npos){
				isEndFile = true;
			}
			if (subNum >= subMax){
				isEndSubs = true;
			}

			if (lineCnt > maxLines & isSubNext == true || isEndFile == true){
				maxLinesExceed = true;
				///doNext = false;	///temporary based on spiral programs

				string subfileName = fileName + "_" + std::to_string(subNum);
				KRLprog subProgram = KRLprog(subfileName);
				program01.isMastProg = true;
				subProgram.isSubProg = true;
				subProgram.command(lines);
				subProgram.command("END");
				subProgram.setDeclarations(program01.getDeclarations());
				subProgram.setInitializations(program01.getInitializations());
				subProgram.setFunctions(program01.getFunctions());
				subProgram.write();

				program01.addExternal("EXT  "+ subfileName +"()" );
				program01.command(subfileName + "()");
			
				subNum += 1;
				lineCnt = 1;
				lines = "";
			}
		}

		if (maxLinesExceed == false){
			program01.command(lines);
		}
		///
		program01.command("");
		program01.laserOff();
		program01.powderOff();
		program01.leadOut();
		program01.command("$VEL.CP = TRVL_SPEED");
		program01.command("WAIT SEC 3");
		program01.command("BASE()");		///EXTERNAL AXIS COORDINATION DISABLED

		///ADD RETRUN TO END POSITION

		program01.addVariables("");
		program01.write();
		}
}

void krlRhino::run(){
	display();
}

void krlRhino::display(){
	///////////////////////////////////////////////////////////////
	///ADD POINTS ON PATH COLOR CODED TO OPERATION
	program01.display();
	///////////////////////////////////////////////////////////////
}