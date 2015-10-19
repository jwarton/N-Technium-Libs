#include "krl_focalPlane.h"

void krlFPlane::init() {
	///////////////////////////////////////////////////////////////
	//////////////////////////////////////////// DEFINE KRL PROGRAM
	program01 = KRLprog("FP_09-11");

	///////////////////////////////////////////////////////////////
	////////////////////////////////////////////// DEFINE GEOMETERY
	program01.setTool(2);
	program01.setBase(1);
	program01.setSubstrate(6); //thickness in mm

	program01.setBasePos(Vec3(1120, 645, 970 + program01.substrate));

	program01.setVel("0.05");
	program01.laserPow("0.0");
	program01.powderVel("0.15");

	///DEFAULT HOME POSITION INACTIVE DEFAULT
	program01.setStart(-30, -90, 90, 0, 45, -50);
	program01.setEnd(-30, -90, 90, 0, 45, -50);

	///FIRST POSITION
	Vec3 startPos(0,0,-1);
	program01.PTP(startPos);

	float x = startPos.x;
	float y = startPos.y;
	float z = startPos.z;
	
	///MATRIX OF DISTANCES BETWEEN FOCAL PLANE AND SUBSTRATE
	for (int i = 0; i < 2; i ++){
		y = startPos.y;
		for (int j = 0; j < 4; j++){
			program01.LIN(Vec3(x, y, z));
			program01.laserOn();
			program01.delay("BEAM_DELAY");
			program01.laserOff();

			program01.delay("5");
			//SHIFT TO NEXT POSITION
			y += 5;
			z += .25;
		}
		x += 5;
	}
	program01.laserOff();

	program01.addFunction("FUNCTIONS");
	program01.addSetting("MOTION CONSTANTS");
	program01.addVariables("VARIABLES");
	program01.write();
}

void krlFPlane::run(){
	//display();
}

void krlFPlane::display(){
	///////////////////////////////////////////////////////////////
	///ADD POINTS ON PATH COLOR CODED TO OPERATION
	///////////////////////////////////////////////////////////////
}