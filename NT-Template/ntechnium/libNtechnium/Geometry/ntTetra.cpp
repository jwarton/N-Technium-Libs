#include "ntTetra.h"

ntTetra::ntTetra(){
}
ntTetra::ntTetra(const ntVec3& pos, float rad) :
	ntBrep(pos){
	dim.x = rad * 2;
	dim.y = 0;
	dim.z = 0;
	rot.x = 0;
	rot.y = 0;
	rot.z = 0;

	init();
}

void ntTetra::init(){
	///////////////////////////////////////////////////////////////////////////area of base triangle
	float area = ((sqrt(3))/4 * (dim.x * dim.x));
	///////////////////////////////////////////////////////////////////////////distance of vertext from face to centroid
	float radius = (1/sqrt(3))*dim.x;
	///////////////////////////////////////////////////////////////////////////calculate height of shape
	float h = ((sqrt(6)/3)*dim.x);

	///////////////////////////////////////////////////////////////////////////generate peak vertex
	ntVec3* v0 = new ntVec3( pos.x, pos.y, pos.z+(.75*h));
	vecs.push_back(v0);
	ntVertex* vert = new ntVertex(v0);
	verts.push_back(vert);

	///////////////////////////////////////////////////////////////////////////generate base vertices
	for(int i = 0; i < 3; ++i){
			float theta = (i * 120) * M_PI / 180;
			ntVec3* v1 = new ntVec3(	pos.x + (radius * cos(theta)), 
									pos.y + (radius * sin(theta)), 
									pos.z - (h/4));
			vecs.push_back(v1);

			ntVertex* vertex = new ntVertex(v1);
			verts.push_back(vertex);
	}
    inds.push_back(ntTup3i(0,1,2));
    inds.push_back(ntTup3i(0,2,3));
    inds.push_back(ntTup3i(0,3,1));
    inds.push_back(ntTup3i(1,3,2));
	///////////////////////////////////////////////////////////////////////////instantiate faces
	for (int i = 0; i<inds.size(); ++i) {
		edges.push_back(ntEdge(vecs.at(inds.at(i).elem0), vecs.at(inds.at(i).elem1)));
	}
	edges.push_back(ntEdge(vecs.at(1), vecs.at(2)));
	edges.push_back(ntEdge(vecs.at(2), vecs.at(3)));
	///////////////////////////////////////////////////////////////////////////instantiate faces
	for(int i=0; i<inds.size(); ++i){
		faces.push_back(ntFace3(vecs.at(inds.at(i).elem0), vecs.at(inds.at(i).elem1), vecs.at(inds.at(i).elem2)));
	}
}
void ntTetra::display(){
	for(int i = 0;i<faces.size();++i){
		faces.at(i).display();
	}
}
