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

	//INITIALIZE VECS- TRANSFORM LIST !!!!!
	//THESE VALUES ARE NOT INITIALIZED !!!!
	//this->vecs[3] = c0;
	//this->vecs[4] = c1;
	//this->vecs[5] = c2;
	//this->vecs[6] = c3;
	//this->vecs[7] = c4;

	//this->vecs[8 ] = f0;
	//this->vecs[9 ] = f1;
	//this->vecs[10] = f2;
	//this->vecs[11] = f3;
	//this->vecs[12] = f4;

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
void ntPanel::calcArea() {
	std::vector <double> xy_pairs;
	std::vector <double> yx_pairs;
	std::vector <double> v_;
	double sum = 0;
	area = 0;

	//MULTIPLY VERTEX COMPONENTS
	for (int i = 0; i < verts.size() - 1; i++) {
		double xy_ = vecs[i]->x * vecs[i + 1]->y;
		double yx_ = vecs[i]->y * vecs[i + 1]->x;
		xy_pairs.push_back(xy_);
		yx_pairs.push_back(yx_);
	}
	//SUBTRACT MULTIPLICATION SETS
	for (int i = 0; i < verts.size() - 1; i++) {
		double val = xy_pairs.at(i) - yx_pairs.at(i);
		v_.push_back(val);
	}
	//SUMMATION OF ALL SETS
	std::for_each(v_.begin(), v_.end(), [&](int n) { sum += n; });
	area = abs(sum * 0.5);
	//std::cout << sum << " _ " << area << endl;
}
void ntPanel::calcPhi() {
	double theta;
	int ind1 = 1;
	int ind2 = 2;
	ntVec3* v  = new ntVec3(0, 0, 0);
	ntVec3* d1 = new ntVec3(0, 0, 0);
	ntVec3* d2 = new ntVec3(0, 0, 0);

	for (int i = 0; i < 3; i++) {
		v->set(vecs[i]);
		d1->set(vecs[ind1]);
		d2->set(vecs[ind2]);
		
		d1->sub(v);
		d2->sub(v);

		d1->unitize();
		d2->unitize();

		theta = toDegrees(d1->angle(d2));
		phi.push_back(theta);

		ind1 += 1;
		ind2 += 1;

		if (ind1 == 3) {
			ind1 = 0;
		}
		if (ind2 == 3) {
			ind2 = 0;
		}
	}
	delete v, d1, d2;
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
	int n_cols = div + 1;
	int n_rows = div + 1;
	float step_X;
	float shif_X;
	float step_Y;
	float orig_X;
	float orig_Y;

	if (isPerf) {
		///////////////////////////////////////////////////////////////
		////////////////////////////////////// COPY PANEL CORNER POINTS
		ntVec3	v00 = ntVec3(f_L.at(0)->x, f_L.at(0)->y, f_L.at(0)->z);
		ntVec3	v01 = ntVec3(f_L.at(1)->x, f_L.at(1)->y, f_L.at(1)->z);
		ntVec3	v02 = ntVec3(f_L.at(2)->x, f_L.at(2)->y, f_L.at(2)->z);
		
		ntFace3 face = ntFace3(&v00, &v01, &v02);

		///////////////////////////////////////////////////////////////
		/////////////////////////// COPY PANEL UV PARAMETERS AT CORNERS
		ntVec3 UV0 = ntVec3(vecs_UV.at(0)->x, vecs_UV.at(0)->y, vecs_UV.at(0)->z);
		ntVec3 UV1 = ntVec3(vecs_UV.at(1)->x, vecs_UV.at(1)->y, vecs_UV.at(1)->z);
		ntVec3 UV2 = ntVec3(vecs_UV.at(2)->x, vecs_UV.at(2)->y, vecs_UV.at(2)->z);

		///////////////////////////////////////////////////////////////
		////////////////// SCALE CORNER POINTS TO SUBDIVISION INCREMENT
		orig_X = v00.x;
		orig_Y = v00.y;

		double scFactor = 1.0 / div;
		for (int i = 0; i < 3; i++) {
				/// TRANSLATE FACE TO ORIGIN
				face.vecs[i]->x -= orig_X;
				face.vecs[i]->y -= orig_Y;
				/// SCALE FACE TO SUBDIVISION INCREMENT
				ntMatrix4 matSc = ntMatrix4(face.vecs[i]);
				matSc.scale3d(scFactor);
				/// TRANSLATE TO ORIGINAL POSITION
				face.vecs[i]->x += orig_X;
				face.vecs[i]->y += orig_Y;
		}

		step_X = v01.x - orig_X;
		shif_X = v02.x - orig_X;
		step_Y = v02.y - orig_Y;

		std::vector <ntVec3*> vecs_F;
		vecs_F.push_back(&v00);
		vecs_F.push_back(&v01);
		vecs_F.push_back(&v02);

		////AFFINE TRANSFORMATION OF POINT IN XYZ TO UVW
		arma::mat T = arma::eye<arma::mat>(3, 3);
		T = get_Mat_T(vecs_SD, vecs_UV);

		///////////////////////////////////////////////////////////////
		//////////////////////////////////// PLOT SUBDIVISION POSITIONS
		for (int i = 0; i < n_rows; i++) {
			std::vector <ntVec3*>* p_vecs = new vector <ntVec3*>;
			std::vector <ntVec3*>* p_uvws = new vector <ntVec3*>;
			for (int j = 0; j < n_cols; j++) {
				// DEFINE POSITION OFFSET FOR X
				vec = new ntVec3(orig_X + (step_X * j) + (shif_X * i), orig_Y + (step_Y * i), 0);
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
				
				//if(is_Odds == true  && j < (p_XY_Rows.at(i)->size() - 2)){
				//	ntVec3 * vec3 = p_XY_Rows.at(i + 1)->at(j + 1);
				//	ntVec3 * uvw3 = p_UV_Rows.at(i + 1)->at(j + 1);
				//
				//	ntFace3 f1 = ntFace3(vec2, vec3, vec1);

				//	f1.setUVW(uvw2, uvw3, uvw1);
				//	faces->push_back(f1);
				//}
			}			
		}
		faces_L.push_back(faces);
	}
	is_PerfSD = isPerf;
}
///////////////////////////////////////////////////////////////
void ntPanel::plot_Fast(int div){

	double len;
	double inc;
	double param = 0;

	//////////////////////////////////////////////////
	/// DEFINE FASTENER EDGE CENTERLINE
	for (int i = 3; i < 6; i++) {

		std::vector<ntVec3*> pts_P;
		std::vector<ntVec3*> pts_F;
		len = edges.at(i).getLength();
		inc = len / div;
		inc = mapRange(0, 1, 0, len, inc);
		ntVec3* vS;
		ntVec3* vE;

		/// PLOT PERF GRID POINTS ALONG CENTERLINE
		for (int j = 0; j < div; j++) {
			param = inc * j;
			ntVec3* pt = edges.at(i).get_PtP(param);
			pts_P.push_back(pt);

			ntCircle * fastener = new ntCircle(pt, 0.05, n_seg, Col4(1, 0, 0, 1));
			fastr.push_back(fastener);
		}
		/// REDEFINE CENTERLINE END POINTS
		int ind = 5;
		ntVec3* vecS = edges.at(i).v0;
		ntVec3* vecE = edges.at(i).v1;

		vS = eval_Fastener(pts_P, vecS, ind, false);
		vE = eval_Fastener(pts_P, vecE, ind, true);
		ntEdge e = ntEdge(vS, vE);

		/// PLOT IDEAL FASTENER POINTS BASED ON SET SPACING
		int val = ceil(e.getLength() / fast_spaceMax);
		inc = len / val;
		inc = mapRange(0, 1, 0, len, inc);
		for (int j = 0; j < val +1; j++) {
			param = inc * j;
			ntVec3* pt = e.get_PtP(param);
			pts_F.push_back(pt);
		}

		/// REPOSITION FASTENERS TO PERF NEAREST PERF GRID
		for (int j = 0; j < pts_F.size(); j++) {
			int index;
			double distMin = pow(len,2) * 2;

			for (int k = 0; k < pts_P.size(); k++) {
				double dist = pts_F.at(j)->distSqrd(pts_P.at(k));
				if (dist <= distMin) {
					distMin = dist;
					index = k;
				}
			}
			/// REVISED POSITIOIN
			float x = pts_P.at(index)->x;
			float y = pts_P.at(index)->y;
			float z = pts_P.at(index)->z;

			ntVec3* pt = new ntVec3(x,y,z);
			f_Pos.push_back(pt);
		}
	}
	/// ADD DISPLAY SYMBOLS FOR EACH FASTENER
	float rad = ((r_Max - r_Min) * 0.5) + r_Min;
	for (int i = 0; i < f_Pos.size(); i++) {
		ntVec3* pt = f_Pos.at(i);
		ntCircle * fastener = new ntCircle(pt, rad, n_seg, Col4(.5, 0, 0, 1));
		fastr.push_back(fastener);
	}
}
void ntPanel::plot_Perf(int div, grid_Type grid, perf_Type type) {
	grid_type = grid;
	perf_type = type;

	if (grid == TRI) {
		plot_Perf_SD(div);
	}
	else if (grid == DIA || grid == SQU) {
		plot_Perf_GR(div, grid);
	}
	//plot_Fast(div);
}
void ntPanel::plot_Perf_GR(int div, enum grid_Type grid) {
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
				p_Col.push_back(1);
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
				p_Col.push_back(1);
			}
		}
	}
}

ntVec3* ntPanel::eval_Fastener(std::vector <ntVec3*> vecs, ntVec3* vec, int index, bool isAsc) {
	ntVec3 * pt = vecs.at(index);
	float d1 = pt->distSqrd(vec);
	float d2 = pow(6, 2);

	if ( d1 > d2 && isAsc == false) {
		index -= 1;
		pt = eval_Fastener(vecs, vec, index, isAsc);
	} else if ( d1 > d2 && isAsc == true ) {
		index += 1;
		pt = eval_Fastener(vecs, vec, index, isAsc);
	}
	return pt;
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
	float min = inc; //-6 * inc;  // r_Min == 0.1625 == 3 * inc [ if min == r_Min = 255]
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
	// //////////////////////// INSTANTIATE POLYLINE DATA STRUCTURE 
	if (perf_type == TRICELL) {

		/// SET TRICELLS
		set_TriCells();
		

		int index = perf_style; // perf_type INDEX
		for (int i = 0; i < cells_L.size(); i++) {
			int cnt = cells_L.at(i).get_Polylines()[index].size();
			for (int j = 0; j < cnt; j++) {
				ntPolyline* pl = cells_L.at(i).get_Polylines()[index][j];
				lines.push_back(pl);
			}
		}
	}

	///////////////////////////////////////////////////////////////
	// LOOP THROUGH POS|RAD DATA STRUCTURES AND CREATE PERF OBJECTS
	//if (perf_type == DOT) {
		if (p_Pos.size() > 0) {
			for (int i = 0; i < p_Pos.size(); i++) {
				ntVec3* vec = p_Pos.at(i);
				float	val = p_Col.at(i);
				float	r	= calc_Perf_R(vec, val);

				p_Rad.push_back(r);	

				bool isUnique = true;
				double rSq = pow(r,2);

				// OMMIT PERFORATIONS WITHIN FASTENER LOCATIONS
				for (int j = 0; j < f_Pos.size(); j++) {
					double distSq = vec->distSqrd(f_Pos.at(j));
					if (distSq < rSq) {
						isUnique = false;
						r = 0;
						p_Rad.back() = r;
					}
				}

				// ADD RADIUS PERFORATIONS TO VECTOR
				if (r > 0 && isUnique == true) {
					ntCircle * perf = new ntCircle(vec, r, n_seg, Col4(.25, .25, .25, 1));
					perfs.push_back(perf);
					perf_area += perf->get_Area();
				}
			}
		//}
		perf_perc = (perf_area / area) * 100;
		perf_size = perfs.size();
	}
}
void ntPanel::reparam_UV() {
	float uMax, vMax;
	uMax = vecs_SD[1]->x;
	vMax = vecs_SD[2]->y;

	for (int i = 0; i < p_UVs.size(); i++) {
		p_UVs[i]->x = mapRange(0, 1, 0, uMax, vecs_SD[i]->x);
		p_UVs[i]->y = mapRange(0, 1, 0, vMax, vecs_SD[i]->y);
	}
}
///////////////////////////////////////////////////////////////
void ntPanel::set_Color(ntColor4f col){
	this->col = col;
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
void ntPanel::set_cG(ntVec3* pt, int index) {
	c_G.push_back(pt);
	/// SET LOCAL TO GLOBAL VALUES
	/// PRE TRANSFORM 
	ntVec3* pt_L = new ntVec3(pt->x, pt->y, pt->z);
	c_L.push_back(pt_L);
	vecs[index] = pt_L;
}
void ntPanel::set_fG(ntVec3* pt, int index) {

	f_G.push_back(pt);
	/// SET LOCAL TO GLOBAL VALUES
	/// PRE TRANSFORM
	ntVec3* pt_L = new ntVec3(pt->x, pt->y, pt->z);
	f_L.push_back(pt_L);
	vecs[index] = pt_L;
}
void ntPanel::set_fCLines() {
	int cnt = f_L.size();
	cnt = 3;	//TEMPORARY OVER WRITE  | 0,0,0 FOR NON-INIT POINTS FAULTY
	for (int i = 0; i < cnt; i++) {
		int ind_0 = i;
		int ind_1 = i+1;
		if (i == cnt - 1) {
			ind_1 = 0;
		}
		ntEdge e = ntEdge(f_L.at(ind_0), f_L.at(ind_1));
		e.setCol( ntCol4(1, 1, 1, 0.75));
		edges.push_back(e);
	}
}
void ntPanel::set_UVW(string string_UVW){
	this->string_UVW = string_UVW;
}
void ntPanel::set_UVW(std::vector <ntVec3*>	vecs_UV) {
	this->vecs_UV = vecs_UV;
	faces_G.at(0)->at(0).setUVW(vecs_UV);
	faces_L.at(0)->at(0).setUVW(vecs_UV);
}
void ntPanel::set_TriCells() {
	int dim = faces_G.size();
	int cnt = faces_G.at(dim - 1)->size();
	for (int i = 0; i < cnt; i++) {
		ntFace3 f = faces_G.at(dim - 1)->at(i);
		ntTriCell cell = ntTriCell(&f);
		float scFx = f.getFx();
		scFx = 1 - scFx;
		cell.setScale(scFx);
		cells.push_back(cell);
	}
	dim = faces_L.size();
	cnt = faces_L.at(dim - 1)->size();
	/// ///  REVISE DATA STRUCTURE FOR LOCAL FACE TRICELLS !!!!!!!!!!!!!
	/// ///  CREATES UNREFERENCED VECS AND UNNECESSARY COPIES !!!!!!!!!!
	for (int i = 0; i < cnt; i++) {
		ntFace3 ft = faces_L.at(dim - 1)->at(i);
		ntTriCell cell = ntTriCell(&ft);
		float scFx = ft.getFx();
		scFx = 1 - scFx;
		cell.setScale(scFx);
		cells_L.push_back(cell);
	}
}
void ntPanel::set_PerfStyle(perf_Style style) {
	perf_style = style;
}
void ntPanel::set_PerfType(perf_Type type) {
	perf_type = type;
}
void ntPanel::set_Dir(string dir) {
	this->dir = dir;
}
void ntPanel::set_Weight(string wht) {
	weight = std::stof(wht);
}
void ntPanel::set_Region(string reg){
	region = std::stoi(reg);
}
void ntPanel::set_IMG(float val) {
	std::cout << val << endl;
	image_Val = val;
}
void ntPanel::set_Graph() {
	///////////////////////////////////////////////////////////////////////
	/////////////////////////////////////// GRAPH PERFORATION | PANELS DATA
	ntVec3 *graphPos = new ntVec3(5, 400, 0);
	ntVec3 *graphDim = new ntVec3(448, 40, 0);

	/// int dim =		vecs_SD.size() - 1;  //USED FOR MAX SCALE OF GRAPH DIM Y
	int set_size =  p_Rad.size()   - 1;
	if (set_size > 0) {
		graph = ntGraph(graphPos, graphDim, p_Rad);
		graph.sort();
		graph.init();
		is_Graph = true;
	}
	
	ntVec3 *graph_A_Pos = new ntVec3(5, 392, 0);
	ntVec3 *graph_A_Dim = new ntVec3(448, 2, 0);

	graph_A = ntGraph(graph_A_Pos, graph_A_Dim, Y_axis);
	graph_A.set_Param(area, 0);
	graph_A.set_Param(area - perf_area, 1);

	graph_A.set_Width(4);
	graph_A.init();
}
///////////////////////////////////////////////////////////////
string ntPanel::get_ID() {
	return panel_ID;
}
string ntPanel::get_n_G() {
	return n_G;
}

string ntPanel::get_UVW() {
	return string_UVW;
}
string ntPanel::get_Dir() {
	return dir;
}
float ntPanel::get_MeanVal() {
	float mean = std::accumulate(p_Col.begin(), p_Col.end(), 0.0) / p_Col.size();
	return mean;
}
float ntPanel::get_Area() {
	return area;
}
float ntPanel::get_AngleMin() {
	float val = phi[0];
	for (int i = 1; i < phi.size(); i++) {
		if (val > phi[i]) {
			val = phi[i];
		}
	}
	return val;
}
float ntPanel::get_AngleMax() {
	float val = phi[0];
	for (int i = 1; i < phi.size(); i++) {
		if (val < phi[i]) {
			val = phi[i];
		}
	}
	return val;
}
float ntPanel::get_EdgeMin() {
	set_fCLines();		///TEMPORARY SOLUTION !!!!!!!!!!!!
	float lenMin = 999999999999;// v1->x;

	for (int i = 3; i < 6; i++) {
		float len = edges[i].getLength();
		if (len < lenMin) {
			lenMin = len;
		}
	}
	return lenMin;
}
float ntPanel::get_Weight() {
	return weight;
}
int ntPanel::get_Region() {
	return region;
}
std::vector<ntVec3*> ntPanel::get_v_G() {
	return v_G;
}
std::vector<ntVec3*> ntPanel::get_c_G() {
	return c_G;
}
std::vector<ntVec3*> ntPanel::get_c_L() {
	return c_L;
}
std::vector<ntVec3*> ntPanel::get_f_L() {
	return f_L;

}
std::vector<ntPolyline*> ntPanel::get_PLin() {
	return lines;
}
std::vector<ntVec3*> ntPanel::get_Perf() {
	return p_Pos;
}
std::vector<float> ntPanel::get_Perf_R() {
	return p_Rad;
}
std::vector<ntVec3*> ntPanel::get_Fast() {
	return f_Pos;
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
		graph_A.display();
	}
}

