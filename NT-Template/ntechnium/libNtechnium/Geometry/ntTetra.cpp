#include "ntTetra.h"

ntTetra::ntTetra(){
}

ntTetra::ntTetra(const ntVec3& pos, const ntVec3&  rot, const ntVec3&  dim, 
				 const ntColor4f& fillCol, const ntColor4f& strokeCol, 
				 std::string name):
BaseShape(pos,rot,dim,fillCol,strokeCol), name(name)
{	
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
	ntVec3* ptrVec = new ntVec3( pos.x,
							pos.y,
							pos.z+(.75*h));

			vecs.push_back(ptrVec);
			verts.push_back(&ntVertex(ptrVec));

	///////////////////////////////////////////////////////////////////////////generate base vertices
	for(int i = 0; i < 3; ++i){
			float theta = (i * 120) * M_PI / 180;
			ntVec3* ptrVec = new ntVec3(	pos.x + (radius * cos(theta)), 
									pos.y + (radius * sin(theta)), 
									pos.z - (h/4));
			vecs.push_back(ptrVec);
			verts.push_back(&ntVertex(ptrVec));
	}
    inds.push_back(ntTup3i(0,1,2));
    inds.push_back(ntTup3i(0,2,3));
    inds.push_back(ntTup3i(0,3,1));
    inds.push_back(ntTup3i(1,3,2));
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

void ntTetra::displayNorms(float len){
	for(int i = 0;i<faces.size();++i){
		faces.at(i).normal.display();
	}
}
