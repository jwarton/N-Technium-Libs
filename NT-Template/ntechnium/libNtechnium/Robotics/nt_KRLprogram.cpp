#include "nt_KRLprogram.h"

ntKRLprogram::ntKRLprogram(){
}
ntKRLprogram::ntKRLprogram(string name):
programName(name){
}
void ntKRLprogram::init(){
}

void ntKRLprogram::display(){
	for (int i = 0; i < verts.size(); i++){
		verts.at(i)->display();
		//std::cout << verts.at(i)->pos->x << endl;
		//std::cout << vecs.at(i)->x << endl;
	}
}
void ntKRLprogram::addVert(Vec3 * pos){
	vecs.push_back(pos);
	ntVertex * vert = new ntVertex(vecs.back(), 2.0, ntCol4(1, 0, .5));
	verts.push_back( vert );
	if (verts.size() > 0){
		//std::cout << "     " << vecs.back()->x << endl;
		//std::cout << "     " << verts.back()->pos->x << endl;
	}
}

///WRITE KRL TO FILE
void ntKRLprogram::write(){

	SRCwriter srcFile = SRCwriter(programName);
	
	srcFile.declarations = this->declarations;
	srcFile.variables = this->variables;
	srcFile.initializations = this->initializations;
	srcFile.instructions = this->instructions;				
	srcFile.functions = this->functions;

	if (isSubProg == false){
		srcFile.startPos = this->startPos;
		srcFile.endPos = this->endPos;
		srcFile.settings = this->settings;
	}

	srcFile.writeFile();
}

///ADD ELEMENTS TO PROGRAM
void ntKRLprogram::addSetting(string settingName){
	settings = ";/////////////////// SETTING\n;FOLD SET " + settingName + "\n" +
		"  BASE_DATA[" + std::to_string(base) + "] = { X " + std::to_string(basePos.x) + ", Y " + std::to_string(basePos.y) + ", Z " + std::to_string(basePos.z) + ", A " + std::to_string(baseOri.x) + ", B " + std::to_string(baseOri.y) + ", C " + std::to_string(baseOri.z) + " }\n" +
		"  $VEL.CP   = 0.05\n" +
		"  $VEL.ORI1 = 20\n" +  //"  $VEL.ORI1 = 400\n" +
		"  $VEL.ORI2 = 20\n" +  //"  $VEL.ORI2 = 400\n" +
		"  $ACC.CP   = 10\n" +
		"  $ACC.ORI1 = 1000\n" +
		"  $ACC.ORI2 = 1000\n" +
		"  BAS(#VEL_PTP, 10)\n" +
		"  ;/////////////////// APPROXIMATION\n" +
		"  $ADVANCE  = 1\n" +
		"  $APO.CDIS = 0.25\n" +
		"  $APO.CVEL = 100\n" +
		"  $APO.CORI = 1.0\n" +
		"  $APO.CPTP = 5.0\n" +						//5 degrees for approximation move
		"  ;/////////////////// POSITIONING MATRIX\n" +
		"  BAS(#TOOL, " + std::to_string(tool) + ")\n" +
		"  BAS(#BASE, " + std::to_string(base) + ")\n" +
		";ENDFOLD (SET " + settingName + ")\n";
}
void ntKRLprogram::addExternal(string programName){
	if (isMastProg == true & isSubProg == false){
		externals += programName + "\n";
	}
}
void ntKRLprogram::addFunction(string functionName){
	functions = functions + "DEF LEADIN(FEED_RATE1:IN)\n" +
		"  DECL REAL FEED_RATE1\n" +
		"  $ANOUT[" + powderRed_01 + "] = FEED_RATE1\n" +
		"  $OUT[" + gasShield + "] = TRUE\n" +
		"  $OUT[" + gasCarrier + "] = TRUE\n" +
		"END\n\n" +
		///
		"DEF LEADOUT()\n" +
		"  $ANOUT[" + powderRed_01 + "] = 0\n" +
		"  $OUT[" + gasShield + "] = FALSE\n" +
		"  $OUT[" + gasCarrier + "] = FALSE\n" +
		"END\n\n" +
		///
		"DEF POWDERON(FEED_RATE1:IN)\n" +
		"  DECL REAL FEED_RATE1\n" +
		"  $ANOUT[" + powderRed_01 + "] = FEED_RATE1\n" +
		"END\n\n" +
		"DEF POWDEROFF()\n" +
		"  $ANOUT[" + powderRed_01 + "]=0\n" +
		"END\n\n" +
		///
		"DEF LASERON()\n" +
		//SYN OUT " + laserActive + " 'Laser Shutter Fiberlaser' State = TRUE   at END
		//"  $OUT[" + laserActive + "] = TRUE\n" +
		"  $OUT_C[" + laserActive + "] = TRUE\n" +
		"END\n\n" +
		///
		"DEF LASEROFF()\n" +
		//"  $OUT[" + laserActive + "] = FALSE\n" +
		"  $OUT_C[" + laserActive + "] = FALSE\n" +
		"END\n\n" +
		"\n"
		///
		"DEF BASE()\n" +
		"  $BASE = BASE_DATA[1]\n" +		
		"END\n\n" +
		///
		"DEF BASE_EXT()\n" +
		"  $ASYNC_AXIS = $ASYNC_AXIS B_AND 'B111100'; set E1 and E2 in SYNC mode\n" +
		"  $BASE = EK(MACHINE_DEF[2].ROOT, MACHINE_DEF[2].MECH_TYPE, { X " + to_string(extOff.x) + ", Y " + to_string(extOff.y) + ", Z " + to_string(extOff.z) + ", A " + to_string(extOri.x) + ", B " + to_string(extOri.y) + ", C " + to_string(extOri.z) + " })\n" +
		"  $ACT_EX_AX = 2\n" +
		"END\n\n";
}
void ntKRLprogram::addDeclartion(string declarationName){
	declarations = declarations +
		//"DECL INT I \n" +
		"DECL REAL SCAN_SPEED\n" +
		"DECL REAL TRVL_SPEED\n" +
		"DECL REAL BEAM_DELAY\n" +
		"DECL REAL FEED_RATE1\n" +
		"DECL REAL OFST_DIST1\n\n";

	//"DECL " + variableNames + "\n" +
	//"SIGNAL LASER  $OUT[34]\n";
	//"SIGNAL POWDER $ANOUT[1]\n" +
}
void ntKRLprogram::addInitialization(){
	initializations = initializations +
		"TRVL_SPEED= " + vel_trvl + "; m/s \n" +
		"SCAN_SPEED= " + vel_scan + "; m/s \n" +
		"BEAM_DELAY= " + beamDelay + "; seconds \n" +
		"FEED_RATE1= " + powderFeed + "; volts \n" +
		"OFST_DIST1= " + offsetDist + "; mm \n";
}
void ntKRLprogram::addVariables(string variableNames){
	declarations += externals;
}

