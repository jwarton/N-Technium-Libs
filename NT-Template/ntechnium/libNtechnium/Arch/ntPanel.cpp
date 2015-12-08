#include "ntPanel.h"

ntPanel::ntPanel(){}

ntPanel::ntPanel(ntVec3* v0,ntVec3* v1,ntVec3* v2):
v0(v0),v1(v1),v2(v2){
	this->vecs[0] = v0;
	this->vecs[1] = v1;
	this->vecs[2] = v2;
	this->col = ntColor4f(1,1,1,1);
	init();
}

void ntPanel::init(){
	//INITIALIZE VERTEX
	vert0 = new ntVertex (v0,col);
	vert1 = new ntVertex (v1,col);
	vert2 = new ntVertex (v2,col);
	verts.push_back(vert0);
	verts.push_back(vert1);
	verts.push_back(vert2);

	//LIST OF ALL PANEL VECTORS
	vecs_SD.push_back(v0);
	vecs_SD.push_back(v1);
	vecs_SD.push_back(v2);

	//INITIALIZE EDGES
	edges.push_back(ntEdge(v0,v1,col));
	edges.push_back(ntEdge(v1,v2,col));
	edges.push_back(ntEdge(v2,v0,col));

	//INITIALIZE face_L
	std::vector <ntFace3>* face_L = new vector<ntFace3>;
	face_L->push_back(ntFace3(v0, v1, v2));
	faces_L.push_back(face_L);

	//INITIALIZE faces_G
	std::vector <ntFace3>* face = new vector<ntFace3>;
	ntVec3 * n0 = new ntVec3(v0->x, v0->y, v0->z);
	ntVec3 * n1 = new ntVec3(v1->x, v1->y, v1->z);
	ntVec3 * n2 = new ntVec3(v2->x, v2->y, v2->z);
	face->push_back(ntFace3(n0, n1, n2));
	faces_G.push_back(face);

	///std::cout << panel_ID << " Faces Size:  " << faces_G.size() << endl;
	// INITIALIZE CENTROID AND NORMAL
	calcCentroid();
	calcNorm();
	set_vG();
}
void ntPanel::calcCentroid(){
	cent = new ntVec3();
	cent->x = (v1->x + v2->x + v0->x)/3;
    cent->y = (v1->y + v2->y + v0->y)/3;
    cent->z = (v1->z + v2->z + v0->z)/3;
	centroid = ntVertex(cent);
	centroid.setSize(2);
	centroid.setColor(ntColor4f(1,1,1,0));
}
void ntPanel::calcNorm(){
	ntVec3* t1 = new ntVec3();
    t1->set(v2);
    t1->sub(v1);

    ntVec3* t2 = new ntVec3();
    t2->set(v0);
    t2->sub(v1);

    t1->unitize();
    t2->unitize();
	
	norm = t1->cross(t2);
	norm.unitize();
	//norm.invert();
	normal = ntNormal(*cent,norm,.05);
}
///////////////////////////////////////////////////////////////
////////////////////// MANAGES BOTH LOCAL AND GLOBAL SD PROCESS
void ntPanel::sub_Div(int gen) {
	cnt_SubDiv += gen;

	if (is_SubDiv == false && gen > 0) {

		std::vector < vector <ntFace3>* >* faces_ptr = &faces_G;
		sub_Div(faces_ptr, gen, false);

		faces_ptr = &faces_L;
		sub_Div(faces_ptr, gen, true);
	}
	else {
		is_SubDiv = true;  ///EXCEPTION PREVENTS MULTIPLE SUBDIVISION CALLS
	}
}
///////////////////////////////////////////////////////////////
/////////////// CALLED FROM sub_Div(gen) RECURSIVE UNITL GEN==0
void ntPanel::sub_Div(std::vector< vector <ntFace3>* >*	faces, int gen, bool isPanel) {
	if (gen > 0) {
		int dim = faces->size() - 1;
		std::vector <ntFace3>* face = new vector<ntFace3>;
		faces->push_back(face);

		for (int i = 0; i < faces->at(dim)->size(); i++) {
			ntVec3 * v0 = faces->at(dim)->at(i).v0;
			ntVec3 * v1 = faces->at(dim)->at(i).v1;
			ntVec3 * v2 = faces->at(dim)->at(i).v2;
			//FIND MIDPOINT OF EACH EDGE IN FACE
			ntVec3 * n0 = faces->at(dim)->at(i).edges[0].getMid();
			ntVec3 * n1 = faces->at(dim)->at(i).edges[1].getMid();
			ntVec3 * n2 = faces->at(dim)->at(i).edges[2].getMid();
			// SET UVW INTERPOLATION
			ntVec3 * uvw0 = faces->at(dim)->at(i).uvw0;
			ntVec3 * uvw1 = faces->at(dim)->at(i).uvw1;
			ntVec3 * uvw2 = faces->at(dim)->at(i).uvw2;
			// CALCULATE UVW POINT BETWEEN
			ntVec3 * uvw3 = pt_Mid(uvw0,uvw1);
			ntVec3 * uvw4 = pt_Mid(uvw1,uvw2);
			ntVec3 * uvw5 = pt_Mid(uvw2,uvw0);
			//NEW FACES FROM VECS POINTERS
			ntFace3 f0 = ntFace3(v0, n0, n2);
			ntFace3 f1 = ntFace3(v1, n1, n0);
			ntFace3 f2 = ntFace3(v2, n2, n1);
			ntFace3 f3 = ntFace3(n0, n1, n2);
			// SET UVW FOR EACH VERTEX IN GROUP
			f0.setUVW(uvw0, uvw3, uvw5);
			f1.setUVW(uvw1, uvw4, uvw3);
			f2.setUVW(uvw2, uvw5, uvw4);
			f3.setUVW(uvw3, uvw4, uvw5);

			face->push_back(f0);
			face->push_back(f1);
			face->push_back(f2);
			face->push_back(f3);

			if (isPanel == true) {
				//LIST OF PANEL VECTORS
				vecs_SD.push_back(n0);
				vecs_SD.push_back(n1);
				vecs_SD.push_back(n2);
				//LIST OF PANEL PARAMETERS
				//vecs_UV.push_back(uvw3);
				//vecs_UV.push_back(uvw4);
				//vecs_UV.push_back(uvw5);
			}
		}
		sub_Div(faces, gen - 1, isPanel);
	}
}
///////////////////////////////////////////////////////////////
void ntPanel::sub_Div(int div, bool isPerf) {
	/////////////////////////////////////////////////////////////////
	////////////////// FUNCTION SUBDIVIDES TO SPECIFIED EDGE DIVISION
	////////////////// IF BOOL == TRUE PERFORATION POS PASSED TO LIST
	/////////////////////////////////////////////////////////////////
	cnt_SubDiv ++;
	/////////////////////////////////////////////////////////////////
	/////////////// CALCULATE GRID POSTION BY TRIANGULAR SUBDIVISIION
	ntVec3* vec;
	ntVec3* uvw;
	///////////////////////////////////////////////////////////////
	int n_cols = div - 1;
	int n_rows = div - 1;
	float step_X;
	float shif_X;
	float step_Y;

	if (isPerf) {
		///////////////////////////////////////////////////////////////
		////////////////////////////////////// COPY PANEL CORNER POINTS
		ntVec3 v00 = ntVec3(vecs_SD.at(0)->x, vecs_SD.at(0)->y, vecs_SD.at(0)->z);
		ntVec3 v01 = ntVec3(vecs_SD.at(1)->x, vecs_SD.at(1)->y, vecs_SD.at(1)->z);
		ntVec3 v02 = ntVec3(vecs_SD.at(2)->x, vecs_SD.at(2)->y, vecs_SD.at(2)->z);

		ntFace3 face = ntFace3(&v00, &v01, &v02);

		///////////////////////////////////////////////////////////////
		/////////////////////////// COPY PANEL UV PARAMETERS AT CORNERS
		ntVec3 UV0 = ntVec3(vecs_UV.at(0)->x, vecs_UV.at(0)->y, vecs_UV.at(0)->z);
		ntVec3 UV1 = ntVec3(vecs_UV.at(1)->x, vecs_UV.at(1)->y, vecs_UV.at(1)->z);
		ntVec3 UV2 = ntVec3(vecs_UV.at(2)->x, vecs_UV.at(2)->y, vecs_UV.at(2)->z);

		///////////////////////////////////////////////////////////////
		////////////////// SCALE CORNER POINTS TO SUBDIVISION INCREMENT
		float scFactor = 1.0 / div;
		for (int i = 0; i < 3; i++) {
			ntMatrix4 matSc = ntMatrix4(face.vecs[i]);
			matSc.scale3d(scFactor);
		}
		step_X = v01.x;
		shif_X = v02.x;
		step_Y = v02.y;

		////AFFINE TRANSFORMATION OF POINT IN XYZ TO UVW
		arma::mat T = arma::eye<arma::mat>(3, 3);
		T = get_Mat_T(vecs_SD, vecs_UV);

		///////////////////////////////////////////////////////////////
		//////////////////////////////////// PLOT SUBDIVISION POSITIONS
		for (int i = 1; i < n_rows; i++) {
			std::vector <ntVec3*>* p_vecs = new vector <ntVec3*>;
			std::vector <ntVec3*>* p_uvws = new vector <ntVec3*>;
			for (int j = 1; j < n_cols; j++) {
				// DEFINE POSITION OFFSET FOR X
				vec = new ntVec3((step_X * j) + (shif_X * i), (step_Y * i), 0);
				p_vecs->push_back(vec);
				vecs_SD.push_back(vec);

				uvw = map_UVW(T, vec);
				p_uvws->push_back(uvw);

				if (j == (n_cols - 1)) {
					n_cols = n_cols - 1;
				}
			}
			p_XY_Rows.push_back(p_vecs);
			p_UV_Rows.push_back(p_uvws);
		}

		///////////////////////////////////////////////////////////////
		///////////////////////////// ADD FACES TO NEW SUBDIVISION LIST
		std::vector <ntFace3>* faces = new vector<ntFace3>;

		for (int i = 0; i < (p_XY_Rows.size()-1); i++) {
			for (int j = 0; j < (p_XY_Rows.at(i)->size()-1); j++) {

				ntVec3 * vec0 = p_XY_Rows.at(i)->at(j);
				ntVec3 * vec1 = p_XY_Rows.at(i)->at(j + 1);
				ntVec3 * vec2 = p_XY_Rows.at(i + 1)->at(j);
				ntVec3 * uvw0 = p_UV_Rows.at(i)->at(j);
				ntVec3 * uvw1 = p_UV_Rows.at(i)->at(j + 1);
				ntVec3 * uvw2 = p_UV_Rows.at(i + 1)->at(j);

				ntFace3 f0 = ntFace3(vec0, vec1, vec2);

				f0.setUVW(uvw0, uvw1, uvw2);
				faces->push_back(f0);
			}			
		}
		faces_L.push_back(faces);
	}
	is_PerfSD = isPerf;
}
///////////////////////////////////////////////////////////////
void ntPanel::plot_Perf(int div, enum grid_T grid = SQU) {
	if (grid == TRI) {
		plot_Perf_SD(div);
	}
	else if (grid == DIA || grid == SQU) {
		plot_Perf_GR(div, grid);
	}
}
void ntPanel::plot_Perf_GR(int div, enum grid_T grid) {
	ntVec3* vec;
	ntVec3* uvw;
	///////////////////////////////////////////////////////////////
	///////////////// CALCULATE GRID POSTION ORTHOGRAPHIC TO EDGE-0
	float w = (v1->x - v0->x) - (edge_Offset * 2);
	int n_cols = div;

	float spX = (v2->y - v0->y) / n_cols;
	float spY = spX;
	if (spX < ((r_Max * 2) + r_Max)) {
		spX = ((r_Max * 2) + r_Max);
		spY = spX;
		n_cols = ceil((v1->x - v0->x) / spX) + 10;
	}

	int n_rows = ceil((v1->x - v0->x) / spX) + 10;
	float y;
	float x;

	bool isPtInSd = false;
	float val = 0.5;

	////AFFINE TRANSFORMATION OF POINT IN XYZ TO UVW
	arma::mat T = arma::eye<arma::mat>(3, 3);
	T = get_Mat_T(vecs_SD, vecs_UV);

	///////////////////////////////////////////////////////////////
	//////////////////////////////////// PLOT PERFORATION POSITIONS
	for (int i = 0; i <= n_cols; i++) {
		std::vector <ntVec3*>* p_vecs = new vector <ntVec3*>;
		std::vector <ntVec3*>* p_uvws = new vector <ntVec3*>;
		for (int j = 0; j <= n_rows; j++) {

			if (grid == SQU) {
				x = v2->x - (spX * n_cols * 0.5) + (spX * i) + (spX * 0.5);
			}
			if (grid == DIA) {
				if (j % 2 == 0) {
					x = v2->x - (spX * n_cols * 0.5) + (spX * i) + (spX * 0.5);
				}
				else {
					x = v2->x - (spX * n_cols * 0.5) + (spX * i);
				}
			}

			y = (spY * j) + edge_Offset;

			vec = new ntVec3(x, y, 0);
			bool c1 = faces_L.at(0)->at(0).pt_isInside(vec);

			if (c1 == true) {
				uvw = map_UVW(T, vec);

				p_vecs->push_back(vec);
				p_uvws->push_back(uvw);

				p_Pos.push_back(vec);
				p_UVs.push_back(uvw);
			}
		}
		p_XY_Rows.push_back(p_vecs);
		p_UV_Rows.push_back(p_uvws);
	}
}
void ntPanel::plot_Perf_SD(int div) {
	ntVec3* vec;
	ntVec3* uvw;

	int gen = cnt_SubDiv;

	if (is_PerfSD == false) {
		sub_Div(div, true);
	}
	if (is_PerfSD == true) {
		for (int i = 0; i < p_XY_Rows.size(); i++) {
			for (int j = 0; j < p_XY_Rows.at(i)->size(); j++) {

				vec = p_XY_Rows.at(i)->at(j);
				uvw = p_UV_Rows.at(i)->at(j);

				p_Pos.push_back(vec);
				p_UVs.push_back(uvw);
			}
		}
	}
}

