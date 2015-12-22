#include "nt_09_ovisApp.h"
#include <GL/glu.h>

using namespace arma;

int ovisApp::gen = 1;
int ovisApp::gen_L = gen;
int ovisApp::gen_G = gen;
bool ovisApp::isImgLoaded = false;
bool ovisApp::isTxtLoaded = false;
int ovisApp::img_X = 0;
int ovisApp::img_Y = 0;
int ovisApp::img_T = 1;

std::vector<float> ovisApp::p_Rad;
int ovisApp::n_perf = 0;
float ovisApp::areaS_gross = 0;
float ovisApp::areaS_net = 0;
float ovisApp::phi_Min = 180;
float ovisApp::phi_Max = 0;

arma::mat ovisApp::img_00 = arma::zeros<mat>(img_X, img_Y);
///////////////////////////////////////////////////////////////
///////////////////////////////////////// SOURCE DATA VARIABLES
string ovisApp::path = nt_Utility::getPathToOutput();

void ovisApp::init() {

	read_DATA();

	std::cout << "\n\n///////////////////////////////////////////////////////////////\n";
	std::cout << "READ TXT COMPLETE           -----------------------------------" << endl;
	panel_Dim = panels.size();
	std::cout << "TOTAL PANELS LOADED:        " << panel_Dim << "\n" << endl;

	read_IMG();

	std::cout << "READ IMG COMPLETE           -----------------------------------" << endl;
	std::cout << "IMAGE SIZE:                 " << img_X << " x " << img_Y << "\n" << endl;
	std::cout << "CALCULATING PANEL GEOMETRY  -----------------------------------" << endl;

	t_CPU = clock();

	//isMultiThread = true;
	unsigned thread_Cnt = std::thread::hardware_concurrency();

	int index_S = 0;
	int index_E = 0;
	float items = (panel_Dim / thread_Cnt) + 1;
	items = ceil(items);

	if (isMultiThread == true) {
		for (int i = 0; i < thread_Cnt; i++) {

			index_S =  items * i;
			index_E = (items * i) + items - 1;

			if (index_E > panel_Dim - 1) {
				index_E = panel_Dim - 1;
			}

			async(launch::async, ovisApp::fun02, index_S, index_E, &panels, i);
			//a.get();

			//vector<thread> ths;
			//ths.push_back(thread(ovisApp::fun02, index_S, index_E, &panels, i));
			//for (auto& th : ths) {
			//	th.join();
			//}
		}
	} else {
		///////////////////////////////////////////////////////////////
		///////////////////////////////////////////////// SINGLE THREAD
		for (int i = 0; i < panel_Dim; i++) {
			ntPanel* panel_ptr(panels.at(i));
			funct(panel_ptr);
		}
	}
	t_CPU = clock() - t_CPU;

	gen_L = panels.at(0)->faces_L.size()-1;

	std::cout << "EVAL_CPU TIME  [SECONDS]:   " << ((float)t_CPU) / CLOCKS_PER_SEC << "\n" << endl;

	std::cout << "SYSTEM STATISTICS           -----------------------------------" << endl;
	std::cout << "SUBDIVISION GENERATION:     " << panels.at(0)->cnt_SubDiv << endl;
	std::cout << "FACE VECTOR SIZE [GEN]:     " << panels.at(0)->faces_G.size() << endl;
	std::cout << "GLOBAL FACES:               " << panels.at(0)->faces_G.at(gen)->size() << endl;
	std::cout << "LOCAL FACES:                " << panels.at(0)->faces_L.at(gen_L)->size() << "\n" << endl;
	std::cout << "PANELS:                     " << panel_Dim << endl;
	std::cout << "SURFACE AREA GROSS:         " << areaS_gross / 144 << endl;
	std::cout << "SURFACE AREA NET:           " << areaS_net / 144 << endl;
	std::cout << "PERCENT OF OPENING:         " << 100 - (areaS_net / areaS_gross) * 100 << "\n" << endl;
	std::cout << "PERFORATIONS:               " << p_Rad.size() << "\n" << endl;

	std::cout << "MIN ANGLE:                  " << phi_Min << endl;
	std::cout << "MAX ANGLE:                  " << phi_Max << endl;
	std::cout << "DEVIATION:                  " << phi_Max - phi_Min << "\n" << endl;

	std::cout << "///////////////////////////////////////////////////////////////\n";
	index_S = 0;
	index_E = 0;

	///////////////////////////////////////////////////////////////////////
	/////////////////////////////////////// GRAPH PERFORATION | PANELS DATA
	int w = 448;
	int h = 40;

	ntVec3 *graphPos = new ntVec3(  5, 460, 0);
	ntVec3 *graphDim = new ntVec3(w,  h, 0);
	int set_size = p_Rad.size() - 1;

	if (set_size > 0) {
		graph00 = ntGraph(graphPos, graphDim, p_Rad);
		graph00.set_Color(ntColor4f(0.3, 0.3, 0.3, 1));
		graph00.sort();
		graph00.init();
	}

	ntVec3 *graph01_Pos = new ntVec3(5, 452, 0);
	ntVec3 *graph01_Dim = new ntVec3(w, 10, 0);

	graph01 = ntGraph(graph01_Pos, graph01_Dim, Y_axis);
	graph01.set_Param(areaS_gross, 0);
	graph01.set_Param(areaS_net, 1);
	graph01.set_Width(4);
	graph01.init();

	///////////////////////////////////////////////////////////////////////
	/////////////////////////////////////// GRAPH PERFORATION | PANELS DATA
	if (isMultiThread == true) {
		for (int i = 0; i < thread_Cnt; i++) {
			index_S = items * i;
			index_E = (items * i) + items - 1;

			if (index_E > panel_Dim - 1) {
				index_E = panel_Dim - 1;
			}
			//async(launch::async, ovisApp::write_Panels_TXT, index_S, index_E, &panels);
		}
	}
}

