#include "ntSphere.h"

ntSphere::ntSphere(){
}

ntSphere::ntSphere(const ntVec3& pos, const ntVec3&  rot, const ntVec3&  dim, const ntColor4f& fillCol, const ntColor4f& strokeCol):
BaseShape(pos,rot,dim,fillCol,strokeCol)
{	
	init();
}

ntSphere::ntSphere(const ntVec3& pos, const ntVec3&  rot, const ntVec3&  dim, int U, int V):
BaseShape(pos,rot,dim), U(U), V(V)
{	
	init();
}

void ntSphere::init(){
	//distance of vertex from face to centroid
	float radius = dim.x;
	//temp variables for column and row
	float phi = 0;
	float theta = 0;
	//plot vertex coordinates
	int cnt = 0;
	for (int i=0; i< U +1; i++) {
		for (int j=0; j< V; j++) {
			theta =(2*M_PI/U)*(j);
			phi = (M_PI/V)*i;

			ntVec3* ptrVec = new ntVec3( pos.x +(cos(theta) * sin(phi) * radius), 
				pos.y +(sin(theta) * sin(phi) * radius), 
				pos.z +(cos(phi) * radius));

			vecs.push_back(ptrVec);

			ntVertex* ptrVert = new ntVertex (vecs.at(cnt));
			verts.push_back(ptrVert);
			cnt++;
		}
	}
	///generate faces from vertices
    for (int i=0;i<(vecs.size()-(V)) ;i++) {
		/*
		inds.push_back(ntTup3i(0,1,2));
		inds.push_back(ntTup3i(0,2,3));
		inds.push_back(ntTup3i(0,3,1));
		inds.push_back(ntTup3i(1,3,2));
		*/
      if (i>0) {
        if (fmod(i,V) == 0) { 
			int val0 = i-1;
			int val1 = i + (V-1);
			int val2 = i - V;
			int val3 = i;
			///last face is missing
			faces.push_back(ntFace3(vecs.at(val0), vecs.at(val1), vecs.at(val2), verts.at(val0), verts.at(val1), verts.at(val2)));
			faces.push_back(ntFace3(vecs.at(val2), vecs.at(val1), vecs.at(val3), verts.at(val2), verts.at(val1), verts.at(val3)));
        } else {
			int val0 = i;
			int val1 = i-1;
			int val2 = i-1 + V;
			int val3 = i + V;
			faces.push_back(ntFace3(vecs.at(val0), vecs.at(val1), vecs.at(val2), verts.at(val0), verts.at(val1), verts.at(val2)));
			faces.push_back(ntFace3(vecs.at(val0), vecs.at(val2), vecs.at(val3), verts.at(val0), verts.at(val2), verts.at(val3)));
        }
      }
	}
	/*
	for(int i=0; i<inds.size(); ++i){
		faces.push_back(ntFace3(vecs.at(inds.at(i).elem0), vecs.at(inds.at(i).elem1), vecs.at(inds.at(i).elem2)));
	}
	*/
}

void ntSphere::setColor(ntColor4f col){

	for(int i = 0; i<faces.size(); i++){
		faces.at(i).setColor(col);
	}
}

void ntSphere::display(){
	for(int i = 0;i<faces.size();++i){
		faces.at(i).display();
	}
}

void ntSphere::displayNorms(float len){
	for(int i = 0;i<faces.size();++i){
		faces.at(i).normal.setLength(len);
		faces.at(i).normal.display();
	}
}

void ntSphere::displayVerts(float dim){
	for(int i = 0;i<verts.size();++i){
		verts.at(i)->setSize(dim);
		verts.at(i)->display();
	}
}

void ntSphere::displayEdges(float w){
	for(int i = 0;i<faces.size();++i){
		for(int j = 0;j<3;j++){
			faces.at(i).edges.at(j).display(w);
		}
	}
}

void ntSphere::displayNorms(){
	for (int i = 0; i<faces.size(); ++i){
		faces.at(i).normal.display();
	}
}

void ntSphere::displayVerts(){
	for (int i = 0; i<verts.size(); ++i){
		verts.at(i)->display();
	}
}

void ntSphere::displayEdges(){
	for (int i = 0; i<faces.size(); ++i){
		for (int j = 0; j<3; j++){
			faces.at(i).edges.at(j).display();
		}
	}
}