#include "ntVertex.h"


/// USED BY BOUNDING BOX FUNCTIONS
double ntVertex::min_X = 10000000000.0;
double ntVertex::min_Y = 10000000000.0;
double ntVertex::min_Z = 10000000000.0;
double ntVertex::max_X = 0.0;
double ntVertex::max_Y = 0.0;
double ntVertex::max_Z = 0.0;

ntVertex::ntVertex(){
}
ntVertex::ntVertex(ntVec3 posF){
	this->size = 1;
	this->col.r = 1;
	this->col.g = 1;
	this->col.b = 1;
	this->col.a = 1;
	setPos(posF);

	setBounds();
}
ntVertex::ntVertex(ntVec3* pos):
pos(pos){
	this->size = 1;
	this->col.r = 1;
	this->col.g = 1;
	this->col.b = 1;
	this->col.a = 1;

	setBounds();
}
ntVertex::ntVertex(ntVec3* pos,float size):
pos(pos),size(size){
	this->col.r = 1;
	this->col.g = 1;
	this->col.b = 1;
	this->col.a = 1;

	setBounds();
}
ntVertex::ntVertex(ntVec3* pos,ntColor4f col):
pos(pos),col(col){
	this->size = 1;

	setBounds();
}
ntVertex::ntVertex(ntVec3* pos,float size,ntColor4f col):
pos(pos),size(size),col(col){

	setBounds();
}

void ntVertex::setPos(ntVec3 posF){
	this->pos->x = posF.x;
	this->pos->y = posF.y;
	this->pos->z = posF.z;
}
void ntVertex::setSize(float size){
	this->size = size;
}
void ntVertex::set_color(ntColor4f col){
	this->col = col;
}
void ntVertex::setBounds() {
	if (pos->x < min_X) {
		min_X = pos->x;
	}
	if (pos->y < min_Y) {
		min_Y = pos->y;
	}
	if (pos->z < min_Z) {
		min_Z = pos->z;
	}
	if (pos->x > max_X) {
		max_X = pos->x;
	}
	if (pos->y > max_Y) {
		max_Y = pos->y;
	}
	if (pos->z > max_Z) {
		max_Z = pos->z;
	}

	//std::cout << "\n";
	//pos->print();
}
void ntVertex::getBounds(ntVec3* minVec, ntVec3* maxVec) {
	ntVec3* min = new ntVec3(min_X, min_Y, min_Z);
	ntVec3* max = new ntVec3(max_X, max_Y, max_Z);

	minVec->set(min);
	maxVec->set(max);
}

ntColor4f ntVertex::getColor(){
	//std::cout << col.r << endl;
	return col;
}
void ntVertex::display(float size){
	this->size = size;
	display();
}
void ntVertex::display(){
	glPointSize(size);
	glBegin(GL_POINTS);
	glColor4f(col.r, col.g, col.b, col.a);
	glVertex3f(pos->x, pos->y, pos->z);
	glEnd();
}