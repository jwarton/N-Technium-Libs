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
			/// // SET UVW INTERPOLATION
			ntVec3 * uvw0 = faces->at(dim)->at(i).uvw0;
			ntVec3 * uvw1 = faces->at(dim)->at(i).uvw1;
			ntVec3 * uvw2 = faces->at(dim)->at(i).uvw2;
			/// CALCULATE UVW POINT BETWEEN
			ntVec3 * uvw3 = pt_Mid(uvw0,uvw1);
			ntVec3 * uvw4 = pt_Mid(uvw1,uvw2);
			ntVec3 * uvw5 = pt_Mid(uvw2,uvw0);
			//NEW FACES FROM VECS POINTERS
			ntFace3 f0 = ntFace3(v0, n0, n2);
			ntFace3 f1 = ntFace3(v1, n1, n0);
			ntFace3 f2 = ntFace3(v2, n2, n1);
			ntFace3 f3 = ntFace3(n0, n1, n2);
			/// SET UVW FOR EACH VERTEX IN GROUP
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
void ntPanel::sub_Div(int div, bool isPerf) {
	using namespace arma;
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
	int cols = div - 1;
	int rows = div - 1;
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

		//face.setUVW(&UV0, &UV1, &UV2);
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

		//AFFINE TRANSFORMATION OF POINT IN XYZ TO UVW
		arma::mat A;	//XYZ POSITION
		A	<< vecs_SD.at(0)->x << vecs_SD.at(0)->y << 1.0 << arma::endr
			<< vecs_SD.at(1)->x << vecs_SD.at(1)->y << 1.0 << arma::endr
			<< vecs_SD.at(2)->x << vecs_SD.at(2)->y << 1.0 << arma::endr;
		
		arma::mat B;	//UVW PARAMETER
		B	<< UV0.x << UV0.y << 1.0 << arma::endr
			<< UV1.x << UV1.y << 1.0 << arma::endr
			<< UV2.x << UV2.y << 1.0 << arma::endr;

		//TRANSFORMATION MATRIX
		arma::mat T = eye<mat>(3, 3);
		T = solve(A, B);
		T = T.t();

		arma::mat UVW =	zeros(3,1);
		///////////////////////////////////////////////////////////////
		//////////////////////////////////// PLOT PERFORATION POSITIONS
		for (int i = 1; i < rows; i++) {
			std::vector <ntVec3*>* p_vecs = new vector <ntVec3*>;
			std::vector <ntVec3*>* p_uvws = new vector <ntVec3*>;
			for (int j = 1; j < cols; j++) {

				/// DEFINE POSITION OFFSET FOR X
				vec = new ntVec3((step_X * j) + (shif_X * i), (step_Y * i), 0);
				p_vecs->push_back(vec);
				vecs_SD.push_back(vec);

				UVW(0) = vec->x;
				UVW(1) = vec->y;
				UVW(2) = 1.0;
				UVW = T * UVW;

				uvw = new ntVec3(UVW(0), UVW(1), 0);

				p_uvws->push_back(uvw);
				p_UVs.push_back(uvw);

				if (j == (cols - 1)) {
					cols = cols - 1;
				}
			}
			p_Rows.push_back(p_vecs);
			p_UV_Rows.push_back(p_uvws);
		}
		//std::cout << endl;
		int uvSize = p_UV_Rows.size();
		if (stoi(panel_ID)<1) {
			std::cout << "NUMBER OF ROWS:             " << uvSize << endl;
			std::cout << "NUMBER OF COLUMNS:          " << p_UV_Rows.at(0)->size() << endl;
		}

		///////////////////////////////////////////////////////////////
		///////////////////////////// ADD FACES TO NEW SUBDIVISION LIST
		std::vector <ntFace3>* faces = new vector<ntFace3>;

		for (int row = 0; row < (p_Rows.size()-1); row++) {
			for (int col = 0; col < (p_Rows.at(row)->size()-1); col++) {

				ntVec3 * vec0 = p_Rows.at(row)->at(col);
				ntVec3 * vec1 = p_Rows.at(row)->at(col + 1);
				ntVec3 * vec2 = p_Rows.at(row + 1)->at(col);
				ntVec3 * uvw0 = p_UV_Rows.at(row)->at(col);
				ntVec3 * uvw1 = p_UV_Rows.at(row)->at(col + 1);
				ntVec3 * uvw2 = p_UV_Rows.at(row + 1)->at(col);

				ntFace3 f0 = ntFace3(vec0, vec1, vec2);

				f0.setUVW(uvw0, uvw1, uvw2);
				faces->push_back(f0);
			}			
		}
		faces_L.push_back(faces);
	}
	is_PerfSD = isPerf;
}
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
/// PREVIOUS VERSION TO REFACTOR
void ntPanel::calc_Perf_00() {
	///////////////////////////////////////////////////////////////
	///////////////// CALCULATE GRID POSTION ORTHOGRAPHIC TO EDGE-0
	float w = (v1->x - v0->x) - (edge_Offset * 2);
	float spX = (r_Max * 2) + .125;
	float spY = (r_Max * 2) - .25;
	int x_Div = ceil((v1->x - v0->x) / spX) + 5;
	int y_Div = ceil((v2->y - v0->y) / spY) + 5;
	float y;
	float x;
	float r;
	ntVec3* vec;

	bool isPtInSd = false;
	float valCol = 0;

	for (int i = 0; i <= x_Div; i++) {
		for (int j = 0; j <= y_Div; j++) {

			if (j % 2 == 0) {
				x = v2->x - (spX * x_Div * 0.5) + (spX * i) + (spX * 0.5);
			}
			else {
				x = v2->x - (spX * x_Div * 0.5) + (spX * i); 
			}
			y = (spY * j) + edge_Offset;

			// SET ACTIVE SUBDIVISION GENERATION
			int gen = cnt_SubDiv;
			vec = new ntVec3(x, y, 0);

			if (pt_isInside(vec)) {
				int dim = faces_L.at(gen)->size(); // SIZE OF SUB PANELS

				for (int k = 0; k < dim; k++) {
					isPtInSd = faces_L.at(gen)->at(k).pt_isInside(vec);
					if (isPtInSd == true) {

						// LOAD VERTEX COLOR
						float c0 = faces_L.at(gen)->at(k).vert0->col.r;
						float c1 = faces_L.at(gen)->at(k).vert1->col.r;
						float c2 = faces_L.at(gen)->at(k).vert2->col.r;
						//	EVALUATE DISTANCE SQRD TO EACH VERTEX
						float d0 = faces_L.at(gen)->at(k).v0->distSqrd(vec)/3;
						float d1 = faces_L.at(gen)->at(k).v1->distSqrd(vec)/3;
						float d2 = faces_L.at(gen)->at(k).v2->distSqrd(vec)/3;
						float dT = d0 + d1 + d2/3;
						// MAP DISTANCE TO VERTEX WEIHTS
						float w0 = mapRange(0, 1, 0, dT, d0);
						float w1 = mapRange(0, 1, 0, dT, d1);
						float w2 = mapRange(0, 1, 0, dT, d2);

						valCol = (c0 * w0) + (c1 * w1) + (c2 * w2);
						///valCol = faces_L.at(gen)->at(i).col.r;  //CENTROID COLOR

						r = valCol * ((rand() % 10)*.1);
						r = round(r * 10) * 0.1;
						if (r > 1) {
							r = 1;
						}
						r = mapRange(r_Min, r_Max, 0, 1, r);
						if (r > r_Min) {
							p_Pos.push_back(vec);
							p_Rad.push_back(r);
						}
						k = dim;
					}
				}
			}
		}
	}
	add_Perf();
}
void ntPanel::calc_Perf_SD(int div) {
	///////////////////////////////////////////////////////////////
	///////////// CALCULATE GRID POSTION BY TRIANGULAR SUBDIVISIION
	ntVec3* vec;
	float r;
	bool isPtInSd = false;
	float valCol = 0;

	/// CALL PANEL SUDIVISION(ROWS)
	/// sub_Div(50, true);
	/// /////////////////////////////////////////////////////////////////////////////////////////////////
	int cols = div-1;
	int rows = div-1;
	bool isDiv = true;

	float step_X;
	float shif_X;
	float step_Y;

	if (isDiv) {
		///////////////////////////////////////////////////////////////
		// COPY PANEL CORNER POINTS
		ntVec3 v00 = ntVec3(vecs_SD.at(0)->x, vecs_SD.at(0)->y, vecs_SD.at(0)->z);
		ntVec3 v01 = ntVec3(vecs_SD.at(1)->x, vecs_SD.at(1)->y, vecs_SD.at(1)->z);
		ntVec3 v02 = ntVec3(vecs_SD.at(2)->x, vecs_SD.at(2)->y, vecs_SD.at(2)->z);

		ntFace3 face = ntFace3(&v00, &v01, &v02);

		// SCALE CORNER POINTS TO SUBDIVISION INCREMENT
		float scFactor = 1.0 / div;
		for (int i = 0; i < 3; i++) {
			ntMatrix4 matSc = ntMatrix4(face.vecs[i]);
			matSc.scale3d(scFactor);
		}

		step_X = v01.x;
		shif_X = v02.x;
		step_Y = v02.y;

		// PLOT PERFORATION POSITIONS
		for (int i = 1; i < rows; i++) {
			std::vector <ntVec3*>* p_vecs = new vector <ntVec3*>;
			for (int j = 1; j < cols; j++) {
				/// DEFINE POSITION OFFSET FOR X
				vec = new ntVec3((step_X * j) + (shif_X * i), (step_Y * i), 0);
				p_vecs->push_back(vec);
				if (j == (cols - 1)) {
					cols = cols - 1;
				}
			}
			p_Rows.push_back(p_vecs);
		}
		/// /////////////////////////////////////////////////////////////////////////////////////////////////
		for (int i = 0; i < p_Rows.size(); i++) {
			for (int j = 0; j < p_Rows.at(i)->size(); j++) {
				vec = p_Rows.at(i)->at(j);
				///////////////////////////////////////////////////////////////
				/////////////////// COORDINATE PERF LOCATION TO SUBDIVIDED AREA
				int gen = cnt_SubDiv;
				// QUANTITY OF SUB-PANELS
				int dim = faces_L.at(gen)->size();

				for (int k = 0; k < dim; k++) {
					isPtInSd = faces_L.at(gen)->at(k).pt_isInside(vec);
					if (isPtInSd == true) {

						// LOAD VERTEX COLOR
						float c0 = faces_L.at(gen)->at(k).vert0->col.r;
						float c1 = faces_L.at(gen)->at(k).vert1->col.r;
						float c2 = faces_L.at(gen)->at(k).vert2->col.r;
						float cT = c0 + c1 + c2;
						// EVALUATE DISTANCE SQRD TO EACH VERTEX
						float d0 = faces_L.at(gen)->at(k).v0->distSqrd(vec);
						float d1 = faces_L.at(gen)->at(k).v1->distSqrd(vec);
						float d2 = faces_L.at(gen)->at(k).v2->distSqrd(vec);
						float dT = d0 + d1 + d2;
						// MAP DISTANCE TO VERTEX WEIGHTS
						float w0 = mapRange(0, 1, 0, dT, d0, false);
						float w1 = mapRange(0, 1, 0, dT, d1, false);
						float w2 = mapRange(0, 1, 0, dT, d2, false);

						valCol = ((c0 * w0) + (c1 * w1) + (c2 * w2)) / 3;
						valCol = mapRange(0, 1, 0, (cT / 3), valCol);

						float fx = (((rand() % 10)*.01) - 0.05);

						r = valCol * (rand() % 10) * 0.1 + fx;
						r = round(r * 10) * 0.1;
						if (r > 1) {
							r = 1;
						}
						r = mapRange(r_Min, r_Max, 0, 1, r, false);
						if (r > r_Min) {
							p_Pos.push_back(vec);
							p_Rad.push_back(r);
						}
						k = dim;
					}
				}
			}
		}
		add_Perf();
	}
}
/// CURRENT VERSIONS OF CALC PERF
void ntPanel::calc_Perf_SD() {
	ntVec3* vec;
	bool isPtInSd = false;
	float val = 0;
	int gen = cnt_SubDiv;

	if (is_PerfSD == true){
		int dim = faces_L.at(gen)->size();
		int row_Length = p_Rows.at(0)->size()-1;
		/// //REORGANIZE DATA STRUCTURE OF FACES TO ALLOW ROW ACCESS
		///////////////////////////////////////////////////////////////
		///////////// PLOT PERF LOCATION TO SUBDIVIDED VERTEX POSITIONS
		for (int i = 0; i < dim; i++) {
			if (i < row_Length) {
				vec = faces_L.at(gen)->at(i).vecs[0];
				val = faces_L.at(gen)->at(i).verts[0]->col.r;
				calc_Perf_R(vec, val);
			}
			if (i == (row_Length-1)) {
				vec = faces_L.at(gen)->at(i).vecs[1];
				val = faces_L.at(gen)->at(i).verts[1]->col.r;
				calc_Perf_R(vec, val);
			}
			vec = faces_L.at(gen)->at(i).vecs[2];
			val = faces_L.at(gen)->at(i).verts[2]->col.r;
			calc_Perf_R(vec, val);
		}
	}
	add_Perf();
}
void ntPanel::calc_Perf_R(Vec3 *vec, float val){
	float r;
	float fx = ((rand() % 10)*.01); //(((rand() % 10)*.01) - 0.05);
	bool isRandom = false;

	if (isRandom == true) {
		r = val - fx;
	}
	else {
		r = val;
	}

	r = round(r * 10) * 0.1;
	
	if (r > 1) {
		r = 1;
	}

	float min = -0.125;
	r = mapRange(min, r_Max, 0, 1, r, false);

	if (r >= (r_Max - 0.05)) {
		r = r_Max;
	}

	if (r > r_Min) {
		p_Pos.push_back(vec);
		p_Rad.push_back(r);
	}
}

void ntPanel::add_Perf() {
	if (p_Pos.size() > 0) {
		for (int i = 0; i < p_Pos.size(); i++) {
			float r = p_Rad.at(i); 
			ntCircle * perf = new ntCircle(p_Pos.at(i), r, n_seg, Col4(.25, .25, .25, 1));
			perfs.push_back(perf);
		}
	} 
	if (stoi(panel_ID) < 1) {
		//std::cout << "number of rows:  " << i << endl;
		std::cout << "PERFORATIONS PER PANEL:     " << perfs.size() << endl;
	}
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
			display_Face_L(gen - 1);
		}
	}
}
void ntPanel::display_Face_L(int gen) {
	ntColor4f col = ntColor4f(1, 1, 1, .5);
	if (gen <= faces_L.size()) {
		for (int i = 0; i < faces_L.at(gen)->size(); i++) {

			faces_L.at(gen)->at(i).display();

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
		display_Face_L(gen - 1);
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
