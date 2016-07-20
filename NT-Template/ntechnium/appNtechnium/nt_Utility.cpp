///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////// nt_Utility.cpp
// openGl scratch libs							///////////////////
// Utility Class for Directory Assissitance		///////////////////
// created by James Warton on 02/23/2014		///////////////////
///////////////////////////////////////////////////////////////////
#include "nt_Utility.h"

using namespace jpw;
///////////////////////////////////////////////////////////////////
//////////////////////////////////////////  FUNCTION IMPLEMENTATION
std::string nt_Utility::getBuildPath(){
	char cCurrentPath[FILENAME_MAX];
	//char full[_MAX_PATH];

	if (_fullpath(cCurrentPath, "", _MAX_PATH) != NULL) {
	} else {
		printf("INVALID PATH\n");
	}

	std::string cp = cCurrentPath; //cast char[] to string
	return cp;
	//std::cout << "current path = " << cp << std::endl << std::endl;
}
std::string nt_Utility::getPathToResources(){
	char cCurrentPath[FILENAME_MAX];

	//char full[_MAX_PATH];
	if (_fullpath(cCurrentPath, "", FILENAME_MAX) != NULL) {
		printf("");
	}
	else {
		printf("INVALID PATH\n");
	}

	// NOTE - make workspace project relative instead of using default derivedData path in Library
	cCurrentPath[sizeof(cCurrentPath)-1] = '\0'; /* not really required */
	
	std::string cp = cCurrentPath; //cast char[] to string
	std::string cull = "nt-project template\\NT-template";

	// MOVE PATH UP TO PROJECT PATH
	int start_position_to_erase = cp.find(cull);
	int strLen = cull.length();
	cp.erase(start_position_to_erase, strLen);

	// need to escape backslashes with themselves, ick
	std::string pathExtension = "ntechnium\\resources\\";
	std::string pathURL = cp + pathExtension;
	return pathURL;
}
std::string nt_Utility::getPathToOutput(){
	char cCurrentPath[FILENAME_MAX];
	if (_fullpath(cCurrentPath, "", FILENAME_MAX) != NULL){
		print("");
	}
	else {
		printf("INVALID PATH\n");
	}
	cCurrentPath[sizeof(cCurrentPath)-1] = '\0'; // MAY NOT BE REQUIRED
	std::string temp;
	std::string cp;

	temp = cCurrentPath;

	temp.erase(temp.rfind('\\'), std::string::npos);
	temp.erase(temp.rfind('\\'), std::string::npos);
	cp = temp + "\\ntechnium\\output\\";

	return cp;
}
///////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////// CHAR* URL INPUT
std::string nt_Utility::load(const char* data){
	//trace("data =", data);
	std::ifstream file(data);
	std::string line, block;
	if (file.is_open()) {

		while (!file.eof()) {
			std::getline(file, line);
			block.append(line);
			block.append(std::string("\n"));
		}
		file.close();
		return block;
	}
	else {
		std::cout << "Unable to open file";
	}
	// never reached - added to squelch ide warning
	return 0;
}
///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////// STRING URL INPUT
const std::string nt_Utility::load(std::string data){
	const char *charData = data.c_str();
	return load(charData);
}
///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////// IMAGE .RAW INPUT
void nt_Utility::load(const std::string& url, int w, int h, unsigned char* data){
	FILE * file;
	///////////////////////////////////////////// OPEN TEXTURE DATA
	// from: http://social.msdn.microsoft.com/Forums/vstudio/en-US/fac75a89-dc1b-4b3a-b365-1ac151eee63e/using-fopens-again?forum=vcgeneral
	errno_t e = fopen_s(&file, url.c_str(), "rb");
	if (e != 0) printf("Error Loading File");

	if (file == NULL) std::cout << "NO FILE DATA";
	///////////////////////////////////////// READ TEXTURE MAP DATA
	fread(data, w * h * 3, 1, file);
	fclose(file);
}
std::vector<std::string> nt_Utility::getFileNames(std::string dir){

	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	TCHAR szDir[MAX_PATH];
	size_t length_of_arg;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;

	std::vector<std::string> fileNames;

	// Prepare string for use with FindFile functions.  First, copy the
	// string to a buffer, then append '\*' to the directory name.
	StringCchCopy(szDir, MAX_PATH, dir.c_str());
	StringCchCat(szDir, MAX_PATH, TEXT("*"));

	// Find the first file in the directory.
	hFind = FindFirstFile(szDir, &ffd);

	// List all the files in the directory with some info about them.
	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
			//std::string fn(ffd.cFileName);
			//trace(fn);
			//fileNames.push_back(fn);
		}
		else
		{
			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;
			//_tprintf(TEXT("  %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);
			fileNames.push_back(ffd.cFileName);
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	FindClose(hFind);
	return fileNames;
}
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////