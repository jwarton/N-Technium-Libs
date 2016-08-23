///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////// ntMeshPts.cpp
// openGl scratch libs							///////////////////
// Mesh Class from explicit point list			///////////////////
// created by James Warton on 04/05/2014		///////////////////
///////////////////////////////////////////////////////////////////
#include "ntMeshPts.h"

ntMeshPts::ntMeshPts(){
}

ntMeshPts::ntMeshPts(std::vector<ntVec3*>* vecsPtr):
BaseShape(vecsPtr){
	///this should be set by constructor
	///older class calls rely on these values
	U = 20;//50;
	V = 21;//51;
	init();
}
ntMeshPts::ntMeshPts(std::vector<ntVec3*>* vecsPtr,int U, int V) :
BaseShape(vecsPtr), U(U), V(V){
	init();
}
void ntMeshPts::init(){
	vecs = *vecsPtr;

	for (int i = 0; i < vecsPtr->size(); i++){
		ntVertex* vert = new ntVertex(vecs.at(i));
		///hack response to faulty camera position
		//Mat4(vert->pos).translate(Vec3(0, 0, 1.25)); ///this should be removed
		verts.push_back(vert);
	}

	for (int i = 0; i < (vecs.size() - (V)); i++) {
		/*
		inds.push_back(ntTup3i(0,1,2));
		inds.push_back(ntTup3i(0,2,3));
		inds.push_back(ntTup3i(0,3,1));
		inds.push_back(ntTup3i(1,3,2));
		*/
		if (i>0) {
			if (fmod(i, V) == 0) {
				int val0 = i - 1;
				int val1 = i + (V - 1);
				int val2 = i - V;
				int val3 = i;
				/////last face is missing
				//faces.push_back(ntFace3(vecs.at(val0), vecs.at(val1), vecs.at(val2), verts.at(val0), verts.at(val1), verts.at(val2)));
				//faces.push_back(ntFace3(vecs.at(val2), vecs.at(val1), vecs.at(val3), verts.at(val2), verts.at(val1), verts.at(val3)));
			} else {
				int val0 = i;
				int val1 = i - 1;
				int val2 = i - 1 + V;
				int val3 = i + V;
				faces.push_back(ntFace3(vecs.at(val0), vecs.at(val1), vecs.at(val2), verts.at(val0), verts.at(val1), verts.at(val2)));
				faces.push_back(ntFace3(vecs.at(val0), vecs.at(val2), vecs.at(val3), verts.at(val0), verts.at(val2), verts.at(val3)));
			}
		}
	}
}

void ntMeshPts::update(){
	///TODO
}
void ntMeshPts::display(){
	for (int i = 0; i<faces.size(); ++i){
		faces.at(i).display();
	}
}
void ntMeshPts::display_verts(){
	for (int i = 0; i<verts.size(); ++i){
		verts.at(i)->display();
	}
}
void ntMeshPts::display_verts(float dim){
	for (int i = 0; i<verts.size(); ++i){
		verts.at(i)->setSize(dim);
		verts.at(i)->display();
	}
}
void ntMeshPts::display_norms(){
	for (int i = 0; i<faces.size(); ++i){
		faces.at(i).normal.display();
	}
}
void ntMeshPts::display_norms(float len){
	for (int i = 0; i<faces.size(); ++i){
		faces.at(i).normal.setLength(len);
		faces.at(i).normal.display();
	}
}
void ntMeshPts::display_edges(){
	for (int i = 0; i<faces.size(); ++i){
		for (int j = 0; j<3; j++){
			faces.at(i).edges.at(j).display();
		}
	}
}
void ntMeshPts::display_edges(float w){
	for (int i = 0; i<faces.size(); ++i){
		for (int j = 0; j<3; j++){
			faces.at(i).edges.at(j).display(w);
		}
	}
}
void ntMeshPts::addPhysics(bool enable){
	enablePhysics = true;
	for (int i = 0; i < vecs.size(); i++){
		Particle* p1 = new Particle(vecs.at(i));
		particles.push_back(p1);
	}
	///this approach will duplicate every edge
	///and triplicate every node
	///this needs another approach!!!!!!!!!!!!!!!!!!!!!!!!!
	for (int i = 0; i < faces.size(); i++){
		for (int j = 0; j < 3; j++){
			Particle* p0 = new Particle(faces.at(i).edges.at(j).v0);
			particles.push_back(p0);
			Particle* p1 = new Particle(faces.at(i).edges.at(j).v1);
			particles.push_back(p1);
			addSpring(p0, p1);
		}
	}
}
void ntMeshPts::addSpring(Particle* p0, Particle* p1){
	float r = 1.3 * p0->distanceTo(*p1);	//rest length
	float ks = .2;	// spring constant | set relative to environmental forces
	float d = .6;	// damping range:  0-1 | where 1= critical damping or 0 bounce from l to rl 
	// values < 0 create take longer to balance
	// vlaues > 1 takes longer to reach rest length

	Spring* spring = new Spring(*p0, *p1, ks, d, r);
	springs.push_back(spring);	//add branch to collection
}
