#include "ntCube.h"

ntCube::ntCube(){
}
ntCube::ntCube(const ntVec3& pos, float  width):
ntBrep(pos){	
	this->width = width;

	dim.x = width;
	dim.y = 0;
	dim.z = 0;
	rot.x = 0;
	rot.y = 0;
	rot.z = 0;

	init();
}

void ntCube::init(){
	int cnt = 0;
	for (int i=0; i < 2; i++) {
		for (int j=0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				ntVec3* ptrVec = new ntVec3(pos.x - (width * 0.5) + (i * width),
					pos.y - (width  * 0.5) + (j * width),
					pos.z - (width  * 0.5) + (k * width));

				vecs.push_back(ptrVec);

				ntVertex* ptrVert = new ntVertex(vecs.at(cnt));
				verts.push_back(ptrVert);
				cnt++;
			}
		}
	}

	inds.push_back(ntTup3i(1, 0, 2));
	inds.push_back(ntTup3i(2, 3, 1));
	inds.push_back(ntTup3i(2, 6, 3));
	inds.push_back(ntTup3i(7, 3, 6));
	inds.push_back(ntTup3i(7, 6, 4));
	inds.push_back(ntTup3i(4, 5, 6));
	inds.push_back(ntTup3i(4, 0, 1));
	inds.push_back(ntTup3i(1, 5, 4));
	inds.push_back(ntTup3i(1, 3, 7));
	inds.push_back(ntTup3i(7, 5, 1));
	inds.push_back(ntTup3i(2, 0, 6));
	inds.push_back(ntTup3i(4, 6, 0));

	///////////////////////////////////////////////////////////////////////////instantiate faces
	for (int i = 0; i<inds.size(); ++i) {
		edges.push_back(ntEdge(vecs.at(inds.at(i).elem0), vecs.at(inds.at(i).elem1)));
	}

	///////////////////////////////////////////////////////////////////////////instantiate faces
	for (int i = 0; i<inds.size(); ++i) {
		faces.push_back(ntFace3(vecs.at(inds.at(i).elem0), vecs.at(inds.at(i).elem1), vecs.at(inds.at(i).elem2)));
	}
}
void ntCube::display(){
	for(int i = 0;i<faces.size();++i){
		faces.at(i).display();
	}
}
