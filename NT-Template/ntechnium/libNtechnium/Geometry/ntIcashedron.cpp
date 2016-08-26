#include "ntIcashedron.h"

ntIcashedron::ntIcashedron(){
}
ntIcashedron::ntIcashedron(const ntVec3& pos, float rad) :
	ntBrep(pos){
	dim.x = rad * 2;
	dim.y = 0;
	dim.z = 0;
	rot.x = 0;
	rot.y = 0;
	rot.z = 0;

	init();
}

void ntIcashedron::init(){
	double g = g_mean;
	///////////////////////////////////////////////////////////////////////////generate peak vertex
	ntVec3* v00 = new ntVec3( 1, g, 0);
	ntVec3* v01 = new ntVec3( 1,-g, 0);
	ntVec3* v02 = new ntVec3(-1,-g, 0);
	ntVec3* v03 = new ntVec3(-1, g, 0);
	ntVec3* v04 = new ntVec3( g, 0, 1);
	ntVec3* v05 = new ntVec3(-g, 0, 1);
	ntVec3* v06 = new ntVec3(-g, 0,-1);
	ntVec3* v07 = new ntVec3( g, 0,-1);
	ntVec3* v08 = new ntVec3( 0, 1, g);
	ntVec3* v09 = new ntVec3( 0, 1,-g);
	ntVec3* v10 = new ntVec3( 0,-1,-g);
	ntVec3* v11 = new ntVec3( 0,-1, g);

	vecs.push_back(v00);
	vecs.push_back(v01);
	vecs.push_back(v02);
	vecs.push_back(v03);
	vecs.push_back(v04);
	vecs.push_back(v05);
	vecs.push_back(v06);
	vecs.push_back(v07);
	vecs.push_back(v08);
	vecs.push_back(v09);
	vecs.push_back(v10);
	vecs.push_back(v11);
	///////////////////////////////////////////////////////////////////////////generate base vertices
	for(int i = 0; i < vecs.size(); ++i){
		vecs[i]->unitize();
		vecs[i]->scale(dim.x * 0.5);
		vecs[i]->add(&pos);
		ntVertex* vertex = new ntVertex(vecs[i]);
		verts.push_back(vertex);
	}
	inds.push_back(ntTup3i(8, 4,  0));
	inds.push_back(ntTup3i(8, 0,  3));
	inds.push_back(ntTup3i(8, 3,  5));
	inds.push_back(ntTup3i(8, 5, 11));
	inds.push_back(ntTup3i(8, 11, 4));

	inds.push_back(ntTup3i( 4, 7, 0));
	inds.push_back(ntTup3i( 0, 9, 3));
	inds.push_back(ntTup3i( 3, 6, 5));
	inds.push_back(ntTup3i( 5, 2, 11));
	inds.push_back(ntTup3i(11, 1, 4));
	inds.push_back(ntTup3i( 4, 1, 7));
	inds.push_back(ntTup3i( 0, 7, 9));
	inds.push_back(ntTup3i( 3, 9, 6));
	inds.push_back(ntTup3i( 5, 6, 2));
	inds.push_back(ntTup3i(11, 2, 1));

	inds.push_back(ntTup3i(10, 7, 1));
	inds.push_back(ntTup3i(10, 9, 7));
	inds.push_back(ntTup3i(10, 6, 9));
	inds.push_back(ntTup3i(10, 2, 6));
	inds.push_back(ntTup3i(10, 1, 2));

	///////////////////////////////////////////////////////////////////////////instantiate faces
	for (int i = 0; i<inds.size(); ++i) {
		//edges.push_back(ntEdge(vecs.at(inds.at(i).elem0), vecs.at(inds.at(i).elem1)));
	}
	//Z axis
	edges.push_back(ntEdge(vecs.at(1), vecs.at(2)));
	edges.push_back(ntEdge(vecs.at(3), vecs.at(0)));
	//Y axis
	edges.push_back(ntEdge(vecs.at(5), vecs.at(6)));
	edges.push_back(ntEdge(vecs.at(7), vecs.at(4)));
	//X axis
	edges.push_back(ntEdge(vecs.at(9), vecs.at(10)));
	edges.push_back(ntEdge(vecs.at(11), vecs.at(8)));
	// Z axis - Y axis
	edges.push_back(ntEdge(vecs.at(2), vecs.at(5)));
	edges.push_back(ntEdge(vecs.at(6), vecs.at(2)));
	edges.push_back(ntEdge(vecs.at(3), vecs.at(5)));
	edges.push_back(ntEdge(vecs.at(6), vecs.at(3)));

	edges.push_back(ntEdge(vecs.at(1), vecs.at(7)));
	edges.push_back(ntEdge(vecs.at(4), vecs.at(1)));
	edges.push_back(ntEdge(vecs.at(0), vecs.at(7)));
	edges.push_back(ntEdge(vecs.at(4), vecs.at(0)));
	// Y axis - X axis
	edges.push_back(ntEdge(vecs.at(6), vecs.at(9)));
	edges.push_back(ntEdge(vecs.at(10), vecs.at(6)));
	edges.push_back(ntEdge(vecs.at(7), vecs.at(9)));
	edges.push_back(ntEdge(vecs.at(10), vecs.at(7)));

	edges.push_back(ntEdge(vecs.at(5), vecs.at(11)));
	edges.push_back(ntEdge(vecs.at(8), vecs.at(5)));
	edges.push_back(ntEdge(vecs.at(4), vecs.at(11)));
	edges.push_back(ntEdge(vecs.at(8), vecs.at(4)));
	// X axis - Z axis
	edges.push_back(ntEdge(vecs.at(10), vecs.at(1)));
	edges.push_back(ntEdge(vecs.at(2), vecs.at(10)));
	edges.push_back(ntEdge(vecs.at(11), vecs.at(1)));
	edges.push_back(ntEdge(vecs.at(2), vecs.at(11)));

	edges.push_back(ntEdge(vecs.at(9), vecs.at(3)));
	edges.push_back(ntEdge(vecs.at(0), vecs.at(9)));
	edges.push_back(ntEdge(vecs.at(8), vecs.at(3)));
	edges.push_back(ntEdge(vecs.at(0), vecs.at(8)));

	///////////////////////////////////////////////////////////////////////////instantiate faces
	for(int i=0; i<inds.size(); ++i){
		faces.push_back(ntFace3(vecs.at(inds.at(i).elem0), vecs.at(inds.at(i).elem1), vecs.at(inds.at(i).elem2)));
	}
	//http://rubyquiz.com/quiz3.html
}
void ntIcashedron::display(){
	for(int i = 0;i<faces.size();++i){
		faces.at(i).display();
	}
}
