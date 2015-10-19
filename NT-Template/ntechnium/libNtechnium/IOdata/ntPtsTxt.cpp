#include "ntPtsTxt.h"

using namespace jpw;

ntTextIO::ntTextIO(){
	this->path = nt_Utility::getPathToResources();
	this->pathExtension = "data\\";
	this->fileName = "pointPos_00.txt";
	init();
}

ntTextIO::ntTextIO(std::string fileName) :
fileName(fileName){
	this->path = nt_Utility::getPathToResources();
	this->pathExtension = "ntechnium\\resources\\data\\";
	init();
}

ntTextIO::ntTextIO(std::string pathExtension, std::string fileName) :
pathExtension(pathExtension), fileName(fileName){
	this->path = nt_Utility::getPathToResources();
	init();
}

///////////////////////////////////////////////////////////////////
////////////////////////////////////// INITIALIZE PATH TO ALL FILES
void ntTextIO::init(){
	///FILL FILES VECTORS WITH PATH FOR EACH FILE IN PATH'
	///some of this may occur in BaseApp
	//file_index = 0;
	//for file in files read(file);
	setUrl();
	read();
	//store file as vector of string data
}
void ntTextIO::setUrl(){
	std::string cCurrentPath = path;
	cCurrentPath[sizeof(cCurrentPath)-1] = '\0'; // not required //
	std::string cp = cCurrentPath;

	std::cout << "\nPATH TO TEXT DATA I/O: " << std::endl;
	std::cout << path + pathExtension + fileName << "\n" << std::endl;

	url = "..\\..\\" + pathExtension + fileName;
}

void ntTextIO::read(){
	ifstream file(url);
	std::string lineStr;

	if (file.is_open()){
		while (getline(file, lineStr)){
			lines.push_back(lineStr);
		}
		file.close();
	}
	else cout << "UNABALBE TO OPEN FILE \n";

	string token;

	for (int i = 0; i < lines.size(); i++){
		
		string line = lines.at(i);

		token = "/// U:";
		if (line.find(token) != string::npos){
			char chars[] = "/// U:";
			for (unsigned int j = 0; j < strlen(chars); ++j)
			{
				line.erase(std::remove(line.begin(), line.end(), chars[j]), line.end());
			}
			string U = line;
			float val = stof(U);
			rows_U = val;
		}

		token = "/// V:";
		if (line.find(token) != string::npos){
			char chars[] = "/// V:";
			for (unsigned int j = 0; j < strlen(chars); ++j)
			{
				line.erase(std::remove(line.begin(), line.end(), chars[j]), line.end());
			}
			string V = line;
			float val = stof(V);
			rows_V = val;
		}

		token = "POS:";
		if (line.find(token) != string::npos){

			char chars[] = "/POS:{ }";
			for (unsigned int j = 0; j < strlen(chars); ++j)
			{
				line.erase(std::remove(line.begin(), line.end(), chars[j]), line.end());
			}

			/////split stream at comma and insert into position array = posXYZ
			string pos;
			stringstream stream(line);
			float val;
			int cnt = 0;

			while (getline(stream, pos, ',')){
				val = stof(pos);
				if (cnt <= 2){
					vertPos[cnt] = val;
				}
				else {
					normVec[(cnt - 3)] = val;
				}
				cnt += 1;
			}
			/////insert vectors into respective data structure
			ntVec3* vert = new ntVec3(vertPos[0], vertPos[1], vertPos[2]);
			ntVec3* norm = new ntVec3(normVec[0], normVec[1], normVec[2]);
			/// error//
			verts->push_back(vert); ///when verts is set to nullptr or left without initialization
			/// error//
			norms->push_back(norm);
			//cout << " vertex (" << vert.x << "," << vert.y << "," << vert.z << ")" << "\n";
			//cout << " normal (" << vert.x << "," << vert.y << "," << vert.z << ")" << "\n";
		}
	}
}
