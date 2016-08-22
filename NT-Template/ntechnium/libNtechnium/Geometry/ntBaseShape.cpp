#include "ntBaseShape.h"

BaseShape::BaseShape(){
}

BaseShape::BaseShape(const ntVec3& pos) :
pos(pos){
	rot = ntVec3(0, 0, 0);
	dim = ntVec3(0, 0, 0);
}

BaseShape::BaseShape(const ntVec3& pos, const ntVec3&  rot, const ntVec3&  dim): 
pos(pos), rot(rot), dim(dim){
}

BaseShape::BaseShape(const ntVec3& pos, const ntVec3&  rot, const ntVec3&  dim, const ntColor4f& fillCol, const ntColor4f& strokeCol): 
pos(pos), rot(rot), dim(dim), fillCol(fillCol), strokeCol(strokeCol){
}

BaseShape::BaseShape(std::vector<ntVec3*>* vecsPtr = nullptr):
vecsPtr(vecsPtr){

}
//
//BaseShape::~BaseShape(){
//}

void BaseShape::move(){
}

void BaseShape::display_norms(float len){
	for (int i = 0; i<faces.size(); ++i) {
		faces.at(i).normal.setLength(len);
		faces.at(i).normal.display();
	}
}
void BaseShape::display_verts(float dim){
	for (int i = 0; i<verts.size(); ++i) {
		verts.at(i)->setSize(dim);
		verts.at(i)->display();
	}
}
void BaseShape::display_edges(float w){
	for (int i = 0; i<edges.size(); ++i) {
		edges.at(i).display(w);
	}
}

void BaseShape::display_norms() {
	for (int i = 0; i<faces.size(); ++i) {
		faces.at(i).normal.display();
	}
}

void BaseShape::display_verts(){
	for (int i = 0; i<verts.size(); ++i) {
		verts.at(i)->display();
	}
}
void BaseShape::display_edges(){
	for (int i = 0; i<edges.size(); ++i) {
		edges.at(i).display();
	}
}