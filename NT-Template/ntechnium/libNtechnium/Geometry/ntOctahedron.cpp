#include "ntOctahedron.h"

ntOctahedron::ntOctahedron(){
}
ntOctahedron::ntOctahedron(const ntVec3& pos, float rad) :
	ntBrep(pos){
	dim.x = rad * 2;
	dim.y = 0;
	dim.z = 0;
	rot.x = 0;
	rot.y = 0;
	rot.z = 0;

	init();
}

void ntOctahedron::init(){

	///////////////////////////////////////////////////////////////////////////generate peak vertex
	ntVec3* v0 = new ntVec3( 0, 0, 1);
	ntVec3* v1 = new ntVec3( 1, 0, 0);
	ntVec3* v2 = new ntVec3( 0,-1, 0);
	ntVec3* v3 = new ntVec3(-1, 0, 0);
	ntVec3* v4 = new ntVec3( 0, 1, 0);
	ntVec3* v5 = new ntVec3( 0, 0,-1);

	vecs.push_back(v0);
	vecs.push_back(v1);
	vecs.push_back(v2);
	vecs.push_back(v3);
	vecs.push_back(v4);
	vecs.push_back(v5);
	///////////////////////////////////////////////////////////////////////////generate base vertices
	for(int i = 0; i < vecs.size(); ++i){
		vecs[i]->scale(dim.x * 0.5);
		vecs[i]->add(&pos);
		ntVertex* vertex = new ntVertex(vecs[i]);
		verts.push_back(vertex);
	}
    inds.push_back(ntTup3i(0,2,1));
    inds.push_back(ntTup3i(0,3,2));
    inds.push_back(ntTup3i(0,4,3));
    inds.push_back(ntTup3i(0,1,4));
	inds.push_back(ntTup3i(5,1,2));
	inds.push_back(ntTup3i(5,2,3));
	inds.push_back(ntTup3i(5,3,4));
	inds.push_back(ntTup3i(5,4,1));
	///////////////////////////////////////////////////////////////////////////instantiate faces
	for (int i = 0; i<inds.size(); ++i) {
		edges.push_back(ntEdge(vecs.at(inds.at(i).elem0), vecs.at(inds.at(i).elem1)));
	}
	edges.push_back(ntEdge(vecs.at(1), vecs.at(2)));
	edges.push_back(ntEdge(vecs.at(2), vecs.at(3)));
	edges.push_back(ntEdge(vecs.at(3), vecs.at(4)));
	edges.push_back(ntEdge(vecs.at(4), vecs.at(1)));
	///////////////////////////////////////////////////////////////////////////instantiate faces
	for(int i=0; i<inds.size(); ++i){
		faces.push_back(ntFace3(vecs.at(inds.at(i).elem0), vecs.at(inds.at(i).elem1), vecs.at(inds.at(i).elem2)));
	}
	//http://rubyquiz.com/quiz3.html
}
void ntOctahedron::display(){
	for(int i = 0;i<faces.size();++i){
		faces.at(i).display();
	}
}
