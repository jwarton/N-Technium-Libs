#include "ntVertex.h"

ntVertex::ntVertex(){
}
ntVertex::ntVertex(ntVec3 posF){
	this->size = 1;
	this->col.r = 1;
	this->col.g = 1;
	this->col.b = 1;
	this->col.a = 1;
	setPos(posF);
}
ntVertex::ntVertex(ntVec3* pos):
pos(pos){
	this->size = 1;
	this->col.r = 1;
	this->col.g = 1;
	this->col.b = 1;
	this->col.a = 1;
}
ntVertex::ntVertex(ntVec3* pos,float size):
pos(pos),size(size){
	this->col.r = 1;
	this->col.g = 1;
	this->col.b = 1;
	this->col.a = 1;
}
ntVertex::ntVertex(ntVec3* pos,ntColor4f col):
pos(pos),col(col){
	this->size = 1;
}
ntVertex::ntVertex(ntVec3* pos,float size,ntColor4f col):
pos(pos),size(size),col(col){
}

void ntVertex::setPos(ntVec3 posF){
	this->pos->x = posF.x;
	this->pos->y = posF.y;
	this->pos->z = posF.z;
}
void ntVertex::setSize(float size){
	this->size = size;
}
void ntVertex::setColor(ntColor4f col){
	this->col = col;
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