///FUNCTIONS FOR SUBPROGRAM PARITY
string ntKRLprogram::getDeclarations(){
	return this->declarations;
}
string ntKRLprogram::getInitializations(){
	return this->initializations;
}
string ntKRLprogram::getFunctions(){
	return this->functions;
}

void ntKRLprogram::setDeclarations(string declarations){
	this->declarations = declarations;
}
void ntKRLprogram::setInitializations(string initializations){
	this->initializations = initializations;

}
void ntKRLprogram::setFunctions(string functions){
	this->functions = functions;
}

///MOTION OPERATIONS
string ntKRLprogram::LIN(Vec3 pos, Vec3 ori, float ex1, float ex2, float ex3, float ex4, float ex5, float ex6){
	
	addVert(&pos);
	//std::cout << "in LIN " << verts.back()->pos->x << endl;

	string command = "LIN{ E6POS:"
		"  X " + to_string(pos.x) +
		", Y " + to_string(pos.y) +
		", Z " + to_string(pos.z) +
		", A " + to_string(ori.x) +
		", B " + to_string(ori.y) +
		", C " + to_string(ori.z) +
		", E1 " + to_string(ex1) +
		", E2 " + to_string(ex2) +
		", E3 " + to_string(ex3) +
		", E4 " + to_string(ex4) +
		", E5 " + to_string(ex5) +
		", E6 " + to_string(ex6) +
		" } " + approx +"\n";

	this->instructions += command;
	return command;
}
string ntKRLprogram::PTP(Vec3 pos, Vec3 ori, float ex1, float ex2, float ex3, float ex4, float ex5, float ex6){
	
	addVert(&pos);

	string command = "PTP{ E6POS:"
		"  X " + to_string(pos.x) +
		", Y " + to_string(pos.y) +
		", Z " + to_string(pos.z) +
		", A " + to_string(ori.x) +
		", B " + to_string(ori.y) +
		", C " + to_string(ori.z) +
		", E1 " + to_string(ex1) +
		", E2 " + to_string(ex2) +
		", E3 " + to_string(ex3) +
		", E4 " + to_string(ex4) +
		", E5 " + to_string(ex5) +
		", E6 " + to_string(ex6) +
		" }\n";

	this->instructions += command;
	return command;
}
string ntKRLprogram::CIRC(Vec3 auxPt, Vec3 endPt, float theta){
	//CIRC {X __,Y __, Z __},{X __,Y __,Z __,A __,B __,C __}, CA ___ C_ORI;
	string command = "CIRC{ E6POS: "
		"X " + to_string(auxPt.x) + ", " +
		"Y " + to_string(auxPt.y) + ", " +
		"Z " + to_string(auxPt.z) + "},{" +
		"X " + to_string(endPt.x) + ", " +
		"Y " + to_string(endPt.y) + ", " +
		"Z " + to_string(endPt.z) + "}, " +
		"CA " + to_string(theta) + " C_DIS";
	return command;
}

