///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// ntPtsTxt.h
// openGl scratch libs							///////////////////
// Point Data I/O and manipulation				///////////////////
// created by James Warton on 04/05/2014		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef TEXT_IO_JPW_NTECHNIUM
#define TEXT_IO_JPW_NTECHNIUM

#include <iostream>
#include <fstream>
#include <vector>
#include <string>       // std::string
#include <sstream>      // std::stringstream, std::stringbuf
#include <algorithm>	//needed for string.erase w/ std::remove

#include "nt_Utility.h"
#include "ntVec3.h"
#include "ntBaseShape.h"

using namespace std;

namespace jpw{
	class ntTextIO;
	typedef ntTextIO ptTxt;

	class ntTextIO{
	private:
		string url;
		string path;
		string pathExtension;
		string fileName;

		vector<string> lines;
		vector<string> files;

		float vertPos[3];
		float normVec[3];
		int rows_U;
		int rows_V;

		void setUrl();
		void string_to_Vecs(); //string interp to vec data

	protected:
		///these could be handled within mesh class
		vector<ntVec3*>* verts = new vector<ntVec3*>();
		vector<ntVec3*>* norms = new vector<ntVec3*>();

	public:
		///////////////////////////////////////////////////////////
		////////////////////////////////////////////// CONSTRUCTORS
		ntTextIO();
		ntTextIO(string fileName);
		ntTextIO(string pathExtension, string fileName);

		///////////////////////////////////////////////////////////
		////////////////////////////////////////// PUBLIC FUNCTIONS
		void init(); //initialize path and filenames
		void read(); //read data stored in path

		std::vector<ntVec3*>* getVecsV();	///these could be stored
		std::vector<ntVec3*>* getVecsN();	///within mesh class
		int get_U();
		int get_V();
	};
	///////////////////////////////////////////////////////////////
	////////////////////////////////////////// END CLASS DEFINITION
	inline std::vector<ntVec3*>* ntTextIO::getVecsV() {
		return verts;
	}
	inline std::vector<ntVec3*>* ntTextIO::getVecsN() {
		return norms;
	}
	inline int ntTextIO::get_U(){
		return rows_U;
	}
	inline int ntTextIO::get_V(){
		return rows_V;
	}
}
#endif
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
