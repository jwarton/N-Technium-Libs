#include "krl_scanLines.h"

void krlSLines::init() {
	///////////////////////////////////////////////////////////////
	//////////////////////////////////////////// DEFINE KRL PROGRAM
	//program01 = KRLprog("SL_001");
	program01 = KRLprog("ZZ_000");

	////SUBSTRATE DIMENSIONS
	float sub_X = 112;	//plate x dimension
	float sub_Y = 120;	//plate y dimension
	float sub_Z = 2;	//plate z dimension (thickness)
	float margin = 10;
	float z_step = .25;	//build layer increment

	///////////////////////////////////////////////////////////////
	////////////////////////////////////////////// DEFINE GEOMETERY
	program01.setTool(6);
	program01.setBase(1);
	program01.setSubstrate(sub_Z);		//thickness in mm

	program01.setBasePos(Vec3(1120, 645, 972 + program01.substrate));
	program01.setApproximation(" C_DIS");//replace with enum for C_VEL & C_ORI

	program01.addSetting("MOTION CONSTANTS");
	program01.addDeclartion("");

	program01.setVel_trvl("0.150");		//define initial scan speed
	program01.setVel_scan("0.025");		//define initial scan speed
	program01.laserPow("0.000");		//define initial laser power
	program01.laserDelay("0.000");		//define initial laser delay 
	program01.powderVel("0.200");		//define initial powder flow

	program01.addInitialization();
	///DEFAULT HOME POSITION INACTIVE DEFAULT
	program01.setStart(-30, -90, 90, 0, 45, -50);
	program01.setEnd(-30, -90, 90, 0, 45, -50);

	///FIRST POSITION
	Vec3 startPos(0, 0, 0);
	program01.PTP(startPos);

	float x = startPos.x + margin;
	float y = startPos.y;
	float z = startPos.z;
	
	int rows = 2;		//number of scan lines
	int layers = 5;		//number of build layers

	///MATRIX OF TRAVEL SPEEDS
	for (int i = 0; i <= rows; i++){

		x = (((sub_X - (2 * margin)) / rows) * i) + margin;
		z = 0;
		program01.command("SCAN_SPEED =" + std::to_string(.04 - (.005 * i)) + "\n");  //increment speed

		for (int j = 0; j < layers; j++){

			program01.LIN(Vec3(x, y-25, z));
			program01.leadIn();
			program01.command("$VEL.CP = SCAN_SPEED");
			program01.delay("BEAM_DELAY");
			program01.LIN(Vec3(x, y + margin, z));
			program01.laserOn();
			program01.LIN(Vec3(x, y + (sub_Y - margin), z));
			program01.laserOff();
			program01.leadOut();
			program01.LIN(Vec3(x, y + sub_Y, z));
			program01.command("$VEL.CP = TRVL_SPEED");
			program01.comment(" LINE " + std::to_string(j) + " OF " + std::to_string(layers) + "\n");

			//Z STEP TO NEXT LAYER
			z += z_step;
		}

		///ADVANCE TO NEXT LINE
		program01.comment("NEXT LINE\n\n");
		program01.delay("3");
	}
	program01.laserOff();

	program01.addFunction("FUNCTIONS");
	program01.addSetting("MOTION CONSTANTS");
	program01.addVariables("VARIABLES");
	program01.write();
}

void krlSLines::run(){
	display();
}

void krlSLines::display(){
	///////////////////////////////////////////////////////////////
	///ADD POINTS ON PATH COLOR CODED TO OPERATION
	program01.display();
	///////////////////////////////////////////////////////////////
}