string ntKRLprogram::def_FCT(string var){
	string command = ";COMMAND";
	return command;
}
string ntKRLprogram::def_PATH(vector <Vec3*> pts){

	string command = "LEADIN(FEEDRATE1)\n";
	for (int i = 0; i < pts.size(); i++){
		command += LIN(*pts[i]) + "\n";
	}
	command += "LEADOUT()\n";
	this->instructions += command;
	return command;
}

///WORKCELL DEFINITION AND POSITIONING
void ntKRLprogram::setTool(int tool){
	this->tool = tool;
}
void ntKRLprogram::setBase(int base){
	this->base = base;
}

void ntKRLprogram::setSubstrate(int substrate){
	this->substrate = substrate;
}

void ntKRLprogram::setBasePos(Vec3 base){
	this->basePos = base;
}
void ntKRLprogram::setHome(Vec3 home){
	this->homePos = home;
	this->instructions += LIN(home) + " ;HOME POSITION\n";
}
void ntKRLprogram::setStart(float A1, float A2, float A3, float A4, float A5, float A6, float E1, float E2, float E3, float E4, float E5, float E6){
	this->startPos = ";FOLD STARTPOSITION PTP, VEL 10% , A1 " + std::to_string(A1) + ", A2 " + std::to_string(A2) + ", A3 " + std::to_string(A3) + ", A4 " + std::to_string(A4) + ", A5 " + std::to_string(A5) + ", A6 " + std::to_string(A6) + "\n"
		"  $BWDSTART = FALSE\n"
		"  PDAT_ACT = { VEL 10, ACC 100, APO_DIST 100 }\n"
		"  BAS(#PTP_DAT)\n"
		"  FDAT_ACT = { TOOL_NO " + std::to_string(this->tool) + ", BASE_NO " + std::to_string(this->base) + ", IPO_FRAME #BASE }\n"
		"  BAS(#FRAMES)\n"
		"  BAS(#VEL_PTP, 10)\n"
		"  $H_POS = { A1 " + std::to_string(A1) + ", A2 " + std::to_string(A2) + ", A3 " + std::to_string(A3) + ", A4 " + std::to_string(A4) + ", A5 " + std::to_string(A5) + ", A6 " + std::to_string(A6) + ", E1 " + std::to_string(E1) + ", E2 " + std::to_string(E2) + ", E3 " + std::to_string(E3) + ", E4 " + std::to_string(E4) + ", E5 " + std::to_string(E5) + ", E6 " + std::to_string(E6) + " }\n"
		"  PTP      { A1 " + std::to_string(A1) + ", A2 " + std::to_string(A2) + ", A3 " + std::to_string(A3) + ", A4 " + std::to_string(A4) + ", A5 " + std::to_string(A5) + ", A6 " + std::to_string(A6) + ", E1 " + std::to_string(E1) + ", E2 " + std::to_string(E2) + ", E3 " + std::to_string(E3) + ", E4 " + std::to_string(E4) + ", E5 " + std::to_string(E5) + ", E6 " + std::to_string(E6) + " }\n"
		";ENDFOLD\n\n"
		";FOLD PTP FIRST POSITION, VEL = 10 % , TOOL[" + std::to_string(this->tool) + "], BASE[" + std::to_string(this->base) + "]\n"
		"  $BWDSTART = FALSE\n"
		"  PDAT_ACT = { VEL 10, ACC 100, APO_DIST 100 }\n"			///SWAP VEL FOR VARIABLE
		"  FDAT_ACT = { TOOL_NO " + std::to_string(this->tool) + ", BASE_NO " + std::to_string(this->base) + ", IPO_FRAME #BASE }\n"
		"  BAS(#FRAMES)\n"
		"  BAS(#VEL_PTP, 10)\n"										///SWAP VEL FOR VARIABLE
		";ENDFOLD\n\n";
}
void ntKRLprogram::setEnd(float A1, float A2, float A3, float A4, float A5, float A6, float E1, float E2, float E3, float E4, float E5, float E6){
	///refactor to toggle halt based on boolean 
	if (this->isHalt == true) {
		this->endPos = "HALT\nEND\n\n";
	} else {
		this->endPos = "\n"
			";FOLD ENDPOSITION PTP, VEL 10 % , A1 " + std::to_string(A1) + ", A2 " + std::to_string(A2) + ", A3 " + std::to_string(A3) + ", A4 " + std::to_string(A4) + ", A5 " + std::to_string(A5) + ", A6 " + std::to_string(A6) + "\n"
			"  $BWDSTART = FALSE\n"
			"  PDAT_ACT = { VEL 10, ACC 100, APO_DIST 100 }\n"
			"  BAS(#PTP_DAT)\n"
			"  FDAT_ACT = { TOOL_NO " + std::to_string(this->tool) + ", BASE_NO " + std::to_string(this->base) + ", IPO_FRAME #BASE }\n"
			"  BAS(#FRAMES)\n"
			"  BAS(#VEL_PTP, 10)\n"
			"  $H_POS = { A1 " + std::to_string(A1) + ", A2 " + std::to_string(A2) + ", A3 " + std::to_string(A3) + ", A4 " + std::to_string(A4) + ", A5 " + std::to_string(A5) + ", A6 " + std::to_string(A6) + ", E1 " + std::to_string(E1) + ", E2 " + std::to_string(E2) + ", E3 " + std::to_string(E3) + ", E4 " + std::to_string(E4) + ", E5 " + std::to_string(E5) + ", E6 " + std::to_string(E6) + " }\n"
			"  PTP      { A1 " + std::to_string(A1) + ", A2 " + std::to_string(A2) + ", A3 " + std::to_string(A3) + ", A4 " + std::to_string(A4) + ", A5 " + std::to_string(A5) + ", A6 " + std::to_string(A6) + ", E1 " + std::to_string(E1) + ", E2 " + std::to_string(E2) + ", E3 " + std::to_string(E3) + ", E4 " + std::to_string(E4) + ", E5 " + std::to_string(E5) + ", E6 " + std::to_string(E6) + " }\n"
			";ENDFOLD\n\n"
			"END\n\n";
	}
}
void ntKRLprogram::setVel(string velocity){
	this->velocity = velocity;
}

