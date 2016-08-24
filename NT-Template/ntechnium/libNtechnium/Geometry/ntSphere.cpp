#include "ntSphere.h"

ntSphere::ntSphere(){
}
ntSphere::ntSphere(const ntVec3 & pos, const float & rad, int U, int V):
ntBrep(pos), U(U), V(V)
{
	dim.x = rad;
	dim.y = 0;
	dim.z = 0;
	rot.x = 0;
	rot.y = 0;
	rot.z = 0;

	init();
}
ntSphere::ntSphere(const ntVec3 & pos, const ntVec3 & rot, float rad, int U, int V) :
	ntBrep(pos, rot),U(U),V(V){
	dim.x = rad;
	dim.y = 0;
	dim.z = 0;

	init();
}

void ntSphere::init(){
	float radius = dim.x;
	float phi = 0;
	float theta = 0;

	//PLOT VERTEX COORDINATES
	int cnt = 0;
	for (int i=0; i< U +1; i++) {
		for (int j=0; j< V; j++) {
			theta =(2*M_PI/U)*(j);
			phi = (M_PI/V)*i;

			ntVec3* vec = new ntVec3( pos.x +(cos(theta) * sin(phi) * radius), 
				pos.y +(sin(theta) * sin(phi) * radius), 
				pos.z +(cos(phi) * radius));

			vecs.push_back(vec);

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

void ntSphere::display(){
	for(int i = 0;i<faces.size();++i){
		faces.at(i).display();
	}
}
void ntSphere::display_edges(float w){
	for(int i = 0;i<faces.size();++i){
		for(int j = 0;j<3;j++){
			faces.at(i).edges.at(j).display(w);
		}
	}
}
void ntSphere::display_edges(){
	for (int i = 0; i<faces.size(); ++i){
		for (int j = 0; j<3; j++){
			faces.at(i).edges.at(j).display();
		}
	}
}