void ovisApp::read_DATA(){
	string pathI = nt_Utility::getPathToResources() + "data\\" + fileName_TXT + ".txt";

	ifstream file(pathI);
	string line;

	int cnt = 0;
	std::vector<ntVec3*> params_UV;

	while (std::getline(file, line) && isEndSubs == false) {
		if (line.find(";BEGIN") != string::npos) {
			isStartFile = true;
		}
		if (isStartFile == true && line.find("PAN:") != string::npos) {
			char chars[] = "PAN:";
			for (unsigned int j = 0; j < strlen(chars); ++j)
			{
				line.erase(std::remove(line.begin(), line.end(), chars[j]), line.end());
			}
			panel_ID = line;
		}
		if (isStartFile == true && line.find("PANELNORM:") != string::npos) {
			line = format_STR(line);
			panel_Norm = line + "\n";
		}
		if (isStartFile == true && line.find("PANELUV:") != string::npos) {
			///EXCEPTION FOR > 3 UVW ENTRIES
			ntVec3 temp = add_VEC(line);  /// CREATE POINTER TO VEC
			ntVec3 * uvw = new ntVec3(temp.x, temp.y, temp.z);
			params_UV.push_back(uvw);

			line = format_STR(line);
			panel_UVW = line + "\n";
		}
		if (isStartFile == true && line.find("POS:") != string::npos) {
			ntVec3 vec = add_VEC(line);
			verts[cnt] = vec;
			if (cnt < 2) {
				cnt = cnt + 1;
			}
			else {
				cnt = 0;
			}
		}
		if (line.find("NEXT") != string::npos) {
			isSubNext = true;
		}
		else { isSubNext = false; }

		if (line.find(";END") != string::npos) {
			isEndFile = true;
		}

		//DISTRIBUTE DATA TO PANEL DEFINITION
		if (isSubNext == true || isEndFile == true) {
			/////////////////////////////////////////////////////////////////////////////////
			ntVec3 * v0 = new ntVec3(verts[0].x, verts[0].y, verts[0].z);
			ntVec3 * v1 = new ntVec3(verts[1].x, verts[1].y, verts[1].z);
			ntVec3 * v2 = new ntVec3(verts[2].x, verts[2].y, verts[2].z);
			ntPanel * panel = new ntPanel(v0, v1, v2);

			panel->set_ID(panel_ID);
			panel->set_nG(panel_Norm);
			panel->set_UVW(panel_UVW);			// STRNG UVW
			panel->set_UVW(params_UV);			// VEC3 UVW
			panel->vecs_UV.swap(params_UV);		// FOR RHINO READ ONLY ---REMOVE AFTER UVW SUBDIVIDE COMPLETED
			panel->set_pG(panel_Vert);
			panels.push_back(panel);

			panel_ID =		"<< ERROR >>";
			panel_Norm =	"<< ERROR >>";
			panel_UVW =		"<< ERROR >>";
			panel_Vert =	"";
			vector<ntVec3*>().swap(params_UV);		//DEALLOCATE MEMORY STORED IN PARAMS_UV
		}
	}
	isTxtLoaded = true;
}
void ovisApp::read_IMG() {
	url_IMG = path_IMG + pathExtension_IMG + fileName_IMG + fileExt_IMG;

	const char * file = url_IMG.c_str();
	img_IN = af::loadImage(file, false);

	img_X = img_IN.dims(0);
	img_Y = img_IN.dims(1);

	/// ////////////////////// IMAGE RESIZE FOR 2D VIEW
	dim_t dim_Y = 448;
	dim_t dim_X = mapRange(0, 448, 0, img_Y, img_X);
	double scFx = 1.0 / img_T;

	img_2D = af::resize(af::loadImage(file, false), dim_X, dim_Y);
	img_2D = af::resize(scFx, img_2D);

	img_2d = zeros<mat>(img_2D.dims(0), img_2D.dims(1));
	af::array img_LOAD(img_2D.dims(0), img_2D.dims(1), img_2d.memptr());
	img_LOAD += img_2D;
	img_LOAD.host((void*)img_2d.memptr());
	img_2d = arma::flipud(img_2d);
	/// //////////////////////

	img_00 = zeros<mat>(img_X, img_Y);
	/// ADD EXCEPTION TO RESIZE IMAGE IF > ARRAY FIRE BOUNDS

	af::array img_LOADER(img_X, img_Y, img_00.memptr());
	img_LOADER += img_IN;
	
	img_LOADER.host((void*)img_00.memptr());
	img_00 = arma::flipud(img_00);

	isImgLoaded = true;
}
void ovisApp::write_Panel_TXT(ntPanel* panel_ptr) {

	stringstream ss;
	ss << std::setw(5) << std::setfill('0');
	ss << panel_ptr->get_ID();

	string pathExtension = "ovis\\txt\\";
	string fileName = "OP_" + ss.str();
	string fileExt = ".txt";
	string url = path + pathExtension + fileName + fileExt;

	///////////////////////////////////////////////////// SET TIME STAP
	struct tm timeData;
	time_t timeStamp;

	timeStamp = time(0);
	localtime_s(&timeData, &timeStamp);
	string Y = to_string(timeData.tm_year + 1900);
	string M = to_string(timeData.tm_mon + 1);
	string D = to_string(timeData.tm_mday);
	string Hr = to_string(timeData.tm_hour);
	string Mn = to_string(timeData.tm_min);
	string Sc = to_string(timeData.tm_sec);
	string date = "DATE: " + Y + ":" + M + ":" + D + " | " + "TIME: " + Hr + ":" + Mn + ":" + Sc + "\n";

	ofstream file(url);

	file << "//  HKS | LINE\n";
	file << "//  00603.000:: OVIS STADIUM\n";
	file << "//  PANEL PARAMETERS  | UNITS [ INCHES ]\n";
	file << "//  RELEASE " << date << "\n";
	file << "//  OVIS PANEL_" << ss.str() << "\n";
	file << "//  NODE POSITIONS:\n";
	file << "//  [ WORLD COORDINATES ]\n";
	file << "//        (                 X,                  Y,                  Z)\n";
	/// file << panel_ptr->get_p_G() << "\n";
	file << "     POS: " << format_VEC(panel_ptr->get_v_G().at(0)) << "\n";
	file << "     POS: " << format_VEC(panel_ptr->get_v_G().at(1)) << "\n";
	file << "     POS: " << format_VEC(panel_ptr->get_v_G().at(2)) << "\n";
	file << "\n";
	file << "//  PANEL ORIENTATION VECTOR:\n";
	file << "//   VEC" << panel_ptr->get_n_G();
	file << "//   UVW" << panel_ptr->get_UVW() << "\n";
	file << "//  AREA GROSS:  " << to_string(panel_ptr->get_Area()) << "           [ SQ.INCH ]\n";
	file << "//  AREA NET:    " << to_string(panel_ptr->get_Area() - panel_ptr->perf_area) << "           [ SQ.INCH ]\n";
	file << "//  AREA NET:    " << to_string(panel_ptr->perf_perc) << "           [ % ]\n\n";

	file << "//  A0:          " << panel_ptr->phi[0] << "               [ DEGREES ]\n";
	file << "//  A1:          " << panel_ptr->phi[1] << "               [ DEGREES ]\n";
	file << "//  A2:          " << panel_ptr->phi[2] << "               [ DEGREES ]\n";
	file << "//  SUM:         " << panel_ptr->phi[0] + panel_ptr->phi[1] + panel_ptr->phi[2] << "\n\n";

	file << "//////////////////////////////////////////////////////////////////////\n";
	file << "//  NODE POSITIONS:\n";
	file << "//  [ LOCAL COORDINATES ]\n";
	file << "//        (                 X,                  Y,                  Z)\n";
	file << "     POS: " << format_VEC(panel_ptr->v0) << "\n";
	file << "     POS: " << format_VEC(panel_ptr->v1) << "\n";
	file << "     POS: " << format_VEC(panel_ptr->v2) << "\n";
	file << "\n";
	file << "//  PANEL ORIENTATION VECTOR:\n";
	file << "//   VEC: " << format_VEC(&panel_ptr->norm) << "\n";
	file << "\n";
	file << "//  CORNER POSITIONS:          {POS:            }\n";
	file << "\n";
	file << "//  FASTENER POSITIONS:        {POS:  DIAMETER: }\n";
	file << "\n";
	file << "//  PERFORATION DATA:          {POS:  DIAMETER: }\n";
	file << "//  PERFORATION COUNT:  " << to_string(panel_ptr->get_Perf().size()) << "\n\n";
	for (int i = 0; i < panel_ptr->get_Perf().size(); i++) {
		file << "     POS:  " << format_VEC(panel_ptr->get_Perf().at(i)) << "       DIA:  " << panel_ptr->get_Perf_R().at(i) * 2 << "\n";
	}
	file.close();
}
void ovisApp::write_Panel_IMG(ntPanel* panel_ptr) {

	stringstream ss;
	ss << std::setw(5) << std::setfill('0');
	ss << panel_ptr->get_ID();

	string pathOut = nt_Utility::getPathToOutput();
	string pathExtension = "ovis\\img\\";
	string fileName = "OP_" + ss.str();
	string fileExt = ".jpg";
	string url = pathOut + pathExtension + fileName + fileExt;
	// IMAGE SIZE
	int img_x = 1280; //768;// 1024;
	int img_y = 1280; //768;// 1024;
	// MAXIMUM PANEL SIZE;
	int pX_max = 76;
	int pY_max = 76;
	// PIXEL RANGE LOOP
	int dim_X = mapRange(0, img_x, 0, pX_max, panel_ptr->v1->x); //img_X;// OR = pX_max * dpi
	int dim_Y = mapRange(0, img_y, 0, pY_max, panel_ptr->v2->y); //img_Y;// OR = pY_max * dpi

	bool isDPI = false;
	//isDPI = true;
	int dpi = 72;
	bool isPtAlpha = true;
	bool isPtPanel = false;
	bool isPtPerf = false;

	const char * file = url.c_str();
	//arma::mat img_ARM = zeros<mat>(img_x, img_y);
	af::array img_OUT = af::array(img_x, img_y);
	img_OUT *= 0;

	// MINIMIZE SEARCH AREA
	if (panel_ptr->v1->x > pX_max) {
		std::cout << "/nPANEL" << ss.str() << " X DIM IS LARGER THAN IMAGE SIZE" << endl;
	}
	if (panel_ptr->v2->y > pY_max) {
		std::cout << "/nPANEL" << ss.str() << " Y DIM IS LARGER THAN IMAGE SIZE" << endl;
	}

	// OPTION TO SET IMAGE SIZE BY DPI OR EXPLCIT SIZE
	if (isDPI == true) {
		img_x = pX_max * dpi;
		img_y = pY_max * dpi;
	}

	for (int i = 0; i < dim_X; i++) {
		for (int j = 0; j < dim_Y; j++) {
			//MAP (i, j) TO PANEL LOCAL COORDINATES
			float pos_X = mapRange(0, pX_max, 0, img_x, i);
			float pos_Y = mapRange(0, pY_max, 0, img_y, j);

			ntVec3 * pt = new ntVec3(pos_X, pos_Y, 0);

			isPtPanel = panel_ptr->pt_isInside(pt);

			if (isPtPanel == true) {
				isPtAlpha = false;
				for (int k = 0; k < panel_ptr->perfs.size(); k++) {
					isPtPerf = panel_ptr->perfs.at(k)->pt_isInside(pt);
					if (isPtPerf == true) {
						isPtAlpha = true;
						k = panel_ptr->perfs.size();
					}
				}
			}
			else {
				isPtAlpha = true;
			}

			if (isPtAlpha == false) {
				img_OUT(i, j) = 255;
			}
			else {
				img_OUT(i, j) = 0;
			}
		}
	}
	/// FAILS TO PASS ARMA MATRIX TO AF BEYOND SIZE LIMIT
	//af::array img_OUT(img_x, img_y, img_ARM.memptr());
	//img_OUT.host((void*)img_ARM.memptr());
	//img_ARM.print(	"IMG_ARM:  ");
	//af::print(		"IMG_OUT:   ", img_OUT);
	img_OUT = af::transpose(img_OUT);
	img_OUT = af::flip(img_OUT, 0);
	af::saveImage(file, img_OUT);
}