float ntPanel::calc_Perf_R(Vec3 *vec, float val) {
	///////////////////////////////////////////////////////////////
	////////////////////////// EXPECTED INPUT RANGE FOR VAL [0 - 1]
	float r;
	float fx = ((rand() % 10)*.01) * 8; /// (((rand() % 10)*.01) - 0.05)
										///////////////////////////////////////////////////////////////
										//////////////////////////////// RANDOMIZED NOISE FACTOR RADIUS
	if (is_Noise == true) {
		r = val + fx;
		fx = ((rand() % 10)*.01) * 7;
		r -= fx;
	}
	else {
		r = val;
	}

	if (r > 1) {
		r = 1;
	}
	///////////////////////////////////////////////////////////////
	////////////////////////  RADIUS WITH INCREMENTAL STEPPED SIZES
	float inc = 0.0625;
	///IMPROVE INTERFACE TO CONTRAST MULTIPLIER
	float min = -6 * inc;  // r_Min == 0.1625 == 3 * inc [ if min == r_Min = 255]
	float max = r_Max + inc;
	
	if (is_Increment == true) {
		int U_bound = (max / 0.0625);
		int L_bound = (min / 0.0625);

		r = mapRange(L_bound, U_bound, 0, 1, r, false);
		r = round(r);
		r = (r * inc);
	}
	else {
		r = round(r * 10) * 0.1;
		r = mapRange(min, max, 0, 1, r, false);
	}

	//float u_bd = max - min;

	//if (stoi(panel_ID) == 963) {
	//	val = r_Min - min;
	//	val = mapRange(0,255, min, max, val, false);
	//	//std::cout << "r_Min:" << r_Min << "  r:" << r << endl;
	//	std::cout << "WHITE VALUE THRESHOLD:  " << val << endl;
	//	//std::cout << r << endl;
	//}
	///////////////////////////////////////////////////////////////
	///////////////////////////////////  SET RADIUS AND ADD TO LIST
	if (r > (r_Max)) {
		r = r_Max;
	}

	if (r < r_Min) {
		r = 0;
	}
	return r;
}
void ntPanel::add_Perf() {
	///////////////////////////////////////////////////////////////
	// LOOP THROUGH POS|RAD DATA STRUCTURES AND CREATE PERF OBJECTS
	if (p_Pos.size() > 0) {
		for (int i = 0; i < p_Pos.size(); i++) {
			//float r = p_Rad.at(i);
			ntVec3* vec = p_Pos.at(i);
			float val = p_Col.at(i);
			float r = calc_Perf_R(vec, val);
			p_Rad.push_back(r);
			if (r > 0) {
				ntCircle * perf = new ntCircle(vec, r, n_seg, Col4(.25, .25, .25, 1));
				perfs.push_back(perf);
			}
		}
	}
	perf_size = perfs.size();
}
///////////////////////////////////////////////////////////////
void ntPanel::set_Color(ntColor4f col){
	this->col=col;
	for(int i = 0; i<verts.size(); i++){
		verts.at(i)->setColor(col);
	}
}
void ntPanel::set_ID(string panel_ID){
	this->panel_ID = panel_ID;
}
void ntPanel::set_nG(string n_G) {
	this->n_G = n_G;
}
void ntPanel::set_vG() {
	ntVec3* v_G0 = new ntVec3(v0->x, v0->y, v0->z);
	ntVec3* v_G1 = new ntVec3(v1->x, v1->y, v1->z);
	ntVec3* v_G2 = new ntVec3(v2->x, v2->y, v2->z);
	v_G.push_back(v_G0);
	v_G.push_back(v_G1);
	v_G.push_back(v_G2);
};
void ntPanel::set_pG(string p_G) {
	this->p_G = p_G;
};
void ntPanel::set_UVW(string string_UVW){
	this->string_UVW = string_UVW;
}
void ntPanel::set_UVW(std::vector <ntVec3*>	vecs_UV) {
	this->vecs_UV = vecs_UV;
	faces_G.at(0)->at(0).setUVW(vecs_UV);
	faces_L.at(0)->at(0).setUVW(vecs_UV);
}
void ntPanel::set_IMG(float val) {
	image_Val = val;
}
void ntPanel::set_Graph() {
	///////////////////////////////////////////////////////////////////////
	/////////////////////////////////////// GRAPH PERFORATION | PANELS DATA
	ntVec3 *graphPos = new ntVec3(5, 390, 0);
	ntVec3 *graphDim = new ntVec3(448, 40, 0);

	int dim =		vecs_SD.size() - 1;  //USED FOR MAX SCALE OF GRAPH DIM Y
	int set_size =  p_Rad.size() - 1;

	//std::cout << "MAX SIZE FOR PERFORATIONS " << dim << endl;
	//std::cout << "SET SIZE FOR PERFORATIONS "<< set_size << endl;
	if (set_size > 0) {
		graph = ntGraph(graphPos, graphDim, p_Rad);
		graph.init();
		is_Graph = true;
	}
}
///////////////////////////////////////////////////////////////
string ntPanel::get_ID() {
	return panel_ID;
}
string ntPanel::get_n_G() {
	return n_G;
}
string ntPanel::get_p_G() {
	return p_G;
}
string ntPanel::get_UVW() {
	return string_UVW;
}

