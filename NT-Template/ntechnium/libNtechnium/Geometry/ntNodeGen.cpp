#include "ntNodeGen.h"

ntNodeGen::ntNodeGen(){}
ntNodeGen::ntNodeGen(std::vector<ntEdge*>* edges):
edges(*edges){
	init();
}
ntNodeGen::ntNodeGen(std::vector<ntEdge*> edges) :
	edges(edges) {
	init();
}
void ntNodeGen::init(){
	// SORT BRANCHES

	///////////////////////////////////////////////////////////////
	//////////////////////////////////////////// BUILD BRANCH CAGES
	int cnt_branch = edges.size();
	for (int i = 0; i < cnt_branch; i++) {
		if (mode == 0) {
			// SQUARE PROFILE PROPORTIONAL TO EDGE LENGTH
			gen_profile(edges[i], 0.05f);
		}
		else if (mode == 1) {
			// POLYGON PROFILE
			//gen_profile(edges[i], 0.1);
		}
		else if (mode == 2) {
			// POLYGON PARAMETERS, N-SIDES, RADIUS
			gen_profile(edges[i], 3, 0.05f);
		}
		else if (mode == 3) {
			// RECTANGLE PARAMETERS, X, Y
			gen_profile(edges[i], 0.05f, 0.025f);
		}
	}
	///////////////////////////////////////////////////////////////
	// BUILD JUNCTIONS
	gen_branch();
	///////////////////////////////////////////////////////////////
	// SUBDIVIDE AND/OR SMOOTH
}
void ntNodeGen::gen_profile(ntEdge* edge, float p) {
	//BRANCH PARAMETERS
	ntVec3* v0 = edge->v0;
	ntVec3* v1 = edge->v1;
	float l = edge->length();
	float w = l * p;
	int div = 5;
	float h = l / div;

	ntVec3 axis_T = ntVec3(v0,v1);
	ntVec3 axis_S(0, 0, 1);

	// BUILD PROFILES
	for (int i = 0; i < div; i++) {
		// 4 CORNER POINTS
		std::vector <ntVec3*> pts;
		ntVec3 *p00 = new ntVec3( w,  w, h*i);
		ntVec3 *p01 = new ntVec3(-w,  w, h*i);
		ntVec3 *p10 = new ntVec3(-w, -w, h*i);
		ntVec3 *p11 = new ntVec3( w, -w, h*i);

		pts.push_back(p00);
		pts.push_back(p01);
		pts.push_back(p10);
		pts.push_back(p11);

		ntPolyline *profile = new ntPolyline(pts, true);
		profiles.push_back(profile);

		//ALIGN PROFILE TO EDGE VECTOR
		//TRANSLATE PROFILE TO V0 POSITION
		for (int j = 0; j < 4; j++) {
			pts[j]->orient(&axis_T, &axis_S);
			pts[j]->add(v0);
		}
	}
}
void ntNodeGen::gen_profile(ntEdge* edge, int sides, float radius){
	// EXCEPTION FOR INVALID POLYGON SEGMENTS
	if (sides < 3) {
		sides = 3;
	}
	//BRANCH PARAMETERS
	ntVec3* v0	= edge->v0;
	ntVec3* v1	= edge->v1;
	float len	= edge->length();
	int div		= 5;
	float h		= len / div;

	ntVec3 axis_T = ntVec3(v0, v1);
	ntVec3 axis_S(0, 0, 1);

	/// temporary 
	radius = len * radius;

	// BUILD PROFILES
	for (int i = 0; i < div; i++) {	
		int	cnt_pts = sides;
		std::vector <ntVec3*> pts;

		// PLOT PROFILE POINTS
		for (int j = 0; j < cnt_pts; j++) {
			float theta = 2.0f * M_PI * j / sides;
			float x = radius * cosf(theta);
			float y = radius * sinf(theta);
			float z = h * i;

			ntVec3 *pt = new ntVec3(x, y, z);
			pts.push_back(pt);
		}
		//ALIGN PROFILE TO EDGE VECTOR
		//TRANSLATE PROFILE TO V0 POSITION
		for (int j = 0; j < pts.size(); j++) {
			pts[j]->orient(&axis_T, &axis_S);
			pts[j]->add(v0);
		}

		ntPolyline *profile = new ntPolyline(pts, true);
		profiles.push_back(profile);
	}
}
void ntNodeGen::gen_profile(ntEdge* edge, float dim0, float dim1) {
	//BRANCH PARAMETERS
	ntVec3* v0	= edge->v0;
	ntVec3* v1	= edge->v1;
	float len	= edge->length();
	float h		= len / div;

	ntVec3 axis_T = ntVec3(v0, v1);
	ntVec3 axis_S(0, 0, 1);

	/// temporary 
	dim0 = len * dim0;
	dim1 = len * dim1;

	// BUILD PROFILES
	for (int i = 0; i < div; i++) {
		// 4 CORNER POINTS
		std::vector <ntVec3*> pts;
		ntVec3 *p00 = new ntVec3( dim0,  dim1, h*i);
		ntVec3 *p01 = new ntVec3(-dim0,  dim1, h*i);
		ntVec3 *p10 = new ntVec3(-dim0, -dim1, h*i);
		ntVec3 *p11 = new ntVec3( dim0, -dim1, h*i);

		pts.push_back(p00);
		pts.push_back(p01);
		pts.push_back(p10);
		pts.push_back(p11);

		//ALIGN PROFILE TO EDGE VECTOR
		//TRANSLATE PROFILE TO V0 POSITION
		for (int j = 0; j < 4; j++) {
			pts[j]->orient(&axis_T, &axis_S);
			pts[j]->add(v0);
		}
		ntPolyline *profile = new ntPolyline(pts, true);
		profiles.push_back(profile);
	}
}
void ntNodeGen::gen_branch() {

	std::cout << "running gen_branch" << endl;
	int cnt_profiles = div - 1;


	for (int i = 0; i < edges.size(); i++) {
		for (int j = 0; j < cnt_profiles; j++) {

			int id = (i * div) + j;
			int cnt_pts = profiles[id]->get_Vecs().size() - 1;

			for (int k = 0; k < cnt_pts; k++) {
				ntPolyline* e = new ntPolyline(profiles[id]->get_Vecs()[k], profiles[id+1]->get_Vecs()[k]);
				e->setCol(col);
				profiles.push_back(e);
			}
		}
	}

	std::cout << "gen_branch complete" << endl;
}

void ntNodeGen::set_Color(ntColor4f col){
	this->col=col;
	for(int i = 0; i<verts.size(); i++){
		verts.at(i)->setColor(col);
	}
}
void ntNodeGen::set_Parameters(SectMode mode, int div, float neck)
{
	this->mode = mode;
	this->div = div;
	this->neck = neck;

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
	for (int i = 0; i < edges.size(); i++) {
		//edges.at(i)->display();
	}
}