///////////////////////////////////////////////////////////////
///////////////// HANDLER FUNCTIONS FOR MULTITHREADED EXECUTION
void ovisApp::funct(ntPanel* panel_ptr) {
	
	Vec3* axis_Z = new Vec3(0, 0, 1);
	Vec3* axis_X = new Vec3(1, 0, 0);

	///////////////////////////////////////////////////////////////
	/////////////////////////////// ALIGN CENTROID/NORMAL TO Z-AXIS
	panel_ptr->calcCentroid();
	panel_ptr->calcNorm();
	align_Panel(panel_ptr, axis_Z, &panel_ptr->norm, panel_ptr->cent);
	///////////////////////////////////////////////////////////////
	/////////////////////////////////////// ALIGN V0/EDGE TO X-AXIS
	Vec3 edge_X = Vec3(panel_ptr->v1->x, panel_ptr->v1->y, panel_ptr->v1->z);
	edge_X.sub(panel_ptr->v0);
	align_Panel(panel_ptr, axis_X, &edge_X, panel_ptr->v0);

	panel_ptr->faces_L.at(0)->at(0).calcCentroid();	//REQUIRED AFTER SCALING
	panel_ptr->faces_L.at(0)->at(0).calcNorm();		//REQUIRED AFTER SCALING
	panel_ptr->calcArea();
	panel_ptr->calcPhi();
	///////////////////////////////////////////////////////////////
	/// SCALE STADIUM SURFACE TO VIEW--- REPLACE WITH CAMERA FIT FUNCTION
	for (int j = 0; j < 3; j++) {
		ntMatrix4 SC3 = ntMatrix4(panel_ptr->faces_G.at(0)->at(0).vecs[j]);
		SC3.scale3d(0.0001);
		panel_ptr->faces_G.at(0)->at(0).calcCentroid();	//REQUIRED AFTER SCALING
		panel_ptr->faces_G.at(0)->at(0).calcNorm();		//REQUIRED AFTER SCALING
	}
	///
	///////////////////////////////////////////////////////////////
	panel_ptr->sub_Div(gen);	    // SUBDIVIDE FOR GLOBAL DISPLAY

	///////////////////////////////////////////////////////////////
	///////////////////////////////////// CALCULATE PEFORATION GRID
	panel_ptr->plot_Perf(36, TRI);

	///////////////////////////////////////////////////////////////
	////////////////////////////////// LOAD TEXTURE MAP TO SURFACES
	if (isImgLoaded == true) {
		map_ImgCol(panel_ptr);
	}

	///////////////////////////////////////////////////////////////
	//////////////////////////////////// CALCULATE PERFORATION SIZE
	panel_ptr->add_Perf();
	panel_ptr->set_Graph();

	///////////////////////////////////////////////////////////////
	//////////////////////////////////////// GRAPH PERFORATION DATA
	for (int i = 0; i < panel_ptr->perf_size; i++) {
		/// pointer to matrices
		/// flatten matrix
		/// vector pushback process is too computationally intensive 
		p_Rad.push_back(panel_ptr->p_Rad.at(i));
	}

	log_Angles(panel_ptr);

	n_perf += panel_ptr->perf_size;
	areaS_gross += panel_ptr->get_Area();
	areaS_net += (panel_ptr->get_Area() - panel_ptr->perf_area);

	int val = stoi(panel_ptr->get_ID());

	//if (val < 100) {
	//	write_Panel_TXT(panel_ptr);
	//}
	//if (val < 1000) {
	//	write_Panel_IMG(panel_ptr);
	//	//write_Panel_TXT(panel_ptr);
	//}

	/// SCALE PANELS TO VIEW--- REPLACE WITH CAMERA FIT FUNCTION //
	/// TRANSLATE TO HUD LOCATION
	ntVec3 posXY = ntVec3(55, 640, 0);  /////// POSITION FOR 2D HUD
	//float sc_Factor = 0.015;
	float sc_Factor = 5;
	for (int j = 0; j < panel_ptr->vecs_SD.size(); j++) {
			ntMatrix4 SC1 = ntMatrix4(panel_ptr->vecs_SD[j]);
			SC1.scale3d(sc_Factor);
			SC1.translate(posXY);
	}

	for (int j = 0; j < panel_ptr->perfs.size(); j++) {
		for (int k = 0; k < panel_ptr->perfs.at(j)->seg; k++) {
			ntMatrix4 SC3 = ntMatrix4(panel_ptr->perfs.at(j)->vecs.at(k));
			SC3.scale3d(sc_Factor);
			SC3.translate(posXY);
		}
	}
	///
}
void ovisApp::fun02(int ind_S, int ind_E, std::vector<ntPanel*>* panels, int x){
	std::cout << "INSIDE THREAD:  " << x << endl;
	for (int i = ind_S; i <= ind_E; i++) {
		funct(panels->at(i));
	}
}
void ovisApp::write_Panels_TXT(int ind_S, int ind_E, std::vector<ntPanel*>* panels) {
	for (int i = ind_S; i <= ind_E; i++) {
		ntPanel* panel_ptr(panels->at(i));
		write_Panel_TXT(panel_ptr);
	}
}
void ovisApp::write_Panels_IMG(int ind_S, int ind_E, std::vector<ntPanel*>* panels) {
	for (int i = ind_S; i <= ind_E; i++) {
		ntPanel* panel_ptr(panels->at(i));
		write_Panel_IMG(panel_ptr);
	}
}