std::vector<ntVec3*> ntPanel::get_v_G() {
	return v_G;
}
std::vector<ntVec3*> ntPanel::get_Perf() {
	return p_Pos;
}
std::vector<float> ntPanel::get_Perf_R() {
	return p_Rad;
}


///////////////////////////////////////////////////////////////
///////////////////////////////////////////// MAPPING FUNCTIONS
arma::mat ntPanel::get_Mat_T(std::vector<ntVec3*> vec_XYZ, std::vector<ntVec3*> vec_UVW) {
	///////////////////////////////////////////////////////////////
	//SOLVE TRANSFORMATION MATRIX OF TRIANGLE XYZ TO UVW
	arma::mat A;	//XYZ POSITION
	A << vec_XYZ.at(0)->x << vec_XYZ.at(0)->y << 1.0 << arma::endr
		<< vec_XYZ.at(1)->x << vec_XYZ.at(1)->y << 1.0 << arma::endr
		<< vec_XYZ.at(2)->x << vec_XYZ.at(2)->y << 1.0 << arma::endr;

	arma::mat B;	//UVW PARAMETER
	B << vec_UVW.at(0)->x << vec_UVW.at(0)->y << 1.0 << arma::endr
		<< vec_UVW.at(1)->x << vec_UVW.at(1)->y << 1.0 << arma::endr
		<< vec_UVW.at(2)->x << vec_UVW.at(2)->y << 1.0 << arma::endr;

	//TRANSFORMATION MATRIX
	arma::mat T = arma::eye<arma::mat>(3, 3);
	T = arma::solve(A, B);
	T = T.t();

	return T;
}
ntVec3*   ntPanel::map_UVW(arma::mat T, ntVec3* xyz) {
	///////////////////////////////////////////////////////////////
	// AFFINE TRANSFORMATION OF POINT IN XYZ TO UVW
	// RETURNS POINT IN UVW COORDINATE SPACE
	// EMPTY PARAMETER FOR TRANSFORMATION
	arma::mat UVW = arma::zeros(3, 1);
	UVW(0) = xyz->x;
	UVW(1) = xyz->y;
	UVW(2) = 1.0;

	//UVW PARAMETER AFTER MAPPING
	UVW = T * UVW;
	ntVec3 *uvw = new ntVec3(UVW(0), UVW(1), UVW(2));
	return uvw;
}
ntVec3*   ntPanel::map_UVW(std::vector<ntVec3*> vec_XYZ, std::vector<ntVec3*> vec_UVW, ntVec3* xyz) {
	using namespace arma;

	///////////////////////////////////////////////////////////////
	// AFFINE TRANSFORMATION OF POINT IN XYZ TO UVW
	// RETURNS POINT IN UVW COORDINATE SPACE

	arma::mat A;	//XYZ POSITION
	A << vec_XYZ.at(0)->x << vec_XYZ.at(0)->y << 1.0 << arma::endr
		<< vec_XYZ.at(1)->x << vec_XYZ.at(1)->y << 1.0 << arma::endr
		<< vec_XYZ.at(2)->x << vec_XYZ.at(2)->y << 1.0 << arma::endr;

	arma::mat B;	//UVW PARAMETER
	B << vec_UVW.at(0)->x << vec_UVW.at(0)->y << 1.0 << arma::endr
		<< vec_UVW.at(1)->x << vec_UVW.at(1)->y << 1.0 << arma::endr
		<< vec_UVW.at(2)->x << vec_UVW.at(2)->y << 1.0 << arma::endr;

	//TRANSFORMATION MATRIX
	arma::mat T = eye<mat>(3, 3);
	T = solve(A, B);
	T = T.t();

	//EMPTY PARAMETER
	arma::mat UVW = zeros(3, 1);

	UVW(0) = xyz->x;
	UVW(1) = xyz->y;
	UVW(2) = 1.0;

	//UVW PARAMETER AFTER MAPPING
	UVW = T * UVW;

	ntVec3 *uvw = new ntVec3(UVW(0), UVW(1), UVW(2));

	return uvw;
}

