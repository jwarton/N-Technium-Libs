#include "ntAgent.h"

ntAgent::ntAgent(){}

ntAgent::ntAgent(ntVec3* pos):
ntParticle(pos){}

ntAgent::ntAgent(ntVec3* pos, ntVec3 heading) :
heading(heading), ntParticle(pos){
}

ntAgent::ntAgent(ntVec3* pos, ntVec3 heading, float vision) :
heading(heading), vision(vision), ntParticle(pos){
}

void ntAgent::addNeighbor(ntAgent* a){
	this->neighbors.push_back(a);
}

void ntAgent::displayVision(){
	///TODO
}
void ntAgent::displayHeading(){
	///TODO
}
void ntAgent::displayAgent(){
	///TODO
}