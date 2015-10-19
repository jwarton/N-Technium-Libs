#include "ntAttractor.h"

Attractor::Attractor(){}

Attractor::Attractor(Vec3 pos, float k, float distMin) :
pos(pos), k(k), distMin(distMin){

}
Attractor::Attractor(Particle p0, Vec3 pos, float k, float distMin):
p0(p0),pos(pos),k(k),distMin(distMin){

}
Attractor::Attractor(Particle p0, Particle p1, float k, float distMin) :
p0(p0), p1(p1), k(k), distMin(distMin){
	this->pos = *p1.getPos();
	distMinSquared = pow(distMin, 2);
}

void Attractor::enable(){
	enabled = true;
}
void Attractor::disable(){
	enabled = false;
}
void Attractor::apply(){

	if (p0.isFree()){
		float distSqrd = p0.getPos()->distSqrd(&pos);

		if (distSqrd < distMinSquared){
			distSqrd = distMinSquared;
		}

		float magnitude = k * p0.getMass() * p0.getMass() / distSqrd;///adjust for p0-p1 attractors
		
		///calculate direction of attraction force
		float fX = p0.getPos()->x - pos.x;
		float fY = p0.getPos()->y - pos.y;
		float fZ = p0.getPos()->z - pos.z;
		Vec3 force = Vec3(fX,fY,fZ);
		force.unitize();
		///apply magnitude to force direction;
		force.mult(magnitude);

		p0.getForce()->add(&force);
	}
}

float Attractor::getDistMin(){
	return distMin;
}
float Attractor::getStrength(){
	return k;
}

void Attractor::setP0(Particle* p){
	this->p0 = *p;
}

void Attractor::setDistMin(float d){
	distMin = d;
}
void Attractor::setStrength(float k){
	this->k = k;
}

bool Attractor::isEnabled(){
	return enabled;
}
bool Attractor::isDisabled(){
	return !enabled;
}

void Attractor::displayRange(){
	///TODO
}
void Attractor::displayDirection(){
	///TODO
}