void ntPanel::display_Perf() {
	if (perf_type == DOT) {
		for (int i = 0; i < perfs.size(); i++) {
			perfs.at(i)->display();
		}
	}
	if (perf_type == TRICELL) {
		for (int i = 0; i < cells_L.size(); i++) {
			cells_L.at(i).displaySubD(perf_style);
		}
	}
}
void ntPanel::display_Nodes() {
	verts.at(0)->display(2);
}
void ntPanel::display_Joint() {
	for (int i = 0; i < fastr.size(); i++) {
		fastr.at(i)->display();
	}
}
void ntPanel::display_CL() {
	glLineWidth(1);
	glColor4f(1, 1, 1, 0.25);

	glBegin(GL_LINES);
	glVertex3f(c_L[0]->x, c_L[0]->y, c_L[0]->z);
	glVertex3f(c_L[1]->x, c_L[1]->y, c_L[1]->z);

	glVertex3f(c_L[1]->x, c_L[1]->y, c_L[1]->z);
	glVertex3f(c_L[2]->x, c_L[2]->y, c_L[2]->z);

	glVertex3f(c_L[2]->x, c_L[2]->y, c_L[2]->z);
	glVertex3f(c_L[0]->x, c_L[0]->y, c_L[0]->z);
	glEnd();

	glColor4f(1, 0, 0, 0.25);
	glBegin(GL_LINES);
	glVertex3f(f_L[0]->x, f_L[0]->y, f_L[0]->z);
	glVertex3f(f_L[1]->x, f_L[1]->y, f_L[1]->z);

	glVertex3f(f_L[1]->x, f_L[1]->y, f_L[1]->z);
	glVertex3f(f_L[2]->x, f_L[2]->y, f_L[2]->z);

	glVertex3f(f_L[2]->x, f_L[2]->y, f_L[2]->z);
	glVertex3f(f_L[0]->x, f_L[0]->y, f_L[0]->z);
	glEnd();
}
void ntPanel::display_Edge() {
	edges.at(0).display(.5);
	edges.at(1).display(.5);
	edges.at(2).display(.5);

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
void ntPanel::display_EdgeSD_G(int gen) {
	if (gen > 0) {
		ntColor4f col = ntColor4f(1, 1, 1, .2);
		if (gen <= faces_G.size()) {
			for (int i = 0; i < faces_G.at(gen)->size(); i++) {
				faces_G.at(gen)->at(i).edges.at(0).setCol(col);
				faces_G.at(gen)->at(i).edges.at(1).setCol(col);
				faces_G.at(gen)->at(i).edges.at(2).setCol(col);

				faces_G.at(gen)->at(i).edges.at(0).display();
				faces_G.at(gen)->at(i).edges.at(1).display();
				faces_G.at(gen)->at(i).edges.at(2).display();
			}
			for (int i = 0; i < cells.size(); i++) {
				cells.at(i).displaySubD(perf_03);
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
			///// SHOW ODD
			//if (i % 2 == 0) {
			//	faces_L.at(gen)->at(i).display(mode);
			//}
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


