#include "ntPlane.h"

ntPlane::ntPlane(){
}

ntPlane::ntPlane(const ntVec3& pos, const float&  dimX, const float&  dimY, int U, int V) :
BaseShape(pos), U(U), V(V){
	this->dim.x = dimX;
	this->dim.y = dimY;
	init();
}
ntPlane::ntPlane(const ntVec3& pos, const ntVec3&  rot, const ntVec3&  dim, 
				 const ntColor4f& fillCol, const ntColor4f& strokeCol):
BaseShape(pos,rot,dim,fillCol,strokeCol)
{	
	init();
}

ntPlane::ntPlane(const ntVec3& pos, const ntVec3&  rot, const ntVec3&  dim, int U, int V):
BaseShape(pos,rot,dim), U(U), V(V)
{	
	init();
}

void ntPlane::init(){
	vecs.reserve(U * V);
	verts.reserve(U * V);	//http://lemire.me/blog/archives/2012/06/20/do-not-waste-time-with-stl-vectors/

	//plot vertex coordinates
	int cnt = 0;
	for (int i=0; i< U ; i++) {
		for (int j=0; j< V; j++) {
			float stepV = (j * (dim.y / V));
			float stepU = (i * (dim.x / U));

			ntVec3* ptrVec = new ntVec3( 
				pos.x - (dim.x * .5) + stepU, 
				pos.y - (dim.y * .5) + stepV,
				0);

			vecs.push_back(ptrVec);
			//vecs[cnt] = ptrVec;

			ntVertex* ptrVert = new ntVertex (vecs.at(cnt));
			verts.push_back(ptrVert);
			//verts[cnt] = ptrVert;
			//cnt++;
		}
	}
	//generate edges from vertex position
	for (int i = 0; i<(vecs.size()); i++) {
		int val0 = i;
		if (i == 0) {							//first edge
				int val1 = i + V;
				edges.push_back(ntEdge(vecs.at(val0), vecs.at(val1), ntCol4(1, 1, 0, 1)));
		} else if (i < vecs.size() - (V)) {
				if (fmod(i, V) != 0) {			//first boundry edges and all not boundry edges
					int val1 = i - 1;
					int val2 = i + V;
					edges.push_back(ntEdge(vecs.at(val0), vecs.at(val1), ntCol4(1, 0, 0, 1)));
					edges.push_back(ntEdge(vecs.at(val0), vecs.at(val2), ntCol4(1, 0, 0, 1)));
				} else {						//
					int val1 = i + V;
					edges.push_back(ntEdge(vecs.at(val0), vecs.at(val1), ntCol4(1, 1, 0, 1)));
				}
		} else {
			if (fmod(i, V) != 0) {
				int val1 = i - 1;
				edges.push_back(ntEdge(vecs.at(val0), vecs.at(val1), ntCol4(1, 1, 0, 1)));
			}
		}
	}

	//generate faces from vertices
    for (int i=0;i<(vecs.size()-(V)) ;i++) {
		/*
		inds.push_back(ntTup3i(0,1,2));
		inds.push_back(ntTup3i(0,2,3));
		inds.push_back(ntTup3i(0,3,1));
		inds.push_back(ntTup3i(1,3,2));
		*/
      if (i>0) {
        if (fmod(i,V) != 0) { 
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

void ntPlane::set_color(ntColor4f col){

	for(int i = 0; i<faces.size(); i++){
		faces.at(i).set_color(col);
	}
}

void ntPlane::display(){
	for(int i = 0;i<faces.size();++i){
		faces.at(i).display();
	}
}
///MOVE TO BASE CLASS
///////////////////////////////////////////////////////////
//////////////////////////////////////// PHYSICS OPERATIONS
void ntPlane::addPhysics(bool enable){
	enablePhysics = true;
	int index = 0;
	int c0 = 0;
	int c1 = V - 1;
	int c2 = vecs.size() - 1;
	int c3 = vecs.size() - V;

	for (int i = 0; i < vecs.size(); i++){
		Particle* p1 = new Particle(vecs.at(i));
		particles.push_back(p1);
		if (i == c0 | i == c1 | i == c2 | i == c3){
			//array should use less overhead //corners[index] = p1;
			corners.push_back(p1);
			index += 1;
		}
	}
	///generate springs from particles position  
	///THIS PROCESS REPEATS EDGE CONDITIONS--- REDUNDANT PROCESSING 
	///SPRING FROM EDGES WOULD BE MORE DIRECT
	for (int i = 0; i<(particles.size()); i++) {
		int val0 = i;
		if (i == 0) {							
			int val1 = i + V;
			addSpring(particles.at(val0), particles.at(val1));
		}
		else if (i < particles.size() - (V)) {
			if (fmod(i, V) != 0) {			
				int val1 = i - 1;
				int val2 = i + V;
				addSpring(particles.at(val0), particles.at(val1));
				addSpring(particles.at(val0), particles.at(val2));
			} else {						
				int val1 = i + V;
				addSpring(particles.at(val0), particles.at(val1));
			}
		} else {
			if (fmod(i, V) != 0) {
				int val1 = i - 1;
				addSpring(particles.at(val0), particles.at(val1));
			}
		}
	}

}
void ntPlane::addSpring(Particle* p0, Particle* p1){
	float r = .5 * p0->distanceTo(*p1);	//rest length
	float ks= .8;	// spring constant | set relative to environmental forces
	float d = .6;	// damping range:  0-1 | where 1= critical damping or 0 bounce from l to rl 
	// values < 0 create take longer to balance
	// vlaues > 1 takes longer to reach rest length

	Spring* spring = new Spring(*p0, *p1, ks, d, r);
	springs.push_back(spring);	//add branch to collection
}
