#include "nt_SRCwriter.h"

SRCwriter::SRCwriter(){
	this->fileName = "SRCwriter";
	this->path = nt_Utility::getPathToOutput();
	this->pathExtension = "src\\";
}
SRCwriter::SRCwriter(string fileName):
fileName(fileName){
	this->path = nt_Utility::getPathToOutput();
	this->pathExtension = "src\\";

	init();
}
SRCwriter::SRCwriter(string path, string fileName):
path(path), fileName(fileName){
	init();
}

void SRCwriter::init(){
	url = path + pathExtension + fileName + fileExt;

	//addSetting("proxy setting 01");
	//addFunction("proxy function 01");
	//addVariables("proxy var 01");
}
void SRCwriter::writeFile(){

	writeHeader();
	writeBody();
	writeFooter();

	ofstream file(url);
	file << header;
	file << body;
	file << footer;
	file.close();
}

void SRCwriter::writeHeader(){
	timeStamp = time(0);

	localtime_s(&timeData, &timeStamp);
	string Y = to_string(timeData.tm_year + 1900);
	string M = to_string(timeData.tm_mon + 1);
	string D = to_string(timeData.tm_mday);
	string Hr = to_string(timeData.tm_hour);
	string Mn = to_string(timeData.tm_min);
	string Sc = to_string(timeData.tm_sec);

	string date = ";DATE: " + Y + ":" + M + ":" + D + " | " + Hr + ":" + Mn + ":" + Sc + "\n";

	transform(fileName.begin(), fileName.end(), fileName.begin(), ::toupper);
	header = "&ACCESS RVP\n"
		"&REL 7\n"
		"&PARAM TEMPLATE = C:\\KRC\Roboter\\Template\\vorgabe\n"
		"&PARAM EDITMASK = *\n"
		"DEF " + fileName + "() \n" +
		";PROGRAM:  " + fileName + "\n" +
		";GENERATED WITH NT LIBS\n" +
		";AUTHORED BY JAMES WARTON\n" +
		date +
		";///////////////////////////////////////// USER DECLARATIONS\n" +
		declarations + "\n" +
		//"GLOBAL INTERRUPT DECL 3 WHEN $STOPMESS==TRUE DO IR_STOPM()" + "\n" +
		//"INTERRUPT ON 3" + "\n" +
		//"BAS(#INITMOV, 0)" + "\n\n" +

		";///////////////////////////////////////// USER INITIALIZATIONS\n" +
		initializations + "\n" + startPos + "\n" + settings + "\n";
}
void SRCwriter::writeBody(){
	body =   ";//////////////////////////////////////////// BEGIN BODY\n\n";
	body += instructions+"\n";
}
void SRCwriter::writeFooter(){
	footer = ";//////////////////////////////////////////// BEGIN FOOTER\n";
	footer = footer + endPos + functions + "\n";
}