string ovisApp::format_STR(string line){
	string token = "PANEL";
	string norm = ": (";

	if (line.find(token) != string::npos) {
		char chars[] = "/PANELNORMUV:{";
		for (unsigned int j = 0; j < strlen(chars); ++j)
		{
			line.erase(std::remove(line.begin(), line.end(), chars[j]), line.end());
		}
		/////split stream at comma and insert into position array = posXYZ
		string str;
		stringstream stream(line);
		float val;
		int cnt = 0;

		while (getline(stream, str, ',')) {
			stringstream ss;
			ss.precision(5);
			ss << std::setw(18) << std::setfill(' ');
			ss << str;
			ss >> val;

			if (cnt < 2) {
				norm += ss.str() + ", ";
			}
			else if (cnt == 2) {
				norm += ss.str() + ")";
			}
			cnt += 1;
		}
		return norm;
	}
}
string ovisApp::format_VEC(ntVec3* vec) {
	string pos = "(";
	float val;

	for (int i = 0; i < 3; i++) {
		if (i == 0) {
			val = vec->x;
		}
		else if (i == 1) {
			val = vec->y;
		}
		else if (i == 2) {
			val = vec->z;
		}

		stringstream ss;
		ss.precision(10);
		ss << std::setw(18) << std::setfill(' ');
		ss << val;

		if (i < 2) {
			pos += ss.str() + ", ";
		}
		else if (i == 2) {
			pos += ss.str() + ")";
		}
	}
	return pos;
}
Vec3 ovisApp::add_VEC(string line) {
	float vertPos[3];

	char chars[] = "/POS:{}NRMAELUV";
	for (unsigned int j = 0; j < strlen(chars); ++j)
	{
		line.erase(std::remove(line.begin(), line.end(), chars[j]), line.end());
	}
	/////split stream at comma and insert into position array = posXYZ
	string str;
	stringstream stream(line);
	double val;
	int cnt = 0;

	while (getline(stream, str, ',')) {
		stringstream ss;
		ss << str;
		ss.precision(5);
		ss << std::setw(18) << std::setfill(' ');
		ss >> val;
		str = ss.str();
		
		if (cnt <2) {
			vertPos[cnt] = val;
		}
		else if (cnt == 2) {
			vertPos[cnt] = val;
		}
		cnt += 1;
	}
	ntVec3 vertex = ntVec3(vertPos[0], vertPos[1], vertPos[2]);
	return vertex;
}