void ntKRLprogram::setVel_trvl(string velocity){
	this->vel_trvl = velocity;
}
void ntKRLprogram::setVel_scan(string velocity){
	this->vel_scan = velocity;
}

void ntKRLprogram::setExtOffs(Vec3 offset){
	this->extOff = offset;
}
void ntKRLprogram::setExtOri(Vec3 orientation){
	this->extOri = orientation;
}

void ntKRLprogram::setApproximation(string method){
	this->approx = method;
}

///STANDARD FUNCTION CALLS
void ntKRLprogram::delay(string seconds){
	this->instructions += "WAIT SEC " + seconds + "\n";
}
void ntKRLprogram::command(string command){
	this->instructions += command + "\n";
}
void ntKRLprogram::comment(string comment){
	this->instructions += ";" + comment + "\n";
}

///WORK CELL SPECIFIC FUNCTION CALLS
void ntKRLprogram::powderOn(){
	this->instructions += "POWDERON(" + powderFeed + ")\n";
}
void ntKRLprogram::powderOff(){
	this->instructions +="POWDEROFF()\n";
}

void ntKRLprogram::laserOn(){
	this->instructions += "LASERON()\n";
}
void ntKRLprogram::laserOff(){
	this->instructions += "LASEROFF()\n";
}

void ntKRLprogram::leadIn(){
	this->instructions += "LEADIN(FEED_RATE1)\n";
}
void ntKRLprogram::leadOut(){
	this->instructions += "LEADOUT()\n";
}

///WORK CELL SPECIFIC VARIABLE DECLARATIONS
void ntKRLprogram::powderVel(string signal){
	///refactor feed rate to calculate voltage
	//voltage must be between 0 to 1.0
	this->powderFeed = signal;
}
void ntKRLprogram::laserPow(string signal){
	this->laserPower = signal;
}
void ntKRLprogram::laserDelay(string seconds){
	this->beamDelay = seconds;
}
void ntKRLprogram::hatchOffset(string offsetDist){
	this->offsetDist = offsetDist;
}