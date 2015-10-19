#include "nt_06_krlApp.h"

void krlApp::init() {
	///////////////////////////////////////////////////////////////
	//////////////////////////////////////////// DEFINE KRL PROGRAM
	program01 = KRLprog("CKRL01");

	///////////////////////////////////////////////////////////////
	////////////////////////////////////////////// DEFINE GEOMETERY
	int n = 10;
	points.reserve(n);

	///
	//for (int i = 0; i < n; i++){
	//	Vec3* pt = new Vec3(i, i, i);
	//	points.push_back(pt);
	//}
	///////////////////////////////////////////////////////////////
	////////////////////////////////// PASS GEOMETRY TO KRL PROGRAM
	program01.setTool(2);
	program01.setBase(1);
	program01.setBasePos(Vec3(0, 0, 0));///program01.setBase(Vec3(1250, 0, 1250));
	//program01.setHome(Vec3(0, 0, 0));
	//program01.setEnd(Vec3(0, 0, 0));
	program01.setVel(std::to_string(0.05));
	program01.laserPow(std::to_string(0.0));
	program01.powderVel(std::to_string(0.15));

	///FIRST POSITION
	Vec3 startPos(1120,645,942);

	program01.PTP(startPos);

	float x = startPos.x;
	float y = startPos.y;
	float z = startPos.z;
	
	for (int i = 0; i < 10; i ++){
		y = startPos.y;
		for (int j = 0; j < 10; j++){
			program01.LIN(Vec3(x, y, z));
			program01.laserOn();
			program01.delay("BEAM_DELAY");
			program01.laserOff();

			y += 5;
			z += .25;
			//float voltage = i * .05;
			//program01.powderVel(std::to_string(voltage));
			//program01.powderOn();
		}
		x += 5;
	}
	//program01.def_PATH(points);
	
	program01.laserOff();
	//program01.powderOff();

	program01.addFunction("FUNCTIONS");
	program01.addSetting("MOTION CONSTANTS");
	program01.addVariables("myVariable");
	program01.write();
}

void krlApp::run(){
	//display();
}

void krlApp::display(){
	///////////////////////////////////////////////////////////////
	///ADD POINTS ON PATH COLOR CODED TO OPERATION
	///////////////////////////////////////////////////////////////
}