void ovisApp::align_Panel(ntPanel* panel_ptr, Vec3* axis) {
	////////////////////////////////////////////////////////////////
	//////////////// FUNCTION ALIGNS PANEL FACE NORMAL TO TO WORLD Z
	using namespace arma;

	int rows = 3;
	int cols = 3;
	arma::mat R = zeros<mat>(rows, cols);

	panel_ptr->calcCentroid();
	//////////////////////////////////////////////////////////////// TRANSLATE PANEL CENTROID TO ORIGIN
	Vec3 trans_V = Vec3(panel_ptr->cent->x, panel_ptr->cent->y, panel_ptr->cent->z);			//TRANSLATION VECTOR

	for (int i = 0; i < rows; i++) {
		panel_ptr->vecs[i]->sub(&trans_V);
	}
	panel_ptr->calcNorm();

	//////////////////////////////////////////////////////////////// FIND PANEL NORMAL AND ALIGN TO WORLD Z-AXIS
	ntVec3 v = axis->cross(&panel_ptr->norm);		//axis of rotation							//v = cross(A, B);	
	float  c = axis->dot(&panel_ptr->norm);			//angle cos [rad]							//dot(A,B)
	float  s = v.mag();								//norm / vector length or rotation axis		//||v||
	arma::mat ssc = zeros<mat>(rows, cols);			//ssc = [0, -v(3), v(2); v(3), 0, -v(1); -v(2), v(1), 0];
	float phi = c *(180 / M_PI);					//CONVERT TO DEGREES
													//c = c *(180 / M_PI);							//CONVERT TO DEGREES

	ssc << 0 << -v.z << v.y << endr
		<< v.z << 0 << -v.x << endr
		<< -v.y << v.x << 0 << endr;

	//////////////////////////////////////////////////////////////// BUILD ROTATION MATRIX
	R.eye();
	R = R + ssc + (ssc * ssc) * (1 - c) / (s*s);	//R = eye(3) + ssc + ssc^2*(1-dot(A,B))/(norm(v))^2;
													//R = I + [v] + [v]^2((1-c)/s^2)																				

	//////////////////////////////////////////////////////////////// MULTIPLY VERTEX POSITONS BY ROTATION MATRIX
	rows = panel_ptr->verts.size();
	arma::mat vertex = zeros<mat>(rows, 1);
	for (int i = 0; i < rows; i++) {
		//ntMatrix4 mat1 = ntMatrix4(panel_ptr->vecs[i]);
		//mat1.rotate3d(c, v);						//TRIG FUNCTION METHOD USED TO VALIDATE RESULTS

		vertex(0, 0) = panel_ptr->vecs[i]->x;
		vertex(1, 0) = panel_ptr->vecs[i]->y;
		vertex(2, 0) = panel_ptr->vecs[i]->z;

		vertex = solve(R, vertex);					//arma::solve(A, B); | matlab- A\B or inv(A)*B

		panel_ptr->vecs[i]->x = vertex(0, 0);
		panel_ptr->vecs[i]->y = vertex(1, 0);
		panel_ptr->vecs[i]->z = vertex(2, 0);
	}
	round_Pos(panel_ptr, .001);
	panel_ptr->calcCentroid();
	panel_ptr->calcNorm();

	//http://math.stackexchange.com/questions/180418/calculate-rotation-matrix-to-align-vector-a-to-vector-b-in-3d
}
void ovisApp::align_Panel(ntPanel* panel_ptr, Vec3* axis_A, Vec3* axis_B, ntVec3* pos) {
	////////////////////////////////////////////////////////////////
	//////////////// FUNCTION ALIGNS PANEL FACE NORMAL TO TO VEC
	using namespace arma;
	axis_A->unitize();
	axis_B->unitize();

	int rows = 3;
	int cols = 3;
	arma::mat R = zeros<mat>(rows, cols);

	//////////////////////////////////////////////////////////////// TRANSLATE PANEL CENTROID TO ORIGIN
	Vec3 trans_V = Vec3(pos->x, pos->y, pos->z);			//TRANSLATION VECTOR

	for (int i = 0; i < rows; i++) {
		panel_ptr->vecs[i]->sub(&trans_V);
	}

	//////////////////////////////////////////////////////////////// FIND PANEL NORMAL AND ALIGN TO WORLD Z-AXIS
	ntVec3 v = axis_A->cross(axis_B);				//axis of rotation							//v = cross(A, B);	
	float  c = axis_A->dot(axis_B);					//angle cos [rad]							//dot(A,B)
	float  s = v.mag();								//norm / vector length or rotation axis		//||v||
	arma::mat ssc = zeros<mat>(rows, cols);			//ssc = [0, -v(3), v(2); v(3), 0, -v(1); -v(2), v(1), 0];

	ssc <<    0 << -v.z <<  v.y << endr
		<<  v.z <<    0 << -v.x << endr
		<< -v.y <<  v.x <<    0 << endr;

	////////////////////////////////////////////////////////////////  BUILD ROTATION MATRIX
	R.eye();
	R = R + ssc + (ssc * ssc) * (1 - c) / (s*s);	//R = eye(3) + ssc + ssc^2*(1-dot(A,B))/(norm(v))^2;
													//R = I + [v] + [v]^2((1-c)/s^2)																				

	//////////////////////////////////////////////////////////////// MULTIPLY VERTEX POSITONS BY ROTATION MATRIX
	rows = panel_ptr->verts.size();
	arma::mat vertex = zeros<mat>(rows, 1);
	for (int i = 0; i < rows; i++) {
		vertex(0, 0) = panel_ptr->vecs[i]->x;
		vertex(1, 0) = panel_ptr->vecs[i]->y;
		vertex(2, 0) = panel_ptr->vecs[i]->z;

		vertex = solve(R, vertex);					//arma::solve(A, B); | matlab- A\B or inv(A)*B

		panel_ptr->vecs[i]->x = vertex(0, 0);
		panel_ptr->vecs[i]->y = vertex(1, 0);
		panel_ptr->vecs[i]->z = vertex(2, 0);
	}

	round_Pos(panel_ptr, .001);
	panel_ptr->calcCentroid();
	panel_ptr->calcNorm();

	//http://math.stackexchange.com/questions/180418/calculate-rotation-matrix-to-align-vector-a-to-vector-b-in-3d
}
void ovisApp::round_Pos(ntPanel* panel_ptr, float tolerance) {
	float t = tolerance;
	for (int i = 0; i < 3; i++) {
		if (panel_ptr->vecs[i]->x < t && panel_ptr->vecs[i]->x > -t) {
			panel_ptr->vecs[i]->x = round(panel_ptr->vecs[i]->x);
		}

		if (panel_ptr->vecs[i]->y < t && panel_ptr->vecs[i]->y > -t) {
			panel_ptr->vecs[i]->y = round(panel_ptr->vecs[i]->y);
		}

		if (panel_ptr->vecs[i]->z < t && panel_ptr->vecs[i]->z > -t) {
			panel_ptr->vecs[i]->z = round(panel_ptr->vecs[i]->z);
		}
	}
}
void ovisApp::map_ImgCol(ntPanel* panel_ptr) {
	/// ////////////////////// MAP COLOR FROM UV LIST
	for (int i = 0; i < panel_ptr->p_UVs.size(); i++) {

		float x = panel_ptr->p_UVs.at(i)->x;
		float y = panel_ptr->p_UVs.at(i)->y;

		x = floor(mapRange(0, img_X, 0, 1, x));
		y = floor(mapRange(0, img_Y, 0, 1, y));

		float col = img_00(x, y);

		col = mapRange(0, 1, 0, 255, col);

		panel_ptr->p_Col.push_back(col);
	}
	/// ////////////////////// MAP COLOR TO VERTEX
	for (int i = 0; i < panel_ptr->faces_L.size(); i++) {
		for (int j = 0; j < panel_ptr->faces_L.at(i)->size(); j++) {
			for (int k = 0; k < 3; k++) {

				float x = panel_ptr->faces_L.at(i)->at(j).uvws[k]->x;
				float y = panel_ptr->faces_L.at(i)->at(j).uvws[k]->y;

				x = floor(mapRange(0, img_X, 0, 1, x));
				y = floor(mapRange(0, img_Y, 0, 1, y));

				float col = img_00(x, y);
				col = mapRange(0, 1, 0, 255, col);

				panel_ptr->faces_L.at(i)->at(j).verts.at(k)->setColor(ntCol4(col, col, col, 1));
				if (i < panel_ptr->faces_G.size()) {
					panel_ptr->faces_G.at(i)->at(j).verts.at(k)->setColor(ntCol4(col, col, col, 1));
				}
			}
		}
	}
	/// ////////////////////// CENTROID MAPPING
	//int dim = panel_ptr->vecs_UV.size();
	//int gen_ID = 0;
	//int face_ID = 0;
	//float col;

	//for (int j = 0; j < dim; j++) {

	//	float x = panel_ptr->vecs_UV.at(j).x;
	//	float y = panel_ptr->vecs_UV.at(j).y;

	//	x = floor(mapRange(0, img_X, 0, 1, x));
	//	y = floor(mapRange(0, img_Y, 0, 1, y));

	//	col = img_00(x, y);
	//	col = mapRange(0, 1, 0, 255, col);

	//	if (gen_ID < panel_ptr->faces_G.size()) {

	//		panel_ptr->faces_G.at(gen_ID)->at(face_ID).setColor(ntCol4(col, col, col, 1));
	//		panel_ptr->faces_L.at(gen_ID)->at(face_ID).setColor(ntCol4(col, col, col, 1));

	//		if (face_ID < (panel_ptr->faces_G.at(gen_ID)->size() - 1)) {
	//			face_ID++;
	//		}
	//		else {
	//			face_ID = 0;
	//			gen_ID++;
	//		}
	//	}
	/// ////////////////////// IMAGE TILE MAPPING
	//af::array img_Tile(img_X, img_Y);
	//img_Tile += img_IN;

	//if (img_T > 1) {
	//	for (int i = 0; i < img_T; i++) {
	//		af::flip(img_IN, 0);
	//		img_IN = af::join(0, img_Tile, img_IN);
	//	}
	//	for (int j = 0; j < img_T; j++) {
	//		af::flip(img_IN, 1);
	//		img_IN = af::join(1, img_Tile, img_IN);
	//	}
	//}

	//int step_U = 0;
	//int step_V = 0;
	//float inc_U = img_X / img_T;
	//float inc_V = img_Y / img_T;
	//int index = 0;
	//for (int i = 0; i < panel_ptr->faces_G.size(); i++) {
	//	for (int j = 0; j < panel_ptr->faces_G.at(i)->size(); j++) {
	//		for (int k = 0; k < 3; k++) {

	//			float x = panel_ptr->vecs_UV.at(index).x;
	//			float y = panel_ptr->vecs_UV.at(index).y;

	//			if (x > (img_X - (step_U * inc_U))) {
	//				step_U += 1;
	//			} else {
	//				step_U = 0;
	//			}
	//			if (y > (img_Y - (step_V * inc_V))) {
	//				step_V += 1;
	//			} else {
	//				step_V = 0;
	//			}

	//			x = (x * img_T) - (step_U * inc_U);
	//			y = (y * img_T) - (step_V * inc_V);

	//			x = floor(mapRange(0, img_X, 0, 1, x));
	//			y = floor(mapRange(0, img_Y, 0, 1, y));

	//			float col = img_00(x, y);
	//			col = mapRange(0, 1, 0, 255, col);

	//			index++;

	//			panel_ptr->faces_G.at(i)->at(j).verts.at(k)->setColor(ntCol4(col, col, col, 1));
	//			panel_ptr->faces_L.at(i)->at(j).verts.at(k)->setColor(ntCol4(col, col, col, 1));
	//		}
	//	}
	//}
}
void ovisApp::tile_Img(int U, int V, af::array img) {

	int dim_X = img_X;
	int dim_Y = img_Y;

	img_X = img_X * U;
	img_Y = img_Y * V;

	af::array img_Tile(img_X, img_Y);
	//img_Tile += img;

	//af::tile(img, U, V);
}
void ovisApp::log_Angles(ntPanel* panel_ptr) {
	float min = panel_ptr->get_AngleMin();
	float max = panel_ptr->get_AngleMax();

	if (min < phi_Min) {
		phi_Min = min;
	}
	if (max > phi_Max) {
		phi_Max = max;
	}
}
void ovisApp::run(){
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		if (gen_G >= 0) {
			gen = 0;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		if (gen_G >= 1) {
			gen = 1;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
		if (gen_G >= 2) {
			gen = 2;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
		if (gen_G >= 3) {
			gen = 3;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
		if (gen_G >= 4) {
			gen = 4;
		}
	}
	///////////////////////////////////////////////////////////////
	/////////////////////////////////////////  WORLD MAPPED DISPLAY
	Col4 colSel = Col4(1, 0.7, 0.9, 1);

	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
		if (panel_Index < panel_Dim-1) {
			float c = panels.at(panel_Index)->image_Val;
			panels.at(panel_Index)->faces_G.at(0)->at(0).setColor(Col4(c, c, c, 1));
			panel_Index = panel_Index + 1;
			std::cout << panel_Index << endl;
		}
		else {
			float c = panels.at(panel_Dim-1)->image_Val;
			panels.at(panel_Dim - 1)->faces_G.at(0)->at(0).setColor(Col4(c, c, c, 1));
			panel_Index = 0;
			std::cout << panel_Index << endl;
		}
		if (m == vW) {
			panels.at(panel_Index)->faces_G.at(0)->at(0).setColor(colSel);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
		if (panel_Index > 0 && panel_Index < panel_Dim) {
			float c = panels.at(panel_Index)->image_Val;
			panels.at(panel_Index)->faces_G.at(0)->at(0).setColor(Col4(c, c, c, 1));
			panel_Index = panel_Index - 1;
			std::cout << panel_Index << endl;
		}
		else {
			float c = panels.at(0)->image_Val;
			panels.at(0)->faces_G.at(0)->at(0).setColor(Col4(c, c, c, 1));
			panel_Index = panel_Dim-1;
			std::cout << panel_Index << endl;
		}
		if (m == vW || m == vA) {
			panels.at(panel_Index)->faces_G.at(0)->at(0).setColor(colSel);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		m = vQ;
		for (int i = 0; i < panels.size(); i++) {
			float r = mapRange(0, 1, 0, panels.size(), i);
			float b = mapRange(0, 1, 0, panels.size(), i, false);
			if (i % 2 == 0) {
				panels.at(i)->faces_G.at(0)->at(0).setColor(Col4(r, 0, b, 1));
			}
			else {
				panels.at(i)->faces_G.at(0)->at(0).setColor(Col4(r, 0, b, 1));
			}
			panels.at(i)->faces_G.at(0)->at(0).edges.at(0).setCol(ntColor4f(0, 0, 0, .5));
			panels.at(i)->faces_G.at(0)->at(0).edges.at(1).setCol(ntColor4f(0, 0, 0, .5));
			panels.at(i)->faces_G.at(0)->at(0).edges.at(2).setCol(ntColor4f(0, 0, 0, .5));
		}
	}
	///////////////////////////////////////////////////////////////
	/////////////////////////////////////////  IMAGE MAPPED DISPLAY
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		if (gen_L > gen) {
			m = vA;
			for (int i = 0; i < panels.size(); i++) {
				panels.at(i)->faces_G.at(0)->at(0).edges.at(0).setCol(ntColor4f(1, 1, 1, .5));
				panels.at(i)->faces_G.at(0)->at(0).edges.at(1).setCol(ntColor4f(1, 1, 1, .5));
				panels.at(i)->faces_G.at(0)->at(0).edges.at(2).setCol(ntColor4f(1, 1, 1, .5));
				if (i == panel_Index) {
					panels.at(i)->faces_G.at(0)->at(0).setColor(colSel);
				}
			}
		} else {
			std::cout << "NO EXPLICIT SUBDIVISION HAS BEEN SET" << endl;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		m = vS;
		for (int i = 0; i < panels.size(); i++) {
			float col = panels.at(i)->image_Val;
			panels.at(i)->faces_G.at(0)->at(0).setColor(ntColor4f(col, col, col, 1));
		}
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		m = vD;
		for (int i = 0; i < panels.size(); i++) {
			float col = panels.at(i)->image_Val;
			panels.at(i)->faces_G.at(0)->at(0).setColor(ntColor4f(col, col, col, 1));
		}
	}
	///////////////////////////////////////////////////////////////
	///////////////////////////////////////////  WIRE FRAME DISPLAY
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		m = vW;
		for (int i = 0; i < panels.size(); i++) {
			panels.at(i)->faces_G.at(0)->at(0).edges.at(0).setCol(ntColor4f(1, 1, 1, .5));
			panels.at(i)->faces_G.at(0)->at(0).edges.at(1).setCol(ntColor4f(1, 1, 1, .5));
			panels.at(i)->faces_G.at(0)->at(0).edges.at(2).setCol(ntColor4f(1, 1, 1, .5));
			if (i == panel_Index) {
				panels.at(i)->faces_G.at(0)->at(0).setColor(colSel);
			}
		}
	}

	///////////////////////////////////////////////////////////////
	/////////////////////////////////////  LOCAL FACE DISPLAY MODES
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		mode_L = vX;
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		mode_L = vC;
	}
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
		mode_L = vV;
	}
		display();
}

void ovisApp::display(){
	if (m == vQ) {
		for (int i = 0; i < panels.size(); i++) {
			panels.at(i)->faces_G.at(0)->at(0).display();
			panels.at(i)->faces_G.at(0)->at(0).edges.at(0).display();
		}
	}
	if (m == vS) {
		for (int i = 0; i < panels.size(); i++) {
			panels.at(i)->display_Face_G(gen);
		}
	}
	if (m == vD) {
		for (int i = 0; i < panels.size(); i++) {
			panels.at(i)->display_Face_G(gen);
		}
	}
	if (m == vW || m == vA) {
		for (int i = 0; i < panels.size(); i++) {
			panels.at(i)->faces_G.at(0)->at(0).edges.at(0).display();
			panels.at(i)->faces_G.at(0)->at(0).edges.at(1).display();
			panels.at(i)->faces_G.at(0)->at(0).edges.at(2).display();
		}
		panels.at(panel_Index)->faces_G.at(0)->at(0).display();
	}
	///////////////////////////////////////////////////////////////
	////////////////////////////////////////////  2D DISPLAY CONENT
	//view_Orth();  ///REFACTOR THIS FUNCTION TO BASE CONTENT CLASS
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	//glOrtho(0, appWidth, 0, appHeight, .1, 100);
	gluOrtho2D(0, appWidth, 0, appHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	///////////////////////////////////////////////////////////////
	if (panel_Index >= 0 && panel_Index < panels.size()) {
		if (m == vW || m == vQ) {
			panels.at(panel_Index)->display_Perf();
		}
		if (m == vA) {
			panels.at(panel_Index)->display_Face_L(mode_L, gen_L);
		}
		if (m == vD) {
			panels.at(panel_Index)->display_Face_L(mode_L, gen);
		}
		if (m == vS) {
			panels.at(panel_Index)->display_EdgeSd(gen);
			panels.at(panel_Index)->display_Perf();
		}
		panels.at(panel_Index)->display_Edge();
		panels.at(panel_Index)->display_Graph();
	}
	/// CREATE SEPARATE CLASS FOR IMAGE OBJECTS
	display_IMG();
	graph00.display();
	graph01.display();
}
void ovisApp::display_IMG() {
	int n_cols = img_2d.n_cols;
	int n_rows = img_2d.n_rows/2;
	int u_cols = img_T;
	int v_rows = img_T;

	float col;
	glPointSize(1);
	glBegin(GL_POINTS);
	for (int u = 0; u < u_cols; u++) {
		for (int v = 0; v < v_rows; v++) {
			for (int i = 0; i < n_cols; i++) {
				for (int j = 0; j < n_rows; j++) {
					col = img_2d(j, i);
					col = mapRange(0, 1, 0, 255, col);
					
					if (u != 0 || v != 0) {
						col *= 0.75;
					}
					glColor4f(col, col, col, col);
					glVertex3f((i + 5)+(u*n_cols), (j + 161)+(v*n_rows), 0);
				}
			}
		}
	}
	glEnd();
}