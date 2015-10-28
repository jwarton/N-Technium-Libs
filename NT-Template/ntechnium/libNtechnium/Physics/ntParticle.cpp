#include "ntParticle.h"

ntParticle::ntParticle(){}
ntParticle::ntParticle(ntVec3* pos) :
ntVertex(pos){
	///show = false;
	velocity = new Vec3(0, 0, 0);
	init();
}
ntParticle::ntParticle(ntVec3* pos, ntVec3* vel) :
ntVertex(pos), velocity(vel){
	///show = false;
	init();
}
ntParticle::ntParticle(ntVec3* pos, ntVec3* vel, ntCol4 col) :
ntVertex(pos, col), velocity(vel){
	///show = true;
	init();
}

void ntParticle::init(){

	//if (show = true){
	//	addVertex();
	//}
	force = new Vec3(0, 0, 0);
	mass  = 2;
	dead  = false;
	fixed = false;
	fixed_X = false;
	fixed_Y = false;
	fixed_Z = false;
	age = 0;
}
void ntParticle::reset(){
	velocity = new Vec3(0, 0, 0);
	force = new Vec3(0, 0, 0);
	mass = 1;
	dead = false;
	fixed = false;
	fixed_X = false;
	fixed_Y = false;
	fixed_Z = false;
	age = 0;
}
//void ntParticle::display(){
//	//////emitter of lost souls, The 500,000,000 year reign
//	if ((pos->x > 5) || (pos->x < -5) || (pos->y < -5) || (pos->y > 5)){
//		view_Reset();
//	}
//
//	vert.display();
//}

float ntParticle::distanceTo(ntParticle p){
	return pos->distance(p.pos);
}

void ntParticle::setVel(ntVec3 v){
	this->velocity->set(&v);
}
//void ntParticle::addVertex(){
//	vert = ntVertex(pos, col);
//}

ntVec3* ntParticle::getPos(){
	return pos;
}
ntVec3* ntParticle::getVel(){
	return velocity;
}
ntVec3* ntParticle::getForce(){
	return force;
}
float ntParticle::getMass(){
	return mass;
}

bool ntParticle::isFree(){
	if (fixed == false & fixed_X == false & fixed_Y == false & fixed_Z == false){
		return true;
	} else {
		return false;
	}
}
bool ntParticle::isFixed(){
	return fixed;
}
bool ntParticle::isDead(){
	return dead;
}
int ntParticle::getAge(){
	return age;
}