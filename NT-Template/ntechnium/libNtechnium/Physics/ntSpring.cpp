#include "ntSpring.h"


ntSpring::ntSpring(){}
ntSpring::ntSpring(Particle p0, Particle p1):
p0(p0),p1(p1){
	springConstant = 2; 
	damping = 1; 
	restLength = .75;
	init();
}
ntSpring::ntSpring(Particle p0, Particle p1, float ks, float d, float r):
p0(p0), p1(p1), springConstant(ks), damping(d), restLength(r){
	init();
}
/////
//ntSpring::ntSpring(Particle* p0, Particle* p1){
//	this->p0 = *p0;
//	this->p1 = *p1;
//
//	springConstant = 0;
//	damping = 1;
//	restLength = .01;
//	init();
//}
/////
void ntSpring::init(){
	enabled = true;
}
void ntSpring::setP0(ntParticle p){
	this->p0 = p;
}
void ntSpring::setP1(ntParticle p){
	this->p1 = p;
}

void ntSpring::enable(){
	enabled = true;
}
void ntSpring::disable(){
	enabled = false;
}
bool ntSpring::isEnabled(){
	return enabled;
}

Particle ntSpring::getP0(){
	return p0;
}
Particle ntSpring::getP1(){
	return p1;
}

float ntSpring::getLength(){
	return p0.distanceTo(p1);
}
float ntSpring::getRestLength(){
	return restLength;
}
void ntSpring::setRestLength(float rl){
	restLength = rl;
}

float ntSpring::getStrength(){
	return springConstant;
}
void ntSpring::setStrength(float ks){
	springConstant = ks;
}

float ntSpring::getDamping(){
	return damping;
}
void ntSpring::setDamping(float d){
	damping = d;
}

void ntSpring::apply(){
	if (enabled && (p0.isFree() || p1.isFree()))
	{
		float lenX = p0.getPos()->x - p1.getPos()->x;
		float lenY = p0.getPos()->y - p1.getPos()->y;
		float lenZ = p0.getPos()->z - p1.getPos()->z;

		float length = p0.getPos()->distance(p1.getPos());

		if (length == 0){
			lenX = 0;
			lenY = 0;
			lenZ = 0;
		} else {
			lenX /= length;
			lenY /= length;
			lenZ /= length;
		}
		///////////////////////////////////////////////////////////////
		//////////// spring force is proportional to stretched distance
		float springForce = -(length - restLength) * springConstant;

		///////////////////////////////////////////////////////////////
		/////////////////////////////////// velocity along line p0 - p1
		float velX = p0.getVel()->x - p1.getVel()->x;
		float velY = p0.getVel()->y - p1.getVel()->y;
		float velZ = p0.getVel()->z - p1.getVel()->z;

		///////////////////////////////////////////////////////////////
		///////////////////////////////// damping force is proportional
		float dampingForce = -damping * (lenX * velX + lenY * velY + lenZ * velZ);

		///////////////////////////////////////////////////////////////
		//////////////// force1 is same as force0 in opposite direction
		float r = springForce + dampingForce;

		lenX *= r;
		lenY *= r;
		lenZ *= r;

		Vec3 len3 = Vec3(lenX, lenY, lenZ);
		if (p0.isFree())
			p0.getForce()->add(&len3);

		len3.invert();

		if (p1.isFree())
			p1.getForce()->add(&len3);
	}
}