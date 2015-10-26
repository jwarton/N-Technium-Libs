#include "nt_09_ovisApp.h"

void ovisApp::init() {

	read_DATA();

	Vec3* axis_Z = new Vec3( 0, 0, 1 );
	Vec3* axis_X = new Vec3( 1, 0, 0 );
	
	panel_Dim = panels.size();
	for (int i = 0; i < panel_Dim; i++) {
		if (i < faces.size()) {
			for (int j = 0; j < 3; j++) {
				ntMatrix4 SC3 = ntMatrix4(faces.at(i)->vecs[j]);
				SC3.scale3d(0.0001);
			}
			float r = mapRange(0, 1, 0, panel_Dim, i);
			float b = mapRange(0, 1, 0, panel_Dim, i, false);
			if (i % 2 == 0) {
				faces.at(i)->setColor(Col4(r, 0, b, 1));
			}
			else {
				faces.at(i)->setColor(Col4(r, 0, b, 1));
			}
			faces.at(i)->edges.at(0).setCol(ntColor4f(0, 0, 0, 1));
		}
		/////////////////////////////////////////////////////////////////////////////  ALIGN CENTROID/NORMAL TO Z-AXIS
		panels.at(i)->calcCentroid();
		panels.at(i)->calcNorm();
		align_Panel(panels.at(i), axis_Z, &panels.at(i)->norm, panels.at(i)->cent);	

		/////////////////////////////////////////////////////////////////////////////  ALIGN V0/EDGE TO X-AXIS
		Vec3 edge_X = Vec3(panels.at(i)->v1->x, panels.at(i)->v1->y, panels.at(i)->v1->z);
		edge_X.sub(panels.at(i)->v0);
		align_Panel(panels.at(i), axis_X, &edge_X, panels.at(i)->v0); 

		float val = (rand() % 255 + 1);		//TEMPORARY VALUES FOR PANEL PERFORATION DRIVER
		val = mapRange(0,1,0,255,val);
		panels.at(i)->set_IMG(val);
		panels.at(i)->calc_Perf();
		//write_Panel(panels.at(i));
		for (int j = 0; j < 3; j++) {
			ntMatrix4 SC1 = ntMatrix4(panels.at(i)->vecs[j]);
			SC1.scale3d(0.015);
		}
		for (int j = 0; j < panels.at(i)->perfs.size(); j++) {
			for (int k = 0; k < panels.at(i)->perfs.at(j)->seg; k++) {
				ntMatrix4 SC2 = ntMatrix4(panels.at(i)->perfs.at(j)->vecs.at(k));
				SC2.scale3d(0.015);
			}
		}
	}
}

void ovisApp::read_DATA(){
	string pathI = nt_Utility::getPathToResources() + "data\\" + fileName + ".txt";

	ifstream file(pathI);
	string line;

	int cnt = 0;

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
			line = format_Norm(line);
			panel_Norm = line + "\n";
		}
		if (isStartFile == true && line.find("POS:") != string::npos) {
			ntVec3 vec = add_Vert(line);
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
			ntVec3 * n0 = new ntVec3(verts[0].x, verts[0].y, verts[0].z);
			ntVec3 * n1 = new ntVec3(verts[1].x, verts[1].y, verts[1].z);
			ntVec3 * n2 = new ntVec3(verts[2].x, verts[2].y, verts[2].z);
			ntFace3 * face = new ntFace3(n0, n1, n2);

			faces.push_back(face);

			ntVec3 * v0 = new ntVec3(verts[0].x, verts[0].y, verts[0].z);
			ntVec3 * v1 = new ntVec3(verts[1].x, verts[1].y, verts[1].z);
			ntVec3 * v2 = new ntVec3(verts[2].x, verts[2].y, verts[2].z);
			ntPanel * panel = new ntPanel(v0, v1, v2);
			panel->set_ID(panel_ID);
			panel->set_nG(panel_Norm);
			panel->set_pG(vertex_Pos);
			panels.push_back(panel);
			panel_ID = "<< ERROR >>";
			panel_Norm = "<< ERROR >>";
			vertex_Pos = "";
		}
	}
}

