#include "ntTriSkin.h"

using namespace arma;


int ntTriSkin::gen = 1;

perf_Mode  ntTriSkin::perf_mode  = SUBD;
perf_Type  ntTriSkin::perf_type  = DOT;
perf_Style ntTriSkin::perf_style = perf_06;
grid_Type  ntTriSkin::grid_type  = TRI;
bool ntTriSkin::isImgLoaded	= false;
bool ntTriSkin::isImgMosaic = false;
string ntTriSkin::url_IMGs;
arma::mat ntTriSkin::img_00 = zeros<mat>(1, 1);

std::vector<float> ntTriSkin::p_Rad;
int	  ntTriSkin::n_perf			= 0;
float ntTriSkin::areaS_gross	= 0;
float ntTriSkin::areaS_net		= 0;
float ntTriSkin::areaP_Max		= 0;
float ntTriSkin::areaP_Min		= 99999999;
float ntTriSkin::phi_Min		= 180;
float ntTriSkin::phi_Max		= 0.0;
float ntTriSkin::perfSpacing	= 1.5;

clock_t ntTriSkin::t_CPU		= 0;
double ntTriSkin::t_transform	= 0;
double ntTriSkin::t_SD			= 0;
double ntTriSkin::t_SC2			= 0;
double ntTriSkin::t_perforate	= 0;
double ntTriSkin::t_calcArea	= 0;
double ntTriSkin::t_graphData	= 0;
double ntTriSkin::t_saveTXT		= 0;
double ntTriSkin::t_saveIMG		= 0;

ntTriSkin::ntTriSkin() {}
ntTriSkin::ntTriSkin(std::string url_TXT, std::string url_IMG, std::string obj_Name) :
	url_TXT(url_TXT), url_IMG(url_IMG), obj_Name(obj_Name) {

	isImgLoaded		= false;
	isImgMosaic		= false;
	url_IMGs		= "";
	img_00			= zeros<mat>(1, 1);

	p_Rad.clear();
	n_perf			= 0;
	areaS_gross		= 0;
	areaS_net		= 0;
	areaP_Max		= 0;
	areaP_Min		= 99999999;
	phi_Min			= 180;
	phi_Max			= 0.0;
	perfSpacing		= 1.5;

	t_CPU			= 0;
	t_transform		= 0;
	t_SD			= 0;
	t_SC2			= 0;
	t_perforate		= 0;
	t_calcArea		= 0;
	t_graphData		= 0;
	t_saveTXT		= 0;
	t_saveIMG		= 0;
}

void ntTriSkin::init() {
	if (isPathDefined == true) {
		///////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////// READ TEXT
		/// ///////////////////////////////////////////////////////////
		/// /////////////////////////////////////////// BEGIN TIMER- 00
		auto t0 = std::chrono::steady_clock::now();
		///
		string url;
		if (isTxtSeq == true) {
			for (int i = file_begin; i < file_end + 1; i++) {
				url = url_TXT + to_string(i) + ".txt";
				read_DATA(url);
				std::cout << url << endl;
			}
		}
		else {
			url = url_TXT + ".txt";
			read_DATA(url);
		}
		isTxtLoaded = true;
		///
		string time;
		auto t1 = std::chrono::steady_clock::now();
		time = format_SEC(t1 - t0);
		t_LoadPanels += time;
		/// ///////////////////////////////////////////// END TIMER- 00
		std::cout << "\n\n///////////////////////////////////////////////////////////////\n";
		std::cout << "READ TXT COMPLETE           -----------------------------------" << endl;
		panel_Dim = panels.size();
		std::cout << "TOTAL PANELS LOADED:        " << panel_Dim << "\n" << endl;
		///////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////// READ IMAGE
		/// ///////////////////////////////////////////////////////////
		/// /////////////////////////////////////////// BEGIN TIMER- 00
		t0 = std::chrono::steady_clock::now();
		///
		read_IMG();
		///
		t1 = std::chrono::steady_clock::now();
		time = format_SEC(t1 - t0);
		t_LoadImage += time;
		/// ///////////////////////////////////////////// END TIMER- 00
		std::cout << "READ IMG COMPLETE           -----------------------------------" << endl;
		std::cout << "IMAGE SIZE:                 " << img_X << " x " << img_Y << "\n" << endl;
		std::cout << "CALCULATING PANEL GEOMETRY  -----------------------------------" << endl;
		///////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////// MULTITHREAD BUILD FUNCTION
		/// START TOTAL PROCESSING TIME CLOCK
		auto t_eval = std::chrono::steady_clock::now();
		t0 = std::chrono::steady_clock::now();

		panel_Dim = panel_end - panel_begin;

		if (isMultiThread == true) {
				std::vector <std::future<bool>> threads;
				//if (panel_Dim > 1000) {
					float items = (panel_Dim / thread_Cnt) + 1;
					items = ceil(items);
					int index_S = 0;
					int index_E = 0;

					for (int i = 0; i < thread_Cnt; i++) {
						index_S = items * i;
						index_E = (items * i) + items - 1;
						threads.push_back(std::async(std::launch::async, set_MT, index_S, index_E, &panels, i));
					}
					std::cout << "USING " << threads.size() << " THREADS\n" << endl;
					for (int i = 0; i < threads.size(); i++) {
						bool ret = threads[i].get();
					}
				//} else {
					//for (int i = 0; i < panel_Dim; i++) {
						//ntPanel* panel_ptr(panels.at(i));
						//threads.push_back(std::async(std::launch::async, build_MT, panel_ptr));
					//}
				//}
		} else {
			for (int i = panel_begin; i < panel_Dim; i++) {
				ntPanel* panel_ptr(panels.at(i));
				//std::cout << panel_ptr->get_ID() << endl;
				funct(panel_ptr);
			}
		}
		t1 = std::chrono::steady_clock::now();
		time = format_SEC(t1 - t0);
		string eval = format_SEC(t1 - t_eval);
		double p_duration = duration_DBL(t1 - t_eval)/ panel_Dim;
		string p_time = format_SEC(p_duration);
		
		///////////////////////////////////////////////////////////////////////
		////////////////////////////////////// UPDATE STRING WITH TIMER VALUES
		t_Transform += format_SEC(t_transform);
		t_SubD		+= format_SEC(t_SD);
		t_Scale2d	+= format_SEC(t_SC2);
		t_Perforate += format_SEC(t_perforate);
		t_CalcArea  += format_SEC(t_calcArea);
		t_Graph		+= format_SEC(t_graphData);
		t_Process	+= time;
		t_TranPan	+= p_time;
		t_saveTxt	+= format_SEC(t_saveTXT);
		t_saveImage += format_SEC(t_saveIMG);

		gen_L = panels.at(0)->faces_L.size()-1;

		print_chronos();
		std::cout << "EVAL_CPU TIME  [SECONDS]:   CHRONOS " << eval << endl;
		std::cout << "EVAL_T / PANEL [SECONDS]:   CHRONOS " << p_time << "\n" << endl;

		std::cout << "SYSTEM STATISTICS           -----------------------------------" << endl;
		std::cout << "SUBDIVISION GENERATION:     " << panels.at(0)->cnt_SubDiv << endl;
		std::cout << "FACE VECTOR SIZE [GEN]:     " << panels.at(0)->faces_G.size() << endl;
		std::cout << "GLOBAL FACES:               " << panels.at(0)->faces_G.at(gen)->size() << endl;
		std::cout << "LOCAL FACES:                " << panels.at(0)->faces_L.at(gen_L)->size() << "\n" << endl;
		std::cout << "PANELS:                     " << panel_Dim << endl;
		std::cout << "SURFACE AREA GROSS:         " << areaS_gross / 144 << endl;
		std::cout << "SURFACE AREA NET:           " << areaS_net / 144 << endl;
		std::cout << "PERCENT OF OPENING:         " << 100 - (areaS_net / areaS_gross) * 100 << "\n" << endl;
		std::cout << "PERFORATIONS:               " << n_perf << "\n" << endl;

		std::cout << "MIN ANGLE:                  " << phi_Min << endl;
		std::cout << "MAX ANGLE:                  " << phi_Max << endl;
		std::cout << "DEVIATION:                  " << phi_Max - phi_Min << "\n" << endl;

		std::cout << "///////////////////////////////////////////////////////////////\n";
	
		///////////////////////////////////////////////////////////////////////
		/////////////////////////////////////// GRAPH PERFORATION | PANELS DATA
		int w = 448;
		int h = 40;
		ntVec3 *graphPos = new ntVec3(5, 460, 0);
		ntVec3 *graphDim = new ntVec3(w, h, 0);
		///////////////////////////////////////////////////////////////
		//////////////////////////////////////// GRAPH PERFORATION DATA
		for (int i = 0; i < panel_Dim; i++) {
			ntPanel* panel_ptr(panels.at(i));
			panel_ptr->set_Graph();
			for (int j = 0; j < panel_ptr->perf_size; j++) {
				/// POINTER TO MATRICES
				/// FLATTEN MATRIX
				/// VECTOR PUSHBACK PROCESS IS TOO COMPUTATIONALLY INTENSIVE 
				p_Rad.push_back(panel_ptr->p_Rad.at(j));
			}
		}

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
		
		init_SysData();
	}
	else {
		std::cout << "ERROR:  PATH TO WRITE FILES IS UNDEFINED" << endl;
	}
}

