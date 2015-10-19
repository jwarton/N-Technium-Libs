#include "krl_xaxisDemo.h"

void krlXAxis::init() {
	///////////////////////////////////////////////////////////////
	//////////////////////////////////////////// DEFINE KRL PROGRAM
	program01 = KRLprog("EA_000");

	////BASE COORDINATE OFFSET [mm]
	float bas_X = 1120;							//work plane x transform
	float bas_Y = 0;							//work plane y transform
	float bas_Z = 50;							//work plane z transform

	////SUBSTRATE DIMENSIONS  [mm]
	float sub_X = 112;							//plate x dimension
	float sub_Y = 120;							//plate y dimension
	float sub_Z = 2;							//plate z dimension (thickness)
	float margin = 0;

	////WORKPLANE OFFSET DIMENSIONS  [mm]
	float off_X = -61.5;						//work plane x transform
	float off_Y = 115.5;						//work plane y transform
	float off_Z = 130.5;						//work plane z transform
	off_Z += sub_Z;

	///////////////////////////////////////////////////////////////
	////////////////////////////////////////////// DEFINE GEOMETERY
	program01.setTool(1);						//select tool stored on KRC-4
	program01.setBase(1);						//select base stored on KRC-4
	program01.setSubstrate(sub_Z);				//plate thickness [mm]

	program01.setBasePos(Vec3(bas_X, bas_Y, bas_Z));	//store base coordinate
	program01.setExtOffs(Vec3(off_X, off_Y, off_Z));
	program01.setApproximation(" C_DIS");		//replace with enum for C_VEL & C_ORI

	program01.addSetting("MOTION CONSTANTS");
	program01.addDeclartion("");

	///INITIALIZE USER DEFINED VARIABLES
	program01.setVel_trvl("0.250");				//define initial scan speed
	program01.setVel_scan("0.025");				//define initial scan speed
	program01.laserPow(   "0.000");				//define initial laser power
	program01.laserDelay( "0.000");				//define initial laser delay 
	program01.powderVel(  "0.200");				//define initial powder flow

	program01.addInitialization();
	///DEFAULT HOME POSITION INACTIVE DEFAULT
	//program01.setHome(Vec3(0, 0, 0));
	program01.setStart(-30, -90, 90, 0, 45, -50);
	program01.setEnd(  -30, -90, 90, 0, 45, -50);

	///POSITIONING VARIALBLES
	Vec3 PTP_pos(0, 0, 500);					//initial position
	Vec3 PTP_ori(0.0, 0, 180.0);				//initial orientation
	Vec3 LIN_pos(0, 0, 0);						//tool position at firsts LIN motion
	Vec3 LIN_ori(-135.0, 0, 180.0);				//tool orientation

	float x = LIN_pos.x;
	float y = LIN_pos.y;
	float z = LIN_pos.z;
	float e1 = 0;
	float e2 = 0;

	program01.command("$VEL.CP = TRVL_SPEED");

	program01.PTP(PTP_pos, PTP_ori);
	program01.command("BASE_EXT()");			//EXTERNAL AXIS COORDINATION ENABLED
	program01.LIN(Vec3(0, 0, z), LIN_ori);
	
	int rows = 5;								//number of scan lines
	int phi = 5;

	for (int i = 0; i <= rows; i++){
	
		e1 = -2 * i;	//tilt axis
		e2 = phi * i;	//turn axis

		program01.LIN(Vec3(x, y, z), LIN_ori, e1, -e2);
		program01.LIN(Vec3(x, y, z), LIN_ori);
		program01.LIN(Vec3(x, y, z), LIN_ori,  3,  e2);
	}

	program01.command("BASE()");				//EXTERNAL AXIS COORDINATION DISABLED

	program01.addFunction("FUNCTIONS");
	program01.addSetting("MOTION CONSTANTS");
	program01.addVariables("VARIABLES");
	program01.write();
}

void krlXAxis::run(){
	display();
}

void krlXAxis::display(){
	///////////////////////////////////////////////////////////////
	///ADD POINTS ON PATH COLOR CODED TO OPERATION
	///////////////////////////////////////////////////////////////
}