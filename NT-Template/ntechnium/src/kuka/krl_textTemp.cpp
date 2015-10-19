#include "krl_textTemp.h"

void krlMatLab::init() {
	///////////////////////////////////////////////////////////////////
	////////////////////////////////////////////// DEFINE PARSING RANGE
	int fileIndS = 0;  //index of first file to parse
	int fileIndE = 0;  //index of last  file to parse

	for (int i = fileIndS; i <= fileIndE; i++){
		///////////////////////////////////////////////////////////////
		//////////////////////////////////////////// DEFINE KRL PROGRAM
		string fileName = "ML_000";
		string fileRead = fileName + "_MatLab";
		int subNum = 0;										//KRL SUBPROGRAM INDEX NUMBER
		int subMax = 20;									//MAXIMUM SUBPROGRAM CAPACITY OF CONTROLLER
		int maxLines = 15000;								//MAXIMUM LINES PER SUBPROGRAM  //KRC2= 15000

		program01 = KRLprog(fileName);

		////BASE COORDINATE OFFSET [mm]
		float bas_X = 1120;									//work plane x transform
		float bas_Y = 645;									//work plane y transform
		float bas_Z = 967;									//work plane z transform
	
		////SUBSTRATE DIMENSIONS  [mm]
		float sub_X  = 125;									//plate x dimension
		float sub_Y  = 125;									//plate y dimension
		float sub_Z  = 4;									//plate z dimension (plate thickness)
		float margin = 0;
		float z_step = 0.25;								//build layer increment

		////WORKPLANE OFFSET DIMENSIONS  [mm]
		float off_X = -61.5;						//work plane x transform
		float off_Y = 115.5;						//work plane y transform
		float off_Z = 130.5;						//work plane z transform
		off_Z += sub_Z;

		///////////////////////////////////////////////////////////////
		//////////////////////////////////// DEFINE WORKCELL PARAMETERS
		program01.setTool(1); 
		program01.setBase(1);
		program01.setSubstrate(sub_Z);						//thickness in mm
		program01.setBasePos(Vec3(bas_X, bas_Y, bas_Z));	//store base coordinate
		program01.setExtOffs(Vec3(off_X, off_Y, off_Z));
		program01.setApproximation(" C_DIS");				//replace with enum for C_VEL & C_ORI
		
		program01.setVel_trvl("0.100");						//define initial scan speed
		program01.setVel_scan("0.020");						//define initial scan speed
		program01.laserPow(   "0.000");						//define initial laser power
		program01.laserDelay( "3.000");						//define initial laser delay 
		program01.powderVel(  "0.200");						//define initial powder flow
		program01.hatchOffset("0.500");						//offset from perimeter path based on bead width

		///DEFAULT HOME POSITION INACTIVE DEFAULT
		program01.setStart(-30, -90, 90, 0, 45, -50);
		program01.setEnd(-30, -90, 90, 0, 45, -50);

		///FIRST POSITION
		Vec3 startPos(0, 0, 0);
		Vec3 startOri(-135.0, 0.0, 180.0);
		Vec3 tool1Ori( 135.00, 0, 180.0);

		program01.PTP(startPos, startOri);
		program01.command("BASE_EXT()");					//EXTERNAL AXIS COORDINATION ENABLED

		///RHINO SUBPROGRAM
		program01.command("");
		program01.LIN(Vec3(0, 0, 0), tool1Ori);
		program01.command("");
		program01.comment("//////////////////////////////////////////// INSERT PROGRAM INSTRUCTIONS HERE\n");

		program01.addSetting("MOTION CONSTANTS");
		program01.addDeclartion("");
		program01.addInitialization();
		program01.addFunction("");

		program01.leadIn();
		program01.command("$VEL.CP = SCAN_SPEED");
		program01.delay("BEAM_DELAY");

		///READ INSTRUCTION SEQUENCE FROM TEXT FILE
		string path = nt_Utility::getPathToOutput() + "src\\" + fileRead + ".txt";
		int lineCnt = 1;

		ifstream file(path);
		string lines;
		string line;
		bool maxLinesExceed = false;
		bool isStartFile = false;
		bool isEndSubs = false;

		while (std::getline(file, line) && isEndSubs == false){
			bool isEndFile = false;

			if (line.find(";BEGIN") != string::npos){
				isStartFile = true;
			}

			if (isStartFile == true){
				///CALL FUNCTION TO FORMAT COORDINATE
				if (line.find(";") != string::npos){

				}
				if (line.find("LIN") != string::npos || line.find("PTP") != string::npos){
					line = format(line);
				}
				lines = lines + line + "\n";
				lineCnt += 1;
			}

			///ADD CONDITION TO BREAK SUBPROGRAM AT ASSIGNED BLOCK DELIMITER
			if (line.find(";END") != string::npos){
				isEndFile = true;
			}
			if (subNum >= subMax){
				isEndSubs = true;
			}

			//if (lineCnt > maxLines & isSubNext == true || isEndFile == true){
			if (lineCnt > maxLines || isEndFile == true){
				maxLinesExceed = true;

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
		/*
		int n = program01.verts.size();
		for (int i = 0; i < n; i++){
			ntMatrix4 mat = ntMatrix4(program01.verts.at(i).pos);
			///ADD SCALE FACTOR BASED ON DIMS
			//mat.scale3d(.25);
		}
		*/
}

void krlMatLab::run(){
	display();
}

void krlMatLab::display(){
	///////////////////////////////////////////////////////////////
	///ADD POINTS ON PATH COLOR CODED TO OPERATION
	//program01.display();

	for (int i = 0; i < verts.size(); ++i){
		verts.at(i)->display(2);
		//std::cout << "     " << verts.at(i)->pos->x << endl;
	}
	///////////////////////////////////////////////////////////////
}

string krlMatLab::format(string line){
	string delim = ", ";
	size_t subSt = 0;
	size_t found = line.find_first_of(delim);
	vector < std::string > command;

	while (found != string::npos)
	{
		if (found > subSt)
			command.push_back(line.substr(subSt, found - subSt));

		subSt = ++found;
		found = line.find_first_of(delim, found);
	}

	if (subSt < line.size())
		command.push_back(line.substr(subSt));

	if (line.find("LIN") != string::npos || line.find("PTP") != string::npos){
		Vec3 pos = Vec3(std::stof(command[1]), std::stof(command[2]), std::stof(command[3]));
		Vec3 ori = Vec3(std::stof(command[4]), std::stof(command[5]), std::stof(command[6]));

		addPt(&pos);

		float e1 = std::stof(command[7]);
		float e2 = std::stof(command[8]);
		float e3 = std::stof(command[9]);
		float e4 = std::stof(command[10]);
		float e5 = std::stof(command[11]);
		float e6 = std::stof(command[12]);

		//DUPLICATE COMMAND IN MASTER PROGRAM
		/*
		if(command[0] == "LIN") {
			program01.LIN(pos, ori, e1, e2, e3, e4, e5, e6);
		}
		if (command[0] == "PTP") {
			program01.PTP(pos, ori, e1, e2, e3, e4, e5, e6);
		}
		*/
	}

	line = command[0] + " { E6POS" +
		": X " +  command[1] +
		", Y " +  command[2] +
		", Z " +  command[3] +
		", A " +  command[4] +
		", B " +  command[5] +
		", C " +  command[6] +
		", E1 " + command[7] +
		", E2 " + command[8] +
		", E3 " + command[9] +
		", E4 " + command[10] +
		", E5 " + command[11] +
		", E6 " + command[12] +
		" }  C_DIS";

	return line;
}

void krlMatLab::addPt(Vec3 * pos){
	vecs.push_back(pos);
	verts.push_back(new ntVertex(vecs.back(), 2.0, ntCol4(.5, 0, 1)));

	std::cout << vecs.size() << endl;
	std::cout << verts.size() << endl;
	std::cout << "     " << verts.back()->pos->x << endl;
}