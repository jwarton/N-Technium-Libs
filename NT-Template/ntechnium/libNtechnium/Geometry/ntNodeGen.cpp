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

void ntNodeGen::gen_profiles() {
	int cnt_branch = edges.size();

	for (int i = 0; i < cnt_branch; i++) {
		//BRANCH PARAMETERS  /// MODIFY TO RECIEVE BRANCH OBJECT // PROPERTIES FOR THICKNESS
		ntVec3* v0 = edges[i]->v0;
		ntVec3* v1 = edges[i]->v1;
		ntVec3* vS = edges[i]->get_PtP(neck);
		ntVec3* vE = edges[i]->get_PtP(1 - neck);
		ntEdge seg = ntEdge(vS, vE);
		float len = seg.length();

		float dim0 = dimX;
		float dim1 = dimY;
		// BRANCH MEMBER | VARIABLE THICKNESS
		if (isUniform == false) {
			dim0 = len * dimX;
			dim1 = len * dimY;
		}
		/// TODO: STRESS OPTOMIZED THICKNESS !!!!!!

		ntVec3 axis_T = ntVec3(v0, v1);
		ntVec3 axis_S(0, 0, 1);

		for (int j = 0; j <= div; j++) {
			std::vector <ntVec3*> pts;
			// PLOT PROFILE VERTICES
			if (mode == POLYGON) {
				//pts = (gen_profile(0.1);
			}
			else if (mode == POLYPARAM) {
				profile = ntPolygon(dim0, sides);
			}
			else if (mode == RECTANGLE) {
				profile = ntRectangle(dim0, dim1);
			}
			else if (mode == SQUARE) {
				profile = ntSquare(dim0);
			}

			pts = profile.vecs;

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
std::vector <ntVec3*> ntNodeGen::gen_profile(float w) {
	// SQUARE PROFILE
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


void ntNodeGen::gen_branch() {
	int cnt_profiles = div + 1;

	for (int i = 0; i < edges.size(); i++) {
		for (int j = 0; j < div; j++) {

			int id = (i * cnt_profiles) + j;
			int cnt_pts = profiles[id]->get_Vecs().size() - 1;

			for (int k = 0; k < cnt_pts; k++) {
				ntPolyline* e = new ntPolyline(profiles[id]->get_Vecs()[k], profiles[id+1]->get_Vecs()[k]);
				profiles.push_back(e);
			}
		}
	}
}
void ntNodeGen::gen_node() {
	
	std::vector<ntEdge> poly_edges;
	int cnt_pts = node.vecs.size();

	// BUILD EDGES OF CONVEX HULL
	for (int i = 0; i < cnt_pts; i++) {
		for (int j = 0; j < cnt_pts; j++) {
			if (i != j) {
				ntVec3* v0 = node.vecs[i];
				ntVec3* v1 = node.vecs[j];
				for (int k = 0; k < poly_edges.size(); k++) {
					if (poly_edges[k].contains(v0) && poly_edges[k].contains(v1)) {
						// LINE EXISTS
						std::cout << "LINE PREVIOUSLY ADDED TO EDGE LIST" << endl;
					} else {
						ntEdge e = ntEdge(v0, v1);
						poly_edges.push_back(e);
					}
				}
			}
		}
	}
	// SORT FACES OF CONVEX HULL

}
void ntNodeGen::calc_thickness() {

}

void ntNodeGen::set_Color(ntColor4f col){
	this->col=col;
	for(int i = 0; i<verts.size(); i++){
		verts.at(i)->set_color(col);
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
void ntNodeGen::set_Profile(ntPolygon profile)
{
	this->profile = profile;
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

	node.display();
}
