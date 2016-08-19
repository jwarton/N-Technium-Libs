#include "ntNodeGen.h"

ntNodeGen::ntNodeGen(){}
ntNodeGen::ntNodeGen(std::vector<ntEdge*>* edges):
edges(*edges){
	///init();
}
ntNodeGen::ntNodeGen(std::vector<ntEdge*> edges) :
	edges(edges) {
	///init();
}
void ntNodeGen::init(){
	// SORT BRANCHES
	// IF BRANCH/NODE RELATIONSHIP IS NOT WELL ORDERED
	///////////////////////////////////////////////////////////////
	//////////////////////////////////////////// BUILD BRANCH CAGES

	//std::cout << "ACTIVE PROFILE MODE:  SQUARE" << endl;
	//std::cout << "ACTIVE PROFILE MODE:  POLYGON" << endl;
	//std::cout << "ACTIVE PROFILE MODE:  POLYGON PARAMETERS" << endl;
	//std::cout << "ACTIVE PROFILE MODE:  RECTANGLE" << endl;
	gen_profiles();
	gen_branch();
	///////////////////////////////////////////////////////////////
	/////////////////////////////////////////////// BUILD JUNCTIONS

	///////////////////////////////////////////////////////////////
	// SUBDIVIDE AND/OR SMOOTH
}
std::vector <ntVec3*> ntNodeGen::gen_profile(ntEdge* edge, float w) {
	std::vector <ntVec3*> pts;
	ntVec3 *p00 = new ntVec3( w,  w, 0);
	ntVec3 *p01 = new ntVec3(-w,  w, 0);
	ntVec3 *p10 = new ntVec3(-w, -w, 0);
	ntVec3 *p11 = new ntVec3( w, -w, 0);
	pts.push_back(p00);
	pts.push_back(p01);
	pts.push_back(p10);
	pts.push_back(p11);
	return pts;
}
std::vector <ntVec3*> ntNodeGen::gen_profile(ntEdge* edge, int sides, float radius){
	// EXCEPTION FOR INVALID POLYGON SEGMENTS
	if (sides < 3) {
		sides = 3;
	}
	std::vector <ntVec3*> pts;
	// PLOT PROFILE POINTS
	int	cnt_pts = sides;
	for (int j = 0; j < cnt_pts; j++) {
		float theta = 2.0f * M_PI * j / sides;
		float x = radius * cosf(theta);
		float y = radius * sinf(theta);

		ntVec3 *pt = new ntVec3(x, y, 0);
		pts.push_back(pt);
	}
	return pts;
}
std::vector <ntVec3*> ntNodeGen::gen_profile(ntEdge* edge, float dim0, float dim1) {
		std::vector <ntVec3*> pts;
		ntVec3 *p00 = new ntVec3( dim0,  dim1, 0);
		ntVec3 *p01 = new ntVec3(-dim0,  dim1, 0);
		ntVec3 *p10 = new ntVec3(-dim0, -dim1, 0);
		ntVec3 *p11 = new ntVec3( dim0, -dim1, 0);
		pts.push_back(p00);
		pts.push_back(p01);
		pts.push_back(p10);
		pts.push_back(p11);
		return pts;
}
void ntNodeGen::gen_profiles(){
	int cnt_branch = edges.size();
	for (int i = 0; i < cnt_branch; i++) {
		//BRANCH PARAMETERS
		ntVec3* v0 = edges[i]->v0;
		ntVec3* v1 = edges[i]->v1;
		ntVec3* vS = edges[i]->get_PtP(neck);
		ntVec3* vE = edges[i]->get_PtP(1 - neck);
		ntEdge seg = ntEdge(vS, vE);
		float len = seg.length();

		float dim0 = len * dimX;
		float dim1 = len * dimY;

		ntVec3 axis_T = ntVec3(v0, v1);
		ntVec3 axis_S(0, 0, 1);

		for (int j = 0; j <= div; j++) {
			std::vector <ntVec3*> pts;
			// PLOT PROFILE VERTICES
			if (mode == SQUARE) {
				pts = gen_profile(edges[i], dim0);
			} else if (mode == POLYGON) {
				//pts = (gen_profile(edges[i], 0.1);
			} else if (mode == POLYPARAM) {
				pts = gen_profile(edges[i], sides, dim0);
			} else if (mode == RECTANGLE) {
				pts = gen_profile(edges[i], dim0, dim1);
			}
			//ALIGN PROFILE TO EDGE VECTOR AND TRANSLATE TO POSITION
			double pt_param = (1.0 / div) * j;
			ntVec3* vP = seg.get_PtP(pt_param);

			for (int k = 0; k < pts.size(); k++) {
				pts[k]->orient(&axis_T, &axis_S);
				pts[k]->add(vP);
			}

			ntPolyline *profile = new ntPolyline(pts, true);
			profiles.push_back(profile);
		}

	}
}
void ntNodeGen::gen_branch() {
	int cnt_profiles = div + 1;

	for (int i = 0; i < edges.size(); i++) {
		for (int j = 0; j < div; j++) {

			int id = (i * cnt_profiles) + j;
			int cnt_pts = profiles[id]->get_Vecs().size() - 1;

			for (int k = 0; k < cnt_pts; k++) {
				ntPolyline* e = new ntPolyline(profiles[id]->get_Vecs()[k], profiles[id+1]->get_Vecs()[k]);
				e->setCol(col);
				profiles.push_back(e);
			}
		}
	}
}

void ntNodeGen::set_Color(ntColor4f col){
	this->col=col;
	for(int i = 0; i<verts.size(); i++){
		verts.at(i)->setColor(col);
	}
}
void ntNodeGen::set_Parameters(SectMode mode, int div, double neck){
	this->mode = mode;

	if (div >= 1) {
		this->div = div;
	} else {
		this->div = 1;
		std::cout << "BRANCH DIVISIONS MUST BE >= TO 1" << endl;
	}

	if (neck < t_max) {
		this->neck = neck;
	} else {
		this->neck = t_max;
	}
}
void ntNodeGen::display(){
	for (int i = 0; i < 3; i++) {
		faces.at(i)->display();
	}
}
void ntNodeGen::display_Verts() {
	for (int i = 0; i < verts.size(); i++) {
		verts.at(i)->display();
	}
}
void ntNodeGen::display_Edges() {
	for (int i = 0; i < profiles.size(); i++) {
		profiles.at(i)->display();
	}
}