bool ntPanel::pt_isInside(ntVec3* point) {
	int i, j, nvert = verts.size();
	bool c = false;

	for (i = 0, j = nvert - 1; i < nvert; j = i++) {
		if (((vecs[i]->y >= point->y) != (vecs[j]->y >= point->y)) &&
			(point->x <= (vecs[j]->x - vecs[i]->x) * (point->y - vecs[i]->y) / (vecs[j]->y - vecs[i]->y) + vecs[i]->x)
			)
			c = !c;
	}
	return c;
}
ntVec3* ntPanel::pt_Mid(ntVec3* v0, ntVec3* v1) {
	float x = (v0->x + v1->x) * 0.5;
	float y = (v0->y + v1->y) * 0.5;
	float z = (v0->z + v1->z) * 0.5;

	ntVec3 * v = new ntVec3(x, y, z);
	return v;
}

///////////////////////////////////////////////////////////////
/////////////////////////// DISPLAYS 2D TRANSFORMATION OF PANEL
void ntPanel::display(){
	glBegin(GL_TRIANGLES);
	glColor4f(vert0->col.r, vert0->col.g, vert0->col.b, vert0->col.a);
	glVertex3f(v0->x, v0->y, v0->z);
	glColor4f(vert1->col.r, vert1->col.g, vert1->col.b, vert1->col.a);
	glVertex3f(v1->x, v1->y, v1->z);	
	glColor4f(vert2->col.r, vert2->col.g, vert2->col.b, vert2->col.a);
	glVertex3f(v2->x, v2->y, v2->z);
	glEnd();
}
void ntPanel::display_Graph() {
	if (is_Graph == true) {
		graph.display();
	}
}

void ntPanel::display_Perf() {
	for (int i = 0; i < perfs.size(); i++) {
			perfs.at(i)->display();
	}
}
void ntPanel::display_Edge() {
	int gen = 2;

	edges.at(0).display(1);
	edges.at(1).display(1);
	edges.at(2).display(1);
	verts.at(0)->display(2);

}
void ntPanel::display_EdgeSd(int gen) {
	if (gen > 0) {
		ntColor4f col = ntColor4f(1, 1, 1, .2);
		if (gen <= faces_L.size()) {
			for (int i = 0; i < faces_L.at(gen)->size(); i++) {
				faces_L.at(gen)->at(i).edges.at(0).setCol(col);
				faces_L.at(gen)->at(i).edges.at(1).setCol(col);
				faces_L.at(gen)->at(i).edges.at(2).setCol(col);

				faces_L.at(gen)->at(i).edges.at(0).display();
				faces_L.at(gen)->at(i).edges.at(1).display();
				faces_L.at(gen)->at(i).edges.at(2).display();
			}
		}
		else {
			// EXCEPTION FOR EXCEEDING GERERATIONS WITHIN BOUNDS
			std::cout << gen << " EXCEEDS AVAILABLE GENERATIONS" << endl;
		}
	}
}
void ntPanel::display_Face_L(L_mode mode, int gen) {
	if (gen <= faces_L.size()) {
		for (int i = 0; i < faces_L.at(gen)->size(); i++) {
			faces_L.at(gen)->at(i).display(mode);
		}
	}
	else {
		// EXCEPTION FOR EXCEEDING GERERATIONS WITHIN BOUNDS
		std::cout << gen << " EXCEEDS AVAILABLE GENERATIONS" << endl;
		display_Face_L(mode, gen - 1);
	}
}

///////////////////////////////////////////////////////////////
////////////////////////// DISPLAYS 3D MODELSPACE VIEW OF PANEL
void ntPanel::display_Face_G(int gen) {
	if (gen <= faces_G.size()) {
		for (int i = 0; i < faces_G.at(gen)->size(); i++) {
			faces_G.at(gen)->at(i).display();
		}
	}
	else {
		// EXCEPTION FOR EXCEEDING GERERATIONS WITHIN BOUNDS
		display_Face_G(gen - 1);
	}
}

///p_Col.push_back(val);
/// WITHOUT SUBDIVISIONS // GRID MUST BE ASSOCIATED TO INDEPENDENT COLOR DATA STRUCTURE
///////////////////////////////////////////////////////////////
///// LOAD VERTEX COLOR IF OPTION SET TO VERTEX OR CENTROID COL
//float c0 = faces_L.at(gen)->at(k).vert0->col.r;
//float c1 = faces_L.at(gen)->at(k).vert1->col.r;
//float c2 = faces_L.at(gen)->at(k).vert2->col.r;
////	EVALUATE DISTANCE SQRD TO EACH VERTEX
//float d0 = faces_L.at(gen)->at(k).v0->distSqrd(vec)/3;
//float d1 = faces_L.at(gen)->at(k).v1->distSqrd(vec)/3;
//float d2 = faces_L.at(gen)->at(k).v2->distSqrd(vec)/3;
//float dT = d0 + d1 + d2/3;
//// MAP DISTANCE TO VERTEX WEIGHT
//float w0 = mapRange(0, 1, 0, dT, d0);
//float w1 = mapRange(0, 1, 0, dT, d1);
//float w2 = mapRange(0, 1, 0, dT, d2);

//val = (c0 * w0) + (c1 * w1) + (c2 * w2);
/////val = faces_L.at(gen)->at(i).col.r;  //CENTROID COLOR

