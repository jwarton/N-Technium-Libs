#include "nt_09_ovisApp.h"
#include <GL/glu.h>

void ovisApp::init() {

	read_DATA();
	read_IMG();

	Vec3* axis_Z = new Vec3( 0, 0, 1 );
	Vec3* axis_X = new Vec3( 1, 0, 0 );
	panel_Dim = panels.size();

	///MULTITHREAD THIS OPERATION
	for (int i = 0; i < panel_Dim; i++) {		
		/////////////////////////////////////////////////////////////////////////////  ALIGN CENTROID/NORMAL TO Z-AXIS
		panels.at(i)->calcCentroid();
		panels.at(i)->calcNorm();
		align_Panel(panels.at(i), axis_Z, &panels.at(i)->norm, panels.at(i)->cent);	

		/////////////////////////////////////////////////////////////////////////////  ALIGN V0/EDGE TO X-AXIS
		Vec3 edge_X = Vec3(panels.at(i)->v1->x, panels.at(i)->v1->y, panels.at(i)->v1->z);
		edge_X.sub(panels.at(i)->v0);
		align_Panel(panels.at(i), axis_X, &edge_X, panels.at(i)->v0); 


		for (int j = 0; j < 3; j++) {
			ntMatrix4 SC3 = ntMatrix4(panels.at(i)->faces.at(0)->at(0).vecs[j]);
			SC3.scale3d(0.0001);
			panels.at(i)->faces.at(0)->at(0).calcCentroid();	//REQUIRED AFTER SCALING
			panels.at(i)->faces.at(0)->at(0).calcNorm();		//REQUIRED AFTER SCALING
		}
		panels.at(i)->sub_Div(gen);

		float col;
		if (isImgLoaded == true) {

			int dim = panels.at(i)->vecs_UV.size();
			int gen_ID = 0;
			int face_ID = 0;

			for (int j = 0; j < dim; j++) {

				float x = panels.at(i)->vecs_UV.at(j).x;
				float y = panels.at(i)->vecs_UV.at(j).y;

				x = floor(mapRange(0, img_X, 0, 1, x));
				y = floor(mapRange(0, img_Y, 0, 1, y));

				col = img_00(x, y);
				col = mapRange(0, 1, 0, 255, col);

				if (gen_ID < panels.at(i)->faces.size()) {

					panels.at(i)->faces.at(gen_ID)->at(face_ID).setColor(ntCol4(col, col, col, 1));

					if (face_ID < (panels.at(i)->faces.at(gen_ID)->size()-1)) {
						face_ID++;
					} else {
						face_ID = 0;
						gen_ID++;
					}
				}

				if (j == 0) {
					panels.at(i)->set_IMG(col);
				}
			}
		}
		else {
			col = (rand() % 255 + 1);// RANDOM VALUES FOR PANEL PERFORATION DRIVER
			panels.at(i)->set_IMG(col);
		}
		///PERFORATION REQUIRES UPDATE TO PERFORATE FOR EACH SUBDIVISION VALUE
		if (i < 10) {
			panels.at(i)->calc_Perf();
		}
		//write_Panel(panels.at(i));

		ntVec3 posXY = ntVec3( 55, 640, 0);
		///SCALE SCENE TO FIT CAMERA---REFACTOR ZOOM TO FIT OPTION
		for (int j = 0; j < 3; j++) {
			ntMatrix4 SC1 = ntMatrix4(panels.at(i)->vecs[j]);
			//SC1.scale3d(0.015);
			SC1.scale3d(5);
			SC1.translate(posXY);
		}
		for (int j = 0; j < panels.at(i)->perfs.size(); j++) {
			for (int k = 0; k < panels.at(i)->perfs.at(j).seg; k++) {
				ntMatrix4 SC2 = ntMatrix4(panels.at(i)->perfs.at(j).vecs.at(k));
				//SC2.scale3d(0.015);
				SC2.scale3d(5);
				SC2.translate(posXY);
			}
		}
	}
}

void ovisApp::read_DATA(){
	string pathI = nt_Utility::getPathToResources() + "data\\" + fileName + ".txt";

	ifstream file(pathI);
	string line;

	int cnt = 0;
	std::vector<ntVec3> params_UV;

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
			ntVec3 uvw = add_VEC(line);
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
			//if (std::stof(panel_ID) < 1000) {	
				ntVec3 * v0 = new ntVec3(verts[0].x, verts[0].y, verts[0].z);
				ntVec3 * v1 = new ntVec3(verts[1].x, verts[1].y, verts[1].z);
				ntVec3 * v2 = new ntVec3(verts[2].x, verts[2].y, verts[2].z);
				ntPanel * panel = new ntPanel(v0, v1, v2);
				panel->set_ID(panel_ID);
				panel->set_nG(panel_Norm);
				panel->set_UVW(panel_UVW);
				panel->vecs_UV.swap(params_UV);
				panel->set_pG(panel_Vert);
				panels.push_back(panel);
			//}

			panel_ID =		"<< ERROR >>";
			panel_Norm =	"<< ERROR >>";
			panel_UVW =		"<< ERROR >>";
			panel_Vert =	"";
			vector<ntVec3>().swap(params_UV);		//DEALLOCATE MEMORY STORED IN PARAMS_UV
		}
	}
}
void ovisApp::read_IMG() {
	using namespace arma;

	url_IMG = path_IMG + pathExtension_IMG + fileName_IMG + fileExt_IMG;
	std::cout << url_IMG << endl;
	const char * file = url_IMG.c_str();
	img_IN = af::loadImage(file, false);
	
	img_X = img_IN.dims(0);
	img_Y = img_IN.dims(1);

	//std::cout << "IMAGE X DIM:  " << img_X << endl;
	//std::cout << "IMAGE Y DIM:  " << img_Y << endl;

	img_00 = zeros<mat>(img_X, img_Y);

	af::array img_LOADER(img_X, img_Y, img_00.memptr());
	img_LOADER += img_IN;
	
	img_LOADER.host((void*)img_00.memptr());
	img_00 = flipud(img_00);

	isImgLoaded = true;
	//af::array varAvg = af::mean(img_IN);
	//float valAvg = af::mean<float>(varAvg);
	//af_print(img_IN);
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
	float val;
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
double ovisApp::calc_Area(ntPanel* panel_ptr){
	
	std::vector <double> xy_pairs;
	std::vector <double> yx_pairs;
	std::vector <double> v_;
	double sum;
	double area;

	//MULTIPLY VERTEX COMPONENTS
	for (int i = 0; i < panel_ptr->verts.size() - 1; i++) {
		double xy_ = panel_ptr->vecs[i]->x * panel_ptr->vecs[i + 1]->y;
		double yx_ = panel_ptr->vecs[i]->y * panel_ptr->vecs[i + 1]->x;	

		xy_pairs.push_back(xy_);
		yx_pairs.push_back(yx_);
	}
	//SUBTRACT MULTIPLICATION SETS
	for (int i = 0; i < panel_ptr->verts.size() - 1; i++) {
		double val = xy_pairs.at(i) - yx_pairs.at(i);

		v_.push_back(val);
	}
	//SUMMATION OF ALL SETS
	std::for_each(v_.begin(), v_.end(), [&](int n) { sum += n;});
	area = abs(sum / 2);//144; //CONVERT FROM UNITS TO FEET
	return area;
}
void ovisApp::write_Panel(ntPanel* panel_ptr) {
	stringstream ss;
	ss << std::setw(5) << std::setfill('0');
	ss << panel_ptr->get_ID();

	fileName = "OP_" + ss.str();
	url = path + pathExtension + fileName + fileExt;

	///////////////////////////////////////////////////// SET TIME STAP
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
	file << panel_ptr->get_p_G() << "\n";
	file << "     POS:  " << format_VEC(panel_ptr->get_v_G().at(0)) << "\n";
	file << "     POS:  " << format_VEC(panel_ptr->get_v_G().at(1)) << "\n";
	file << "     POS:  " << format_VEC(panel_ptr->get_v_G().at(2)) << "\n";
	file << "\n";
	file << "//  PANEL ORIENTATION VECTOR:\n";
	file << "//   VEC" << panel_ptr->get_n_G();
	file << "//   UVW" << panel_ptr->get_UVW();
	file << "//  AREA:  " << to_string(calc_Area(panel_ptr)) << "            [ SQ.INCH ]\n\n";
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
	file << "//  CORNER POSITIONS:          {POS:           }	\n";
	file << "\n";
	file << "//  FASTENER POSITIONS:        {POS:  RADIUS:  }\n";
	file << "\n";
	file << "//  PERFORATION DATA:          {POS:  RADIUS:  }\n";
	for (int i = 0; i < panel_ptr->get_Perf().size(); i++) {
		file << "     POS:  " << format_VEC(panel_ptr->get_Perf().at(i)) << "       RAD:  "<< panel_ptr->get_Perf_R().at(i) << "\n";
	}
	file.close();
}

void ovisApp::run(){
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
		if (panel_Index < panel_Dim-1) {
			float c = panels.at(panel_Index)->image_Val;
			panels.at(panel_Index)->faces.at(0)->at(0).setColor(Col4(c, c, c, 1));
			panel_Index = panel_Index + 1;
			std::cout << panel_Index << endl;
		}
		else {
			float c = panels.at(panel_Dim-1)->image_Val;
			panels.at(panel_Dim - 1)->faces.at(0)->at(0).setColor(Col4(c, c, c, 1));
			panel_Index = 0;
			std::cout << panel_Index << endl;
		}
		if (m == vW) {
			panels.at(panel_Index)->faces.at(0)->at(0).setColor(Col4(1, 0, 0, 1));
		}
	}
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
		if (panel_Index > 0 && panel_Index < panel_Dim) {
			float c = panels.at(panel_Index)->image_Val;
			panels.at(panel_Index)->faces.at(0)->at(0).setColor(Col4(c, c, c, 1));
			panel_Index = panel_Index - 1;
			std::cout << panel_Index << endl;
		}
		else {
			float c = panels.at(0)->image_Val;
			panels.at(0)->faces.at(0)->at(0).setColor(Col4(c, c, c, 1));
			panel_Index = panel_Dim-1;
			std::cout << panel_Index << endl;
		}
		if (m == vW) {
			panels.at(panel_Index)->faces.at(0)->at(0).setColor(Col4(1, 0, 0, 1));
		}
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		m = vQ;
		for (int i = 0; i < panels.size(); i++) {
			float r = mapRange(0, 1, 0, panels.size(), i);
			float b = mapRange(0, 1, 0, panels.size(), i, false);
			if (i % 2 == 0) {
				panels.at(i)->faces.at(0)->at(0).setColor(Col4(r, 0, b, 1));
			}
			else {
				panels.at(i)->faces.at(0)->at(0).setColor(Col4(r, 0, b, 1));
			}
			panels.at(i)->faces.at(0)->at(0).edges.at(0).setCol(ntColor4f(0, 0, 0, .5));
			panels.at(i)->faces.at(0)->at(0).edges.at(1).setCol(ntColor4f(0, 0, 0, .5));
			panels.at(i)->faces.at(0)->at(0).edges.at(2).setCol(ntColor4f(0, 0, 0, .5));
		}
	}
	///////////////////////////////////////////////////////////////
	/////////////////////////////////////////  IMAGE MAPPED DISPLAY
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		m = vS;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		m = vD;
		for (int i = 0; i < panels.size(); i++) {
			float col = panels.at(i)->image_Val;
			panels.at(i)->faces.at(0)->at(0).setColor(ntColor4f(col, col, col, 1));
		}
	}
	///////////////////////////////////////////////////////////////
	///////////////////////////////////////////  WIRE FRAME DISPLAY
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		m = vW;
		for (int i = 0; i < panels.size(); i++) {
			panels.at(i)->faces.at(0)->at(0).edges.at(0).setCol(ntColor4f(1, 1, 1, .5));
			panels.at(i)->faces.at(0)->at(0).edges.at(1).setCol(ntColor4f(1, 1, 1, .5));
			panels.at(i)->faces.at(0)->at(0).edges.at(2).setCol(ntColor4f(1, 1, 1, .5));
			if (i == panel_Index) {
				panels.at(i)->faces.at(0)->at(0).setColor(Col4(1, 0, 0, 1));
			}
		}
	}
	display();
}

void ovisApp::display(){
	if (m == vQ) {
		for (int i = 0; i < panels.size(); i++) {
			panels.at(i)->faces.at(0)->at(0).display();
			panels.at(i)->faces.at(0)->at(0).edges.at(0).display();
		}
	}
	if (m == vD) {
		for (int i = 0; i < panels.size(); i++) {
			panels.at(i)->faces.at(0)->at(0).display();
		}
	}
	if (m == vS) {
		for (int i = 0; i < panels.size(); i++) {
			panels.at(i)->display_Face(2);
		}
	}
	if (m == vW) {
		for (int i = 0; i < panels.size(); i++) {
			panels.at(i)->faces.at(0)->at(0).edges.at(0).display();
			panels.at(i)->faces.at(0)->at(0).edges.at(1).display();
			panels.at(i)->faces.at(0)->at(0).edges.at(2).display();
		}
		panels.at(panel_Index)->faces.at(0)->at(0).display();
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
		for (int i = 0; i < panels.size(); i++) {
			//panels.at(panel_Index)->display();
			//panels.at(panel_Index)->display_Perf();
			panels.at(panel_Index)->display_Edge();
		}
	}
	display_IMG();
}
void ovisApp::display_IMG() {

	float col;
	glPointSize(1);
	glBegin(GL_POINTS);
	for (int i = 0; i < 448; i++){
		for (int j = 0; j < 375; j++) {
			col = img_00(j, i);
			col = mapRange(0, 1, 0, 255, col);
			glColor4f(col, col, col, 1);
			glVertex3f(i +5, j + 161, 0);
		}
	}
	glEnd();
}