void ntTriSkin::init_SysData() {

	string pc_Id = getPC_Name();
	string cpu_Spec = getCPU();
	string memory_Size = getRAM();

	///////////////////////////////////////////////////////////////////////
	///////////////////////////////////// TYPOGRAPHIC CONTENT | PANELS DATA
	std::vector <string> lines;

	string del_01 = "/////////////////////////////////////////////";

	string str_01 = "SUBDIVISION GENERATION:       " + to_string(panels.at(0)->cnt_SubDiv);
	string str_02 = "FACE VECTOR SIZE [GEN]:       " + to_string(panels.at(0)->faces_G.size());
	string str_03 = "GLOBAL FACES:                 " + to_string(panels.at(0)->faces_G.at(gen)->size());
	string str_04 = "LOCAL FACES:                  " + to_string(panels.at(0)->faces_L.at(gen_L)->size());
	string str_05 = "PANELS:                       " + to_string(panel_Dim);
	string str_06 = "SURFACE AREA GROSS:           " + to_string(areaS_gross / 144);
	string str_07 = "SURFACE AREA NET:             " + to_string(areaS_net / 144);
	string str_08 = "PERCENT OF OPENING:           " + to_string(100 - (areaS_net / areaS_gross) * 100);
	string str_09 = "PERFORATIONS:                 " + to_string(n_perf);

	string str_10 = "MIN ANGLE:                    " + to_string(phi_Min);
	string str_11 = "MAX ANGLE:                    " + to_string(phi_Max);
	string str_12 = "DEVIATION:                    " + to_string(phi_Max - phi_Min);

	string str_14 = "ENLARGED PANEL:  ";
	string str_15 = "SYSTEM PERFORATION STATS: ";
	string str_16 = "PANEL PERFORATION STATS:  ";
	string str_17 = "STADIUM SURFACE VIEW:  ";
	string str_18 = "MAPPING SOURCE IMAGE:  ";
	string str_19 = "PERFORATION PARAMETERS:";

	lines.push_back(del_01);
	lines.push_back("SYSTEM PERFORMANCE            ---------------");
	lines.push_back("WORKSTATION NAME:             " + pc_Id);
	lines.push_back("CPU SPECIFICATION:            " + cpu_Spec);
	lines.push_back("MEMORY:                       " + memory_Size + "   GB");
	if (isMultiThread == true) {
		lines.push_back("ALLOCATED THREADS:        " + to_string(thread_Cnt));
	}
	lines.push_back("");
	lines.push_back(t_LoadPanels);
	lines.push_back(t_LoadImage);
	if (isMultiThread == false){
		lines.push_back("");
		lines.push_back(t_SubD);
		lines.push_back(t_Perforate);
		lines.push_back(t_Transform);
		lines.push_back(t_CalcArea);
		lines.push_back(t_Scale2d);
		lines.push_back(t_Graph);
	}
	lines.push_back(t_saveTxt);
	lines.push_back(t_saveImage);
	lines.push_back("");
	lines.push_back(t_Process);
	lines.push_back(t_TranPan);
	lines.push_back("");

	lines.push_back("");
	lines.push_back(del_01);
	lines.push_back("SYSTEM STATISTICS             ---------------");
	lines.push_back("");
	lines.push_back(str_01);
	lines.push_back(str_02);
	lines.push_back(str_03);
	lines.push_back(str_04);
	lines.push_back(str_05);
	lines.push_back(str_06);
	lines.push_back(str_07);
	lines.push_back(str_08);
	lines.push_back(str_09);
	lines.push_back(str_10);
	lines.push_back(str_11);
	lines.push_back(str_12);

	write_SysData(lines);
	/*
	int sz = 24;
	font = "mplus-1m-regular";
	ntVec3* pt1 = new ntVec3(5, 570, 0);
	ntColor4f col = ntColor4f(0.25, 0.25, 0.25);

	title = ntType(pt1, sz, "AGENCYR", "OVIS STADIUM");

	sz = 16;
	ntVec3 strPos = ntVec3(1550, 125, 0);
	///SYSTEM STATS
	t_00 = ntType(new ntVec3(strPos.x, strPos.y + 900, 0), sz, font, str_00, col);
	t_01 = ntType(new ntVec3(strPos.x, strPos.y + 875, 0), sz, font, str_01, col);
	t_02 = ntType(new ntVec3(strPos.x, strPos.y + 850, 0), sz, font, str_02, col);
	t_03 = ntType(new ntVec3(strPos.x, strPos.y + 825, 0), sz, font, str_03, col);
	t_04 = ntType(new ntVec3(strPos.x, strPos.y + 800, 0), sz, font, str_04, col);

	t_05 = ntType(new ntVec3(strPos.x, strPos.y + 750, 0), sz, font, str_05, col);
	t_06 = ntType(new ntVec3(strPos.x, strPos.y + 725, 0), sz, font, str_06, col);
	t_07 = ntType(new ntVec3(strPos.x, strPos.y + 700, 0), sz, font, str_07, col);
	t_08 = ntType(new ntVec3(strPos.x, strPos.y + 675, 0), sz, font, str_08, col);
	t_09 = ntType(new ntVec3(strPos.x, strPos.y + 650, 0), sz, font, str_09, col);
	t_10 = ntType(new ntVec3(strPos.x, strPos.y + 625, 0), sz, font, str_10, col);
	t_11 = ntType(new ntVec3(strPos.x, strPos.y + 600, 0), sz, font, str_11, col);
	t_12 = ntType(new ntVec3(strPos.x, strPos.y + 575, 0), sz, font, str_12, col);

	///DELIMITER
	t_13 = ntType(new ntVec3(strPos.x, strPos.y + 550, 0), sz, font, del_01, col);

	///VIEWPORT HEADER
	t_14 = ntType(new ntVec3(  5, strPos.y + 900, 0), sz, font, str_14, col);
	t_15 = ntType(new ntVec3(  5, 535           , 0), sz, font, str_15, col);
	t_16 = ntType(new ntVec3(  5, 435           , 0), sz, font, str_16, col);
	t_17 = ntType(new ntVec3(485, strPos.y + 900, 0), sz, font, str_17, col);
	t_18 = ntType(new ntVec3(  5, 395           , 0), sz, font, str_18, col);
	t_19 = ntType(new ntVec3(  5,  25           , 0), sz, font, str_19, col);
	*/
}
void ntTriSkin::read_DATA(string url){
	ifstream file(url);
	string line;

	isStartFile = false;
	isEndSubs = false;
	isSubNext = false;
	isEndFile = false;

	int cnt = 0;
	std::vector<ntVec3*> params_UV;

	while (std::getline(file, line) && isEndSubs == false) {
		if (line.find("BEGIN") != string::npos) {
			isStartFile = true;
		}
		if (line.find("NODE") != string::npos) {
			isNode   =		true;
			isCorner =		false;
			isFastener =	false;
			cnt = 0;
		}
		if (line.find("VERT") != string::npos) {
			isNode   =		false;
			isCorner =		true;
			isFastener =	false;
			cnt = 0;
		}
		if (line.find("FAST") != string::npos) {
			isNode   =		false;
			isCorner =		false;
			isFastener =	true;
			cnt = 0;
		}
		///////////////////////////////////////////////////////////////
		// SET PANEL NAME
		if (isStartFile == true && line.find("PAN:") != string::npos) {
			char chars[] = " ";
			for (unsigned int j = 0; j < strlen(chars); ++j)
			{
				line.erase(std::remove(line.begin(), line.end(), chars[j]), line.end());
			}
			size_t token = line.find_last_of(":");
			panel_ID = line.substr(token + 1);
		}
		
		///////////////////////////////////////////////////////////////
		// SET PANEL NORMAL
		if (isStartFile == true && line.find("NRM:") != string::npos) {
			line = format_STR(line);
			panel_Norm = line + "\n";
		}
		///////////////////////////////////////////////////////////////
		// SET UV PARAMETERS
		if (isStartFile == true && line.find("UV:") != string::npos) {
			///EXCEPTION FOR > 3 UVW ENTRIES
			ntVec3 temp = add_VEC(line);  /// CREATE POINTER TO VEC
			ntVec3 * uvw = new ntVec3(temp.x, temp.y, temp.z);
			params_UV.push_back(uvw);

			line = format_STR(line);
			panel_UVW = line + "\n";
		}
		///////////////////////////////////////////////////////////////
		// SET DIRECTION
		if (isStartFile == true && line.find("DIR:") != string::npos) {
			size_t token = line.find_last_of(":");
			panel_Dir = line.substr(token + 1);
		}
		// SET WEIGHT FACTOR
		if (isStartFile == true && line.find("WHT:") != string::npos) {
			size_t token = line.find_last_of(":");
			panel_Weight = line.substr(token + 1);
		}
		// SET REGION
		if (isStartFile == true && line.find("REG:") != string::npos) {
			size_t token = line.find_last_of(":");
			panel_Region = line.substr(token + 1);
		}
		///////////////////////////////////////////////////////////////
		// SET NODE POSITIONS
		if (isStartFile == true && line.find("POS:") != string::npos && isNode == true) {
			ntVec3 vec = add_VEC(line);
			verts[cnt] = vec;
			if (cnt < 2) {
				cnt = cnt + 1;
			}
			else {
				cnt = 0;
			}
		}
		///////////////////////////////////////////////////////////////
		// SET CORNER POSITONS
		if (isStartFile == true && line.find("POS:") != string::npos && isCorner == true) {
			ntVec3 vec = add_VEC(line);
			cpts[cnt] = vec;
			if (cnt < 5) {
				cnt = cnt + 1;
			}
		}
		///////////////////////////////////////////////////////////////
		// SET FASTENER CENTERLINE POSITIONS
		if (isStartFile == true && line.find("POS:") != string::npos && isFastener == true) {
			ntVec3 vec = add_VEC(line);
			fpts[cnt] = vec;
			if (cnt < 5) {
				cnt = cnt + 1;
			}
		}
		///////////////////////////////////////////////////////////////
		// END PANEL MOVE TO NEXT
		if (line.find("NEXT") != string::npos) {
			isSubNext = true;
		}
		else { isSubNext = false; }
		///////////////////////////////////////////////////////////////
		// END FILE
		if (line.find("END") != string::npos) {
			isEndSubs = true;
			isEndFile = true;
		} else { isEndFile = false; }

		///////////////////////////////////////////////////////////////
		//DISTRIBUTE DATA TO PANEL DEFINITION
		if (isSubNext == true || isEndFile == true) {
			
			// INITIALIZE GLOBAL NODE POSITIONS
			ntVec3 * v0 = new ntVec3(verts[0].x, verts[0].y, verts[0].z);
			ntVec3 * v1 = new ntVec3(verts[1].x, verts[1].y, verts[1].z);
			ntVec3 * v2 = new ntVec3(verts[2].x, verts[2].y, verts[2].z);
			ntPanel * panel = new ntPanel(v0, v1, v2);

			// INITIALIZE GLOBAL CORNER POINTS
			ntVec3 * c0 = new ntVec3(cpts[0].x, cpts[0].y, cpts[0].z);
			ntVec3 * c1 = new ntVec3(cpts[1].x, cpts[1].y, cpts[1].z);
			ntVec3 * c2 = new ntVec3(cpts[2].x, cpts[2].y, cpts[2].z);
			ntVec3 * c3 = new ntVec3(cpts[3].x, cpts[3].y, cpts[3].z);
			ntVec3 * c4 = new ntVec3(cpts[4].x, cpts[4].y, cpts[4].z);

			panel->set_cG(c0, 3);
			panel->set_cG(c1, 4);
			panel->set_cG(c2, 5);
			panel->set_cG(c3, 6);
			panel->set_cG(c4, 7);

			// INITIALIZE GLOBAL FASTENER CL POINTS
			ntVec3 * f0 = new ntVec3(fpts[0].x, fpts[0].y, fpts[0].z);
			ntVec3 * f1 = new ntVec3(fpts[1].x, fpts[1].y, fpts[1].z);
			ntVec3 * f2 = new ntVec3(fpts[2].x, fpts[2].y, fpts[2].z);
			ntVec3 * f3 = new ntVec3(fpts[3].x, fpts[3].y, fpts[3].z);
			ntVec3 * f4 = new ntVec3(fpts[4].x, fpts[4].y, fpts[4].z);

			panel->set_fG(f0, 8);
			panel->set_fG(f1, 9);
			panel->set_fG(f2, 10);
			panel->set_fG(f3, 11);
			panel->set_fG(f4, 12);

			// INITIALIZE PANEL PARAMS
			panel->set_ID(panel_ID);
			panel->set_nG(panel_Norm);
			panel->set_UVW(panel_UVW);			// STRNG UVW
			panel->set_UVW(params_UV);			// VEC3  UVW
			panel->set_Dir(panel_Dir);			// STRNG DIR
			panel->set_Weight(panel_Weight);
			panel->set_Region(panel_Region);
			panel->set_Edges();

			panels.push_back(panel);

			panel_ID		=	"<< ERROR >>";
			panel_Norm		=	"<< ERROR >>";
			panel_UVW		=	"<< ERROR >>";
			panel_Dir		=    "<< ERROR >>";
			panel_Weight	=   "0";
			panel_Region	=   "0";
			panel_Vert		=	"";
			vector<ntVec3*>().swap(params_UV);		//DEALLOCATE MEMORY STORED IN PARAMS_UV
		}
	}
	//isTxtLoaded = true;
}
void ntTriSkin::read_IMG() {
	
	///////////////////////////////////////////////////////////////
	////////////////////////////////////////// IMAGE FOR 2D DISPLAY
	string url = url_IMG + ".jpg";
	img_SRC = ntImage(url);
	int width = 448;
	img_SRC.set_Pos(5, 161);
	img_SRC.set_Dim(width, 222);
	///////////////////////////////////////////////////////////////
	/////////////////////////////////// IMAGE FOR PERF CALCULATIONS
	const char * file = url.c_str();
	img_IN = af::loadImage(file, false); //img_SRC.img_2D;//

	img_X = img_IN.dims(1);
	img_Y = img_IN.dims(0);

	//img_00 = zeros<mat>(img_IN.dims(0), img_IN.dims(1));
	img_00.resize(img_IN.dims(0), img_IN.dims(1));
	af::array img_LOADER(img_IN.dims(0), img_IN.dims(1), img_00.memptr());
	img_LOADER += img_IN;
	
	img_LOADER.host((void*)img_00.memptr());
	img_00 = arma::flipud(img_00);

	isImgLoaded = true;
}
void ntTriSkin::read_IMG(string url) {
	///////////////////////////////////////////////////////////////
	/////////////////////////////////// IMAGE FOR PERF CALCULATIONS
	const char * file = url.c_str();
	af::array img_in = af::loadImage(file, false);

	int dim_x = img_in.dims(1);
	int dim_y = img_in.dims(0);

	arma::mat img = zeros<mat>(dim_y, dim_x);
	af::array img_LOADER(dim_y, dim_x, img.memptr());
	img_LOADER += img_in;

	img_LOADER.host((void*)img.memptr());
	img = arma::flipud(img);
}

void ntTriSkin::write_Panel_TXT(ntPanel* panel_ptr) {
	stringstream ss;
	ss << std::setw(5) << std::setfill('0');
	ss << panel_ptr->get_ID();

	string pathExtension = "txt\\";
	string fileName = ss.str();
	string fileExt = ".txt";
	string url = path_OUT + pathExtension + fileName + fileExt;

	///////////////////////////////////////////////////// SET TIME STAMP
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
	file << "//  PANEL INDEX NO:  " << fileName << "\n";
	file << "//  NODE POSITIONS:\n";
	file << "//  [ WORLD COORDINATES ]\n";
	file << "//        (                 X,                  Y,                  Z)\n";
	file << "     POS: " << format_VEC(panel_ptr->get_v_G().at(0)) << "\n";
	file << "     POS: " << format_VEC(panel_ptr->get_v_G().at(1)) << "\n";
	file << "     POS: " << format_VEC(panel_ptr->get_v_G().at(2)) << "\n";
	file << "\n";
	file << "//  CORNER POSITIONS:           {POS:            }\n";
	file << "     POS: " << format_VEC(panel_ptr->get_c_G().at(0)) << "\n";
	file << "     POS: " << format_VEC(panel_ptr->get_c_G().at(1)) << "\n";
	file << "     POS: " << format_VEC(panel_ptr->get_c_G().at(2)) << "\n";
	file << "     POS: " << format_VEC(panel_ptr->get_c_G().at(3)) << "\n";
	file << "     POS: " << format_VEC(panel_ptr->get_c_G().at(4)) << "\n";
	file << "\n";
	file << "//  PANEL ORIENTATION VECTOR:\n";
	file << "//   VEC" << panel_ptr->get_n_G();
	file << "//   UVW" << panel_ptr->get_UVW();
	file << "//   DIR:" << panel_ptr->get_Dir() << "\n";
	file << "//   WHT: " << panel_ptr->get_Weight() << "\n";
	file << "//   REG: " << panel_ptr->get_Region() << "\n\n";
	file << "//  AREA GROSS:   " << to_string(panel_ptr->get_Area()) << "          [ SQ.INCH ]\n";
	file << "//  AREA NET:     " << to_string(panel_ptr->get_Area() - panel_ptr->perf_area) << "          [ SQ.INCH ]\n";
	file << "//  OPACITY:      " << to_string(panel_ptr->perf_perc) << "                  [ % ]\n\n";
	
	stringstream a0, a1, a2;
	a0 << std::setw(8) << std::setfill('_');
	a1 << std::setw(8) << std::setfill('_');
	a2 << std::setw(8) << std::setfill('_');

	a0 << panel_ptr->phi[0];
	a1 << panel_ptr->phi[1];
	a2 << panel_ptr->phi[2];

	file << "//  A0:          " << a0.str() << "              [ DEGREES ]\n";
	file << "//  A1:          " << a1.str() << "              [ DEGREES ]\n";
	file << "//  A2:          " << a2.str() << "              [ DEGREES ]\n";
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
	file << "//  CORNER POSITIONS:           {POS:            }\n";
	file << "     POS: " << format_VEC(panel_ptr->get_c_L().at(0)) << "\n";
	file << "     POS: " << format_VEC(panel_ptr->get_c_L().at(1)) << "\n";
	file << "     POS: " << format_VEC(panel_ptr->get_c_L().at(2)) << "\n";
	///file << "     POS: " << format_VEC(panel_ptr->get_c_L().at(3)) << "\n";
	///file << "     POS: " << format_VEC(panel_ptr->get_c_L().at(4)) << "\n";
	file << "\n";
	file << "//  FASTENER CENTERLINE POINTS: {POS:            }\n";
	file << "     POS: " << format_VEC(panel_ptr->get_f_L().at(0)) << "\n";
	file << "     POS: " << format_VEC(panel_ptr->get_f_L().at(1)) << "\n";
	file << "     POS: " << format_VEC(panel_ptr->get_f_L().at(2)) << "\n";
	///file << "     POS: " << format_VEC(panel_ptr->get_f_L().at(3)) << "\n";
	///file << "     POS: " << format_VEC(panel_ptr->get_f_L().at(4)) << "\n";
	file << "\n";
	file << "//  FASTENER POSITIONS:         {POS:            }\n";
	for (int i = 0; i < panel_ptr->get_Fast().size(); i++) {
		file << "     POS: " << format_VEC(panel_ptr->get_Fast().at(i)) << "\n";

	}
	file << "\n";

	int perf_cnt = panel_ptr->get_Perf().size();
	int poly_cnt = panel_ptr->get_PLin().size();
	int p_dim;

	if (perf_type == DOT) {
		p_dim = perf_cnt;
		file << "//  PERFORATION DATA:           {POS:  DIAMETER: }\n";
		file << "//  PERFORATION COUNT:  " << to_string(p_dim) << "\n\n";
		for (int i = 0; i < perf_cnt; i++) {
			float radius = panel_ptr->get_Perf_R().at(i) * 2;
			if (radius > 0) {
				file << "     POS:  " << format_VEC(panel_ptr->get_Perf().at(i)) << "       DIA:  " << radius << "\n";
			}
		}
	}	
	if (perf_type == TRICELL) {
		p_dim = poly_cnt;
		file << "//  PERFORATION DATA:           {POS:            }\n";
		file << "//  TRICELL COUNT:      " << to_string(p_dim) << "\n\n";
		for (int i = 0; i < poly_cnt; i++) {
			int pt_cnt = panel_ptr->get_PLin().at(i)->get_Vecs().size();
			for (int j = 0; j < pt_cnt; j++) {
				ntVec3* v = panel_ptr->get_PLin().at(i)->get_Vecs().at(j);
				file << "     POS: " << format_VEC(v) << "\n";
			}
			file << "//  CLOSE  " << "\n";
		}
	}
	file << "//  END FILE\n";
	file.close();

	std::cout << "PANEL:: " + ss.str() << " TXT SAVED" << endl;
}
void ntTriSkin::write_Panel_IMG(ntPanel* panel_ptr) {
	stringstream ss;
	ss << std::setw(6) << std::setfill('0');
	ss << panel_ptr->get_ID();

	string pathExtension = "\\img\\";
	string fileName = ss.str();
	string fileExt = ".jpg";
	string url = path_OUT + pathExtension + fileName + fileExt;
	const char * file = url.c_str();

	// MAXIMUM PANEL SIZE;
	int pX_max = 195;
	int pY_max = 45;

	// IMAGE SIZE
	bool isDPI = false;
	//isDPI = true;
	int dpi = 12;
	int img_x = 3400;
	int img_y = mapRange(0, pY_max, 0, pX_max, img_x);

	// MINIMIZE SEARCH AREA | PIXEL RANGE LOOP
	int dim_X = mapRange(0, img_x, 0, pX_max, panel_ptr->v1->x); //img_X;// OR = pX_max * dpi
	int dim_Y = mapRange(0, img_y, 0, pY_max, panel_ptr->v2->y); //img_Y;// OR = pY_max * dpi

	bool isPtAlpha = true;
	bool isPtPanel = false;
	bool isPtPerf  = false;
	
	// CATCH EXCEPTIONS BEYOND IMAGE SIZE
	if (panel_ptr->v1->x > pX_max) {
		std::cout << "PANEL:: " << ss.str() << " X DIM IS LARGER THAN IMAGE SIZE" << endl;
		isExceed = true;
	} else {
		isExceed = false;
	}
	if (panel_ptr->v2->y > pY_max) {
		std::cout << "PANEL:: " << ss.str() << " Y DIM IS LARGER THAN IMAGE SIZE" << endl;
		isExceed = true;
	} else {
		isExceed = false;
	}
	
	if (isExceed == false) {	
		// OPTION TO SET IMAGE SIZE BY DPI OR EXPLCIT SIZE
		if (isDPI == true) {
			img_x = pX_max * dpi;
			img_y = pY_max * dpi;
		}
		// IMAGE ARRAY
		arma::mat img_OUT = zeros<mat>(img_y, img_x);

		for (int i = 0; i < dim_Y; i++) {
			for (int j = 0; j < dim_X; j++) {

				// MAP (i, j) TO PANEL LOCAL COORDINATES
				float pos_X = mapRange(0, pX_max, 0, img_x, j);
				float pos_Y = mapRange(0, pY_max, 0, img_y, i);
				ntVec3 * pt = new ntVec3(pos_X, pos_Y, 0);

				// EVALUATE WHETHER PIXEL IS WIHIN PANEL
				isPtPanel = panel_ptr->pt_isInside(pt);
				// EVALUATE WHETHER PIXEL IS WIHIN PERFORATION
				if (isPtPanel == true) {
					isPtAlpha = false;
					for (int k = 0; k < panel_ptr->perfs.size(); k++) {
						isPtPerf = panel_ptr->perfs.at(k)->pt_isInside(pt);
						if (isPtPerf == true) {
							isPtAlpha = true;
							k = panel_ptr->perfs.size();
						}
					}
				} else {
					isPtAlpha = true;
				}
				// SET PIXEL TO OPAQUE
				if (isPtAlpha == false) {
					img_OUT(i, j) = 255;
				}
			}
		}
		// RETURN IIMAGE TO HOST AND SAVE
		af::array img_AF(img_y, img_x, img_OUT.memptr());
		img_AF = af::flip(img_AF, 0);
		af::saveImage(file, img_AF);
		std::cout << "PANEL:: " + ss.str() << " IMG SAVED\n" << endl;
	}
}
void ntTriSkin::write_SysData(std::vector<string> lines) {


	///////////////////////////////////////////////////////////////
	//////////////////////////////////////////////// SET TIME STAMP
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

	///////////////////////////////////////////////////////////////
	///////////////////////////////////////////////// SET FILE NAME
	stringstream ss;
	ss << obj_Name;
	ss << "_TRI-SKIN DATA_";
	ss << Y << M << D;

	string pathExtension = "txt\\";
	string fileName = ss.str();
	string fileExt = ".txt";
	int ind = 0;
	string fileIndex = "_" + format_INT(ind,2);
	string url = path_OUT + pathExtension + fileName + fileIndex + fileExt;

	///////////////////////////////////////////////////////////////
	//////////////////////////////////////////// WRITE FILE CONTENT
	while (isFile(url)) {
		std::cout << "FILE EXIST: " << fileIndex << endl;
		ind += 1;
		fileIndex = "_" + format_INT(ind, 2);
		url = path_OUT + pathExtension + fileName + fileIndex + fileExt;
	}

	ofstream file(url);

	file << "//  HKS | LINE\n";
	file << "//  00603.000::  " << obj_Name << " TRI-SKIN PROTOTYPE\n";
	file << "//  PANEL PARAMETERS  | UNITS [ INCHES ]\n";
	file << "//  RELEASE " << date << "\n";

	int cnt = lines.size();
	for (int i = 0; i < cnt; i++) {
		file << "\n" << lines.at(i);
	}
	file.close();

	std::cout << "TRI-SKIN:: " + ss.str() << fileIndex << " TXT SAVED" << endl;
}
void ntTriSkin::setPathOut(std::string path) {
	path_OUT = path;
	isPathDefined = true;
}
void ntTriSkin::set_FileCnt(int begin, int end) {
	isTxtSeq = true;
	file_begin = begin;
	if (end == 0) {
		file_end = begin;
	}
	else {
		file_end = end;
	}
}
void ntTriSkin::set_PanelRange(int begin, int end)
{
	panel_begin = begin;
	panel_end = end;
}
///////////////////////////////////////////////////////////////
bool ntTriSkin::set_MT(int ind_S, int ind_E, std::vector<ntPanel*>* panels, int index) {
	for (int i = ind_S; i <= ind_E; i++) {
		if (i < panels->size()) {
			build_MT(panels->at(i));
		}
	}
	return true;
}
bool ntTriSkin::build_MT(ntPanel* panel_ptr) {
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

	panel_ptr->calcArea();
	panel_ptr->calcPhi();
	float areaP = panel_ptr->get_Area();

	///////////////////////////////////////////////////////////////
	panel_ptr->sub_Div(gen);	    // SUBDIVIDE FOR GLOBAL DISPLAY
									// REVISE PER FASTENER GRID !!!
									// !!!!!!!!!!!!!!!!!!!!!!!!!!!!
	///////////////////////////////////////////////////////////////
	///////////////////////////////////// CALCULATE PEFORATION GRID
	int div = floor(panel_ptr->get_EdgeMin() / perfSpacing);
	panel_ptr->plot_Perf(div, grid_type, perf_type);
	panel_ptr->set_PerfStyle(perf_style);

	///////////////////////////////////////////////////////////////
	////////////////////////////////// LOAD TEXTURE MAP TO SURFACES
	if (isImgLoaded == true) {
		map_ImgCol(panel_ptr, &img_00);
	}
	///////////////////////////////////////////////////////////////
	///////////////////////////////////// EVALUATE MEAN VALUE INDEX
	//if (isImgMosaic == true) {
	//	float mean = panel_ptr->get_MeanVal();
	//	int index = ceil(mapRange(0, 255, 0, 1, mean));
	//	string url = url_IMGs + to_string(index) + ".jpg";
	//	ntImage image(url);
	//	arma::fmat* matrix = image.getMatrix();
	//	//*matrix = 255 - *matrix;
	//	read_IMG(url);
	//	panel_ptr->reparam_UV();
	//	map_ImgCol(panel_ptr, matrix);
	//}
	///////////////////////////////////////////////////////////////
	//////////////////////////////////// CALCULATE PERFORATION SIZE
	panel_ptr->add_Perf();
	///panel_ptr->set_Graph();

	log_Angles(panel_ptr);

	n_perf += panel_ptr->perfs.size();
	areaS_gross += areaP;
	areaS_net += (areaP - panel_ptr->perf_area);

	if (areaP < areaP_Min) {
		areaP_Min = areaP;
	}
	if (areaP > areaP_Max) {
		areaP_Max = areaP;
	}
	///////////////////////////////////////////////////////////////
	////////////////////////////////////////// SAVE PANEL DATA DATA
	//if (doSaveTXT == true) {
	//	write_Panel_TXT(panel_ptr);
	//}
	//if (doSaveIMG == true) {
	//	write_Panel_IMG(panel_ptr);
	//}
	///////////////////////////////////////////////////////////////
	set_Scale2D(panel_ptr, 10);
	return true;
}
/*
bool ntTriSkin::MT_01(int ind_S, int ind_E, std::vector<ntPanel*>* panels){
	for (int i = ind_S; i <= ind_E; i++) {
		if (i < panels->size()) {
			pan_2D(panels->at(i));
		}
	}
	return true;
}
bool ntTriSkin::MT_02(int ind_S, int ind_E, std::vector<ntPanel*>* panels){
	for (int i = ind_S; i <= ind_E; i++) {
		if (i < panels->size()) {
			pan_SD(panels->at(i));
		}
	}
	return true;
}
bool ntTriSkin::MT_03(int ind_S, int ind_E, std::vector<ntPanel*>* panels){
	for (int i = ind_S; i <= ind_E; i++) {
		if (i < panels->size()) {
			pan_P0(panels->at(i));
		}
	}
	return true;
}
bool ntTriSkin::MT_04(int ind_S, int ind_E, std::vector<ntPanel*>* panels){
	for (int i = ind_S; i <= ind_E; i++) {
		if (i < panels->size()) {
			pan_F0(panels->at(i));
		}
	}
	return true;
}

bool ntTriSkin::pan_2D(ntPanel* panel_ptr){
	///clock_t t = clock();
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
	///t_transform += (clock() - t);
	///t = clock();
	///
	return true;
}
bool ntTriSkin::pan_SD(ntPanel* panel_ptr){
	///clock_t t = clock();
	///////////////////////////////////////////////////////////////
	panel_ptr->sub_Div(gen);	    // SUBDIVIDE FOR GLOBAL DISPLAY
									// REVISE PER FASTENER GRID !!!
									// !!!!!!!!!!!!!!!!!!!!!!!!!!!!

	///////////////////////////////////////////////////////////////
	///////////////////////////////////// CALCULATE PEFORATION GRID
	int div = floor(panel_ptr->get_EdgeMin() / perfSpacing);
	panel_ptr->plot_Perf(div, grid_type, perf_type);
	panel_ptr->set_PerfStyle(perf_style);
	///t_SD += (clock() - t);
	///t = clock();
	///
	return true;
}
bool ntTriSkin::pan_P0(ntPanel* panel_ptr){
	///clock_t t = clock();
	///////////////////////////////////////////////////////////////
	////////////////////////////////// LOAD TEXTURE MAP TO SURFACES
	if (isImgLoaded == true) {
		map_ImgCol(panel_ptr, &img_00);
	}
	///////////////////////////////////////////////////////////////
	///////////////////////////////////// EVALUATE MEAN VALUE INDEX
	if (isImgMosaic == true) {
		float mean = panel_ptr->get_MeanVal();
		int index = ceil(mapRange(0, 255, 0, 1, mean));
		string url = url_IMGs + to_string(index) + ".jpg";
		ntImage image(url);
		arma::fmat* matrix = image.getMatrix();
		//*matrix = 255 - *matrix;
		read_IMG(url);
		panel_ptr->reparam_UV();
		map_ImgCol(panel_ptr, matrix);
	}
	///////////////////////////////////////////////////////////////
	//////////////////////////////////// CALCULATE PERFORATION SIZE
	panel_ptr->add_Perf();
	///t_perforate += (clock() - t);
	///
	return true;
}
bool ntTriSkin::pan_F0(ntPanel* panel_ptr){
	/// SCALE TO 2ND VIEWPORT--REPLACE WITH CAMERA FIT FUNCTION !
	/// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	set_Scale2D(panel_ptr, 10);
	///
	return true;
}
*/
void ntTriSkin::funct(ntPanel* panel_ptr) {
	/// ///////////////////////////////////////////////////////////
	/// /////////////////////////////////////////// BEGIN TIMER- 00
	auto t0 = std::chrono::steady_clock::now();
	///
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
	///
	auto t1 = std::chrono::steady_clock::now();
	t_transform += duration_DBL(t1 - t0);
	/// ///////////////////////////////////////////// END TIMER- 00
	///////////////////////////////////////////////////////////////
	/// /////////////////////////////////////////// BEGIN TIMER- 01
	t0 = std::chrono::steady_clock::now();
	///
	panel_ptr->calcArea();
	panel_ptr->calcPhi();
	float areaP = panel_ptr->get_Area();
	///
	t1 = std::chrono::steady_clock::now();
	t_calcArea += duration_DBL(t1 - t0);
	/// ///////////////////////////////////////////// END TIMER- 01
	///////////////////////////////////////////////////////////////
	////////////////////////////////// SUBDIVIDE FOR GLOBAL DISPLAY
	/// /////////////////////////////////////////// BEGIN TIMER- 02
	t0 = std::chrono::steady_clock::now();
	///
	panel_ptr->sub_Div(gen);
	///
	t1 = std::chrono::steady_clock::now();
	t_SD += duration_DBL(t1 - t0);
	/// ///////////////////////////////////////////// END TIMER- 02
	///////////////////////////////////////////////////////////////
	///////////////////////////////////// CALCULATE PEFORATION GRID
	/// /////////////////////////////////////////// BEGIN TIMER- 03
	t0 = std::chrono::steady_clock::now();
	///
	int div = floor(panel_ptr->get_EdgeMin() / perfSpacing);
	panel_ptr->plot_Perf(div, grid_type, perf_type);
	panel_ptr->set_PerfStyle(perf_style);

	///////////////////////////////////////////////////////////////
	////////////////////////////////// LOAD TEXTURE MAP TO SURFACES
	if (isImgLoaded == true) {
		map_ImgCol(panel_ptr, &img_00);
	}
	///////////////////////////////////////////////////////////////
	///////////////////////////////////// EVALUATE MEAN VALUE INDEX
	if (isImgMosaic == true) {
		float mean = panel_ptr->get_MeanVal();
		int index = ceil(mapRange(0, 255, 0, 1, mean));
		string url = url_IMGs + to_string(index) + ".jpg";
		ntImage image(url);
		arma::fmat* matrix = image.getMatrix();
		//*matrix = 255 - *matrix;
		read_IMG(url);
		panel_ptr->reparam_UV();
		map_ImgCol(panel_ptr, matrix);
	}
	///////////////////////////////////////////////////////////////
	//////////////////////////////////// CALCULATE PERFORATION SIZE
	panel_ptr->add_Perf();
	///
	t1 = std::chrono::steady_clock::now();
	t_perforate += duration_DBL(t1 - t0);
	/// ///////////////////////////////////////////// END TIMER- 03
	///////////////////////////////////////////////////////////////
	//////////////////////////////////////// GRAPH PERFORATION DATA
	/// /////////////////////////////////////////// BEGIN TIMER- 04
	t0 = std::chrono::steady_clock::now();
	///
	///panel_ptr->set_Graph();
	log_Angles(panel_ptr);

	n_perf += panel_ptr->perfs.size();
	areaS_gross += areaP;
	areaS_net += (areaP - panel_ptr->perf_area);

	if (areaP < areaP_Min) {
		areaP_Min = areaP;
	}
	if (areaP > areaP_Max) {
		areaP_Max = areaP;
	}
	///
	t1 = std::chrono::steady_clock::now();
	t_graphData += duration_DBL(t1 - t0);
	/// ///////////////////////////////////////////// END TIMER- 04
	///////////////////////////////////////////////////////////////
	////////////////////////////////////////// SAVE PANEL DATA DATA
	/// /////////////////////////////////////////// BEGIN TIMER- 05
	t0 = std::chrono::steady_clock::now();
	///
	if (doSaveTXT == true) {
		write_Panel_TXT(panel_ptr);
	}
	///
	t1 = std::chrono::steady_clock::now();
	t_saveTXT += duration_DBL(t1 - t0);
	/// ///////////////////////////////////////////// END TIMER- 05
	///////////////////////////////////////////////////////////////
	/// /////////////////////////////////////////// BEGIN TIMER- 06
	t0 = std::chrono::steady_clock::now();
	///
	if (doSaveIMG == true) {
		write_Panel_IMG(panel_ptr);
	}
	///
	t1 = std::chrono::steady_clock::now();
	t_saveIMG += duration_DBL(t1 - t0);
	/// ///////////////////////////////////////////// END TIMER- 06
	///////////////////////////////////////////////////////////////
	/////////////////////////////////// SCALE PANEL FOR 2D VIEWPORT
	/// /////////////////////////////////////////// BEGIN TIMER- 07
	t0 = std::chrono::steady_clock::now();
	///
	set_Scale2D(panel_ptr, 10);
	///
	t1 = std::chrono::steady_clock::now();
	t_SC2 += duration_DBL(t1 - t0);
	/// ///////////////////////////////////////////// END TIMER- 07
}
void ntTriSkin::set_Parameters(grid_Type grid_type, perf_Type perf_type, float perf_spacing) {
	this->grid_type =	grid_type;
	this->perf_type =	perf_type;
	this->perfSpacing = perf_spacing;
}
void ntTriSkin::set_PerfStyle(perf_Style perf_style) {
	this->perf_style = perf_style;
	for (int i = 0; i < panels.size(); i++) {
		panels.at(i)->set_PerfStyle(perf_style);
	}
}
void ntTriSkin::set_PerfType(perf_Type perf_type) {
	this->perf_type = perf_type;
	for (int i = 0; i < panels.size(); i++) {
		panels.at(i)->set_PerfType(perf_type);;
	}
}
void ntTriSkin::set_ImgMosaic(string url) {
	url_IMGs = url;
	isImgMosaic = true;
}
///////////////////////////////////////////////////////////////
//////////////////////////////// FORMAT FUNCTION FOR INPUT DATA
string ntTriSkin::format_STR(string line){

//	string token = "PANEL";
	string token = ":";
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
string ntTriSkin::format_VEC(ntVec3* vec) {
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
Vec3 ntTriSkin::add_VEC(string line) {
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

///////////////////////////////////////////////////////////////
//////////////////// PANEL TRANSFORMATION AND MAPPING FUNCTIONS
void ntTriSkin::align_Panel(ntPanel* panel_ptr, ntVec3* axis_A, ntVec3* axis_B, ntVec3* pos) {
	int cnt = 13;	//length of vecs[]
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////// TRANSLATE TO POS
	Vec3 trans_V = Vec3(pos->x, pos->y, pos->z);	///TRANSLATION VECTOR
	for (int i = 0; i < cnt; i++) {
		panel_ptr->vecs[i]->sub(&trans_V);
		panel_ptr->vecs[i]->orient(axis_A, axis_B);
	}
	panel_ptr->calcCentroid();
	panel_ptr->calcNorm();
}
void ntTriSkin::map_ImgCol(ntPanel* panel_ptr, arma::fmat* img_ptr) {

	int dim_X = img_ptr->n_cols;
	int dim_Y = img_ptr->n_rows;

	/// ////////////////////// MAP COLOR FROM UV LIST
	for (int i = 0; i < panel_ptr->p_UVs.size(); i++) {

		float x = panel_ptr->p_UVs.at(i)->x;
		float y = panel_ptr->p_UVs.at(i)->y;
		bool isBounds = true;

		if (x >= 1) {
			x = 0.99999;
			isBounds = false;
		}
		if (x <= 0) {
			x = 0.00001;
			isBounds = false;
		}
		if (y >= 1) {
			y = 0.99999;
			isBounds = false;
		}
		if (y <= 0) {
			y = 0.00001;
			isBounds = false;
		}

		x = floor(mapRange(0, dim_X, 0, 1, x));
		y = floor(mapRange(0, dim_Y, 0, 1, y));

		float col = img_ptr->at(y, x);
		col = mapRange(0, 1, 0, 255, col);
		panel_ptr->p_Col.at(i) = col;
	}
	/// ////////////////////// MAP COLOR TO VERTEX
	for (int i = 0; i < panel_ptr->faces_L.size(); i++) {
		for (int j = 0; j < panel_ptr->faces_L.at(i)->size(); j++) {
			for (int k = 0; k < 3; k++) {

				float x = panel_ptr->faces_L.at(i)->at(j).uvws[k]->x;
				float y = panel_ptr->faces_L.at(i)->at(j).uvws[k]->y;
				bool isBounds = true;

				if (x >= 1) {
					x = 0.99999;
					isBounds = false;
				}
				if (x <= 0) {
					x = 0.00001;
					isBounds = false;
				}
				if (y >= 1) {
					y = 0.99999;
					isBounds = false;
				}
				if (y <= 0) {
					y = 0.00001;
					isBounds = false;
				}

				x = floor(mapRange(0, dim_X, 0, 1, x));
				y = floor(mapRange(0, dim_Y, 0, 1, y));

				float col = img_ptr->at(y, x);
				col = mapRange(0, 1, 0, 255, col);

				panel_ptr->faces_L.at(i)->at(j).verts.at(k)->set_color(ntCol4(col, col, col, 1));
				panel_ptr->faces_L.at(i)->at(j).setFx(col);  /// color for panel is same a last vertex

				if (i < panel_ptr->faces_G.size()) {
					panel_ptr->faces_G.at(i)->at(j).verts.at(k)->set_color(ntCol4(col, col, col, 1));
					panel_ptr->faces_G.at(i)->at(j).setFx(col);  /// color for panel is same a last vertex
				}
			}
		}
	}
}
void ntTriSkin::map_ImgCol(ntPanel* panel_ptr, arma::mat* img_ptr) {

	int dim_X = img_ptr->n_cols;
	int dim_Y = img_ptr->n_rows;

	/// ////////////////////// MAP COLOR FROM UV LIST
	for (int i = 0; i < panel_ptr->p_UVs.size(); i++) {

		float x = panel_ptr->p_UVs.at(i)->x;
		float y = panel_ptr->p_UVs.at(i)->y;
		bool isBounds = true;
		
		if (x >= 1) {
			x = 0.99999;
			isBounds = false;
		}
		if (x <= 0) {
			x = 0.00001;
			isBounds = false;
		}
		if (y >= 1) {
			y = 0.99999;
			isBounds = false;
		}
		if (y <= 0) {
			y = 0.00001;
			isBounds = false;
		}
		
		x = floor(mapRange(0, dim_X, 0, 1, x));
		y = floor(mapRange(0, dim_Y, 0, 1, y));

		float col = img_ptr->at(y, x);
		col = mapRange(0, 1, 0, 255, col);
		panel_ptr->p_Col.at(i) = col;
	}
	/// ////////////////////// MAP COLOR TO VERTEX
	for (int i = 0; i < panel_ptr->faces_L.size(); i++) {
		for (int j = 0; j < panel_ptr->faces_L.at(i)->size(); j++) {
			for (int k = 0; k < 3; k++) {

				float x = panel_ptr->faces_L.at(i)->at(j).uvws[k]->x;
				float y = panel_ptr->faces_L.at(i)->at(j).uvws[k]->y;
				bool isBounds = true;

				if (x >= 1) {
				x = 0.99999;
				isBounds = false;
				}
				if (x <= 0) {
				x = 0.00001;
				isBounds = false;
				}
				if (y >= 1) {
				y = 0.99999;
				isBounds = false;
				}
				if (y <= 0) {
				y = 0.00001;
				isBounds = false;
				}
				
				x = floor(mapRange(0, dim_X, 0, 1, x));
				y = floor(mapRange(0, dim_Y, 0, 1, y));
				
				float col = img_ptr->at(y, x);
				col = mapRange(0, 1, 0, 255, col);
				
				panel_ptr->faces_L.at(i)->at(j).verts.at(k)->set_color(ntCol4(col, col, col, 1));
				panel_ptr->faces_L.at(i)->at(j).setFx(col);  /// color for panel is same a last vertex

				if (i < panel_ptr->faces_G.size()) {
					panel_ptr->faces_G.at(i)->at(j).verts.at(k)->set_color(ntCol4(col, col, col, 1));
					panel_ptr->faces_G.at(i)->at(j).setFx(col);  /// color for panel is same a last vertex
				}
			}
		}
	}
}
/*
void ntTriSkin::tile_Img(int U, int V, af::array img) {

	/// REVIEW IMPLEMENTATION OF ARRAY FIRE FUNCTIONS
	/// AF::TILE | AF::JOIN | AF::FLIP | AF:TRANSPOSE

	int dim_X = img_X;
	int dim_Y = img_Y;

	img_X = img_X * U;
	img_Y = img_Y * V;

	af::array img_Tile(img_Y, img_X);
	//img_Tile += img;
	//af::tile(img, V, U);
}

*/
void ntTriSkin::log_Angles(ntPanel* panel_ptr) {
	float min = panel_ptr->get_AngleMin();
	float max = panel_ptr->get_AngleMax();

	if (min < phi_Min) {
		phi_Min = min;
	}
	if (max > phi_Max) {
		phi_Max = max;
	}
}
void ntTriSkin::set_Scale2D(ntPanel* panel_ptr, double scFx) {
	/// SCALE PANELS TO VIEW--- REPLACE WITH CAMERA FIT FUNCTION		//!!!!!!!!!!!!!!!
	/// TRANSLATE TO HUD LOCATION
	ntVec3 posXY = ntVec3(55, 560, 0);  /////// POSITION FOR 2D HUD		//!!!!!!!!!!!!!!!
	float sc_Factor = scFx;

	for (int j = 0; j < panel_ptr->vecs_SD.size(); j++) {
		ntMatrix4 SC1 = ntMatrix4(panel_ptr->vecs_SD[j]);
		SC1.scale3d(sc_Factor);
		SC1.translate(posXY);
	}
	for (int j = 0; j < panel_ptr->perfs.size(); j++) {
		for (int k = 1; k < panel_ptr->perfs.at(j)->vecs.size(); k++) {
			ntMatrix4 SC3 = ntMatrix4(panel_ptr->perfs.at(j)->vecs.at(k));
			SC3.scale3d(sc_Factor);
			SC3.translate(posXY);
		}
	}
	for (int j = 0; j < panel_ptr->fastr.size(); j++) {
		for (int k = 1; k < panel_ptr->fastr.at(j)->vecs.size(); k++) {
			ntMatrix4 SC3 = ntMatrix4(panel_ptr->fastr.at(j)->vecs.at(k));
			SC3.scale3d(sc_Factor);
			SC3.translate(posXY);
		}
	}
	for (int j = 0; j < panel_ptr->c_L.size(); j++) {
			ntMatrix4 SC3 = ntMatrix4( panel_ptr->c_L.at(j));
			SC3.scale3d(sc_Factor);
			SC3.translate(posXY);
	}
	for (int j = 0; j < panel_ptr->f_L.size(); j++) {
		ntMatrix4 SC3 = ntMatrix4(panel_ptr->f_L.at(j));
		SC3.scale3d(sc_Factor);
		SC3.translate(posXY);
	}
	for (int i = 0; i < panel_ptr->cells_L.size(); i++) {
		for (int j = 3; j <  panel_ptr->cells_L.at(i).vecs_SD.size(); j++) {
			ntMatrix4 SC3 = ntMatrix4(panel_ptr->cells_L.at(i).vecs_SD.at(j));
			SC3.scale3d(sc_Factor);
			SC3.translate(posXY);
		}
	}
}

void ntTriSkin::run(){
		display();
}

void ntTriSkin::set_Gen(int gen) {
		this->gen = gen;
		gen_G = gen;
}
void ntTriSkin::set_GenDisplay(int gen) {
	if (gen_G > gen) {
		this->gen = gen;
	} 
}
void ntTriSkin::display_Next() {
	if (panel_Index < panel_Dim - 1) {
		float c = panels.at(panel_Index)->image_Val;
		panels.at(panel_Index)->faces_G.at(0)->at(0).set_color(Col4(c, c, c, 1));
		if (mode_M == vQ) {
			ntColor4f col = panels.at(panel_Index)->col;
			panels.at(panel_Index)->faces_G.at(0)->at(0).set_color(col);
		}
		panel_Index = panel_Index + 1;
		std::cout << panel_Index << endl;
	}
	else {
		float c = panels.at(panel_Dim - 1)->image_Val;
		panels.at(panel_Dim - 1)->faces_G.at(0)->at(0).set_color(Col4(c, c, c, 1));
		if (mode_M == vQ) {
			ntColor4f col = panels.at(panel_Index)->col;
			panels.at(panel_Index)->faces_G.at(0)->at(0).set_color(col);
		}
		panel_Index = 0;
		std::cout << panel_Index << endl;
	}
	if (mode_M == vW || mode_M == vA) {
		panels.at(panel_Index)->faces_G.at(0)->at(0).set_color(col_Select);
	}
}
void ntTriSkin::display_Prev() {
	if (panel_Index > 0 && panel_Index < panel_Dim) {
		float c = panels.at(panel_Index)->image_Val;
		panels.at(panel_Index)->faces_G.at(0)->at(0).set_color(Col4(c, c, c, 1));
		if (mode_M == vQ) {
			ntColor4f col = panels.at(panel_Index)->col;
			panels.at(panel_Index)->faces_G.at(0)->at(0).set_color(col);
		}
		panel_Index = panel_Index - 1;
		std::cout << panel_Index << endl;
	}
	else {
		float c = panels.at(0)->image_Val;
		panels.at(0)->faces_G.at(0)->at(0).set_color(Col4(c, c, c, 1));
		if (mode_M == vQ) {
			ntColor4f col = panels.at(panel_Index)->col;
			panels.at(panel_Index)->faces_G.at(0)->at(0).set_color(col);
		}
		panel_Index = panel_Dim - 1;
		std::cout << panel_Index << endl;
	}
	if (mode_M == vW || mode_M == vA) {
		panels.at(panel_Index)->faces_G.at(0)->at(0).set_color(col_Select);
	}
}
void ntTriSkin::set_Mode(D_mode mode) {
	mode_M = mode;
	if (mode_M == vQ) {
		for (int i = 0; i < panels.size(); i++) {

			float w = 1;
			float r = 1;
			float g = 1;
			float b = 1;

			if (perf_style == 1) {
				w = panels.at(i)->get_Weight();
			}
			if (perf_style == 2) {
				w = panels[i]->get_Area() - areaP_Min;
				w = mapRange(0, 1, areaP_Min, areaP_Max, w);
			}
			if (perf_style == 3) {
				float a = panels[i]->get_Area();
				w = (panels[i]->perf_area) / a;
				w = mapRange(0, 1, 0.5, 1, w);
			}
			if (perf_style == 4) {
				w = panels.at(i)->get_AngleMax() - panels.at(i)->get_AngleMin();
				w = mapRange(0, 1, phi_Min, phi_Max, w);
			}
			if (perf_style ==5) {
				w = panels.at(i)->norm.z;
				if (w > 0.5) { w = 0.5; }
				if (w < 0) { w = 0; }
				w = mapRange(0, 1, 0, 0.5, w);
			}
			r = mapRange(0.7, 1, 0, 1, w);
			g = mapRange(0, 0.8, 0, 1, w);
			b = 0;

			panels.at(i)->set_Color(ntColor4f(r,g,b,1));
			panels.at(i)->faces_G.at(0)->at(0).set_color(Col4(r, g, b, 1));
			panels.at(i)->faces_G.at(0)->at(0).edges.at(0).setCol(ntColor4f(0, 0, 0, .5));
			panels.at(i)->faces_G.at(0)->at(0).edges.at(1).setCol(ntColor4f(0, 0, 0, .5));
			panels.at(i)->faces_G.at(0)->at(0).edges.at(2).setCol(ntColor4f(0, 0, 0, .5));
		}
	}
	///////////////////////////////////////////////////////////////
	/////////////////////////////////////////  IMAGE MAPPED DISPLAY
	if (mode_M == vA) {
		for (int i = 0; i < panels.size(); i++) {
			panels.at(i)->faces_G.at(0)->at(0).edges.at(0).setCol(ntColor4f(1, 1, 1, .5));
			panels.at(i)->faces_G.at(0)->at(0).edges.at(1).setCol(ntColor4f(1, 1, 1, .5));
			panels.at(i)->faces_G.at(0)->at(0).edges.at(2).setCol(ntColor4f(1, 1, 1, .5));
			if (i == panel_Index) {
				panels.at(i)->faces_G.at(0)->at(0).set_color(col_Select);
			}
		}
	}
	if (mode_M == vS) {
			for (int i = 0; i < panels.size(); i++) {
				float col = panels.at(i)->image_Val;
				panels.at(i)->faces_G.at(0)->at(0).set_color(ntColor4f(col, col, col, 1));
			}
	}
	if (mode_M == vD) {
		for (int i = 0; i < panels.size(); i++) {
			float col = panels.at(i)->image_Val;
			panels.at(i)->faces_G.at(0)->at(0).set_color(ntColor4f(col, col, col, 1));
		}
	}
	///////////////////////////////////////////////////////////////
	///////////////////////////////////////////  WIRE FRAME DISPLAY
	if (mode_M == vW){
		for (int i = 0; i < panels.size(); i++) {
			panels.at(i)->faces_G.at(0)->at(0).edges.at(0).setCol(ntColor4f(1, 1, 1, .5));
			panels.at(i)->faces_G.at(0)->at(0).edges.at(1).setCol(ntColor4f(1, 1, 1, .5));
			panels.at(i)->faces_G.at(0)->at(0).edges.at(2).setCol(ntColor4f(1, 1, 1, .5));
			if (i == panel_Index) {
				panels.at(i)->faces_G.at(0)->at(0).set_color(col_Select);
			}
		}
	}
}
void ntTriSkin::set_Mode_L(L_mode mode) {
	mode_L = mode;
}

void ntTriSkin::save_IMG() {
	doSaveIMG = true;
}
void ntTriSkin::save_TXT() {
	doSaveTXT = true;
}

void ntTriSkin::display(){
	if (mode_M == vQ) {
		for (int i = 0; i < panels.size(); i++) {
			panels.at(i)->faces_G.at(0)->at(0).display();
			panels.at(i)->faces_G.at(0)->at(0).edges.at(0).display();
		}
	}
	if (mode_M == vS) {
		for (int i = 0; i < panels.size(); i++) {

			int cnt = panels.size();
			if (cnt < 500) {
				//panels.at(i)->faces_G.at(0)->at(0).edges.at(0).display();
				//panels.at(i)->faces_G.at(0)->at(0).edges.at(1).display();
				//panels.at(i)->faces_G.at(0)->at(0).edges.at(2).display();
				panels.at(i)->display_EdgeSD_G(gen);
			} else {
				panels.at(i)->faces_G.at(0)->at(0).display();
			}
		}
	}
	if (mode_M == vD) {
		for (int i = 0; i < panels.size(); i++) {
			panels.at(i)->display_Face_G(gen);
		}
	}
	if (mode_M == vW || mode_M == vA) {
		for (int i = 0; i < panels.size(); i++) {
			panels.at(i)->faces_G.at(0)->at(0).edges.at(0).display();
			panels.at(i)->faces_G.at(0)->at(0).edges.at(1).display();
			panels.at(i)->faces_G.at(0)->at(0).edges.at(2).display();
		}
		panels.at(panel_Index)->faces_G.at(0)->at(0).display();
	}
}
void ntTriSkin::display_2D() {
	if (panel_Index >= 0 && panel_Index < panels.size()) {
		if (mode_M == vQ) {
			panels.at(panel_Index)->display_Perf();
		}
		if (mode_M == vW) {
			panels.at(panel_Index)->display_Perf();
			panels.at(panel_Index)->display_Joint();
			panels.at(panel_Index)->display_CL();
		}
		if (mode_M == vA) {
			panels.at(panel_Index)->display_Face_L(mode_L, gen_L);
		}
		if (mode_M == vS) {
			panels.at(panel_Index)->display_EdgeSd(gen);
		}
		if (mode_M == vD) {
			panels.at(panel_Index)->display_Face_L(mode_L, gen);
		}
		panels.at(panel_Index)->display_Edge();
		panels.at(panel_Index)->display_Nodes();
		panels.at(panel_Index)->display_Graph();
	}
	img_SRC.display();
	graph00.display();
	graph01.display();

}
void ntTriSkin::display_SysData() {
	//title.display();

	//t_00.display();
	//t_01.display();
	//t_02.display();
	//t_03.display();
	//t_04.display();
	//t_05.display();
	//t_06.display();
	//t_07.display();
	//t_08.display();
	//t_09.display();
	//t_10.display();
	//t_11.display();
	//t_12.display();
	//t_13.display();
	//t_14.display(); 
	//t_15.display();
	//t_16.display();
	//t_17.display();
	//t_18.display();
	//t_19.display();
}