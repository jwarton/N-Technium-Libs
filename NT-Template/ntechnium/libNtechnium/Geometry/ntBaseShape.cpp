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
