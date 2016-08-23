#include "ntNormal.h"

ntNormal::ntNormal(){
}
ntNormal::ntNormal(ntVec3 pos,ntVec3 vec):
pos(pos),vec(vec){
	colS = ntColor4f(1, 1, 1, 1);
	colE = ntColor4f(0, 0, 0, 0);
	w = 1;
	vec.unitize();
	init();
}
ntNormal::ntNormal(ntVec3 pos, ntVec3 vec, float mag):
pos(pos),vec(vec),mag(mag){
	colS = ntColor4f(.5,.5,0,1);
	colE = ntColor4f(0, 0, 0, 0);
	w = 1;
	vec.unitize();
	init();
}
void ntNormal::init(){
	end.set(&vec);	//set end pt orientation from world coord
	end.mult(mag);	//scale by new size
	end.add(&pos);	//translate to vertex coord
}
void ntNormal::setLength(float length){
	end.set(&vec);	//set end pt orientation from world coord
	end.mult(length);	//scale by new size
	end.add(&pos);	//translate to vertex coord
}
void ntNormal::set_color(ntColor4f col){
	this->colS = col;
	this->colE = col;
}
void ntNormal::display(float size){
	setLength(size);
	display();
}
void ntNormal::display(){
	glLineWidth(w);
	glBegin(GL_LINES);
	glColor4f(colS.r, colS.g, colS.b, colS.a);
	glVertex3f(pos.x, pos.y, pos.z);
	glColor4f(colE.r, colE.g, colE.b, colE.a);
	glVertex3f(end.x, end.y, end.z);
	glEnd();
}