string ovisApp::format_Norm(string line){
	string token = "PANEL";
	string norm = "   VEC: <";

	if (line.find(token) != string::npos) {
		char chars[] = "/PANELNORM:{";
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
				norm += ss.str() + ">";
			}
			cnt += 1;
		}
		return norm;
	}
}
string ovisApp::format_POS(string line) {
	string token = "POS:";
	string pos = "     POS: (";
	string norm ="   VEC: <" ;

	if (line.find(token) != string::npos) {
		char chars[] = "/POS:{}NRM";
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
				pos += ss.str() + ", " ;
			} 
			else if (cnt == 2) {
				pos += ss.str() + ")";
			}
			else if (cnt >2 && cnt < 5) {
				norm += ss.str() + ", ";
			}
			else {
				norm += ss.str() + ">";
			}
			cnt += 1;
		}
		return pos;// +norm;
	}
}
string ovisApp::format_VEC(ntVec3* vec) {
	string pos = "     POS: (";
	string norm = "   VEC: <";
	float val;

	for (int i = 0; i < 3; i++) {
		if (i == 0) {
			val = vec->x;
			//if (val < .001 && val > -.001){
			//	val = 0;
			//}
		}
		else if (i == 1) {
			val = vec->y;
			//if (val < .001 && val > -.001) {
			//	val = 0;
			//}
		}
		else if (i == 2) {
			val = vec->z;
			//if (val < .001 && val > -.001) {
			//	val = 0;
			//}
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

void ovisApp::align_Panel(ntPanel* panel_ptr, Vec3* axis) {
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

	////////////////////////////////////////////////////////////////  BUILD ROTATION MATRIX
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
	//panel_ptr->calcCentroid();
	//panel_ptr->calcNorm();

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
	file << format_VEC(panel_ptr->get_v_G().at(0)) << "\n";
	file << format_VEC(panel_ptr->get_v_G().at(1)) << "\n";
	file << format_VEC(panel_ptr->get_v_G().at(2)) << "\n";
	file << "\n";
	file << "//  PANEL ORIENTATION VECTOR:\n";
	file << "//" << panel_ptr->get_n_G();
	file << "//  AREA:  " << to_string(calc_Area(panel_ptr)) << "            [ SQ.INCH ]\n\n";
	file << "//////////////////////////////////////////////////////////////////////\n";
	file << "//  NODE POSITIONS:\n";
	file << "//  [ LOCAL COORDINATES ]\n";
	file << "//        (                 X,                  Y,                  Z)\n";
	file << format_VEC(panel_ptr->v0) << "\n";
	file << format_VEC(panel_ptr->v1) << "\n";
	file << format_VEC(panel_ptr->v2) << "\n";
	file << "\n";
	file << "//  PANEL ORIENTATION VECTOR:\n";
	file << format_VEC(&panel_ptr->norm) << "\n";
	file << "\n";
	file << "//  CORNER POSITIONS:          {POS:           }	\n";
	file << "\n";
	file << "//  FASTENER POSITIONS:        {POS:  RADIUS:  }\n";
	file << "\n";
	file << "//  PERFORATION DATA:          {POS:  RADIUS:  }\n";
	for (int i = 0; i < panel_ptr->get_Perf().size(); i++) {
		file << format_VEC(panel_ptr->get_Perf().at(i)) << "       RAD:  "<< panel_ptr->get_Perf_R().at(i) << "\n";
	}
	file.close();
}

Vec3 ovisApp::add_Vert(string line) {
	string token = "POS:";

	if (line.find(token) != string::npos) {
		char chars[] = "/POS:{}NRM";
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

			if (cnt == 0) {
				vertex_Pos += "     POS: (  ";
			}
			if (cnt <2) {
				vertex_Pos += str + ", ";
				vertPos[cnt] = val;
			}
			else if (cnt == 2) {
				vertex_Pos += str + ")\n";
				vertPos[cnt] = val;
			}
			cnt += 1;
		}
		ntVec3 vertex = ntVec3(vertPos[0], vertPos[1], vertPos[2]);
		return vertex;
	}
}

void ovisApp::run(){
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
		if (panel_Index < panel_Dim) {
			panel_Index = panel_Index + 1;
			std::cout << panel_Index << endl;
		}
		else {
			panel_Index = 0;
			std::cout << panel_Index << endl;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
		if (panel_Index > 0) {
			panel_Index = panel_Index - 1;
			std::cout << panel_Index << endl;
		}
		else {
			panel_Index = panel_Dim;
			std::cout << panel_Index << endl;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		m = vQ;
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		m = vF;
		for (int i = 0; i < faces.size(); i++) {
			float r = mapRange(0, 1, 0, panel_Dim, i);
			float b = mapRange(0, 1, 0, panel_Dim, i, false);
			if (i % 2 == 0) {
				faces.at(i)->setColor(Col4(r, 0, b, 1));
			}
			else {
				faces.at(i)->setColor(Col4(r, 0, b, 1));
			}
			faces.at(i)->edges.at(0).setCol(ntColor4f(0, 0, 0, .5));
			faces.at(i)->edges.at(1).setCol(ntColor4f(0, 0, 0, .5));
			faces.at(i)->edges.at(2).setCol(ntColor4f(0, 0, 0, .5));
		}
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		m = vS;
		for (int i = 0; i < faces.size(); i++) {
			float c = panels.at(i)->image_Val;
			faces.at(i)->setColor(Col4(c, c, c, 1));
			faces.at(i)->setColor(Col4(c, c, c, 1));
		}
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		m = vW;
		for (int i = 0; i < faces.size(); i++) {
			faces.at(i)->edges.at(0).setCol(ntColor4f(1, 1, 1, .5));
			faces.at(i)->edges.at(1).setCol(ntColor4f(1, 1, 1, .5));
			faces.at(i)->edges.at(2).setCol(ntColor4f(1, 1, 1, .5));
		}
	}
	display();
}

void ovisApp::display(){
	///////////////////////////////////////////////////////////////
	if (m == vQ) {
		if (panel_Index >= 0 && panel_Index < panel_Dim) {
			for (int i = 0; i < panels.size(); i++) {
				panels.at(panel_Index)->display();
				panels.at(panel_Index)->edges.at(0).display(1);
				panels.at(panel_Index)->edges.at(1).display(1);
				panels.at(panel_Index)->edges.at(2).display(1);
				panels.at(panel_Index)->verts.at(0)->display(2);
			}
		}
	}
	if (m == vF) {
		for (int i = 0; i < faces.size(); i++) {
			faces.at(i)->display();
			faces.at(i)->edges.at(0).display();
		}
	}
	if (m == vS) {
		for (int i = 0; i < faces.size(); i++) {
			faces.at(i)->display();
		}
	}
	if (m == vW) {
		for (int i = 0; i < faces.size(); i++) {
			faces.at(i)->edges.at(0).display();
			faces.at(i)->edges.at(1).display();
			faces.at(i)->edges.at(2).display();
		}
	}
}