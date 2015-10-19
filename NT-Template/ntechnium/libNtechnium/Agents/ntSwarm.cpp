#include "ntSwarm.h"

ntSwarm::ntSwarm(){}
ntSwarm::ntSwarm(std::vector<ntAgent*>* agents):
agents(agents){
	zoneS = .001;
	zoneA = .001;
	zoneC = .001;
	setAlignmentTurnMax(5.0f);
	setCohesionTurnMax(3.0f);
	setSeparationTurnMax(1.5f);
	init();
}
ntSwarm::ntSwarm(std::vector<ntAgent*>* agents, float s, float a, float c) :
agents(agents), zoneS(s), zoneA(a), zoneC(c){
	turnMaxS = 15;
	turnMaxA = 5;
	turnMaxC = 10;
	init();
}
ntSwarm::ntSwarm(std::vector<ntAgent*>* agents, float s, float a, float c, float turnMaxA, float turnMaxC, float turnMaxS):
agents(agents), zoneS(s), zoneA(a), zoneC(c),turnMaxA(turnMaxA), turnMaxC(turnMaxC), turnMaxS(turnMaxS){
	init();
}

void ntSwarm::init(){
	runCylce = 0;
	this->zoneR = zoneC;
	this->zoneSqrdR = zoneR * zoneR;
	this->zoneSqrdS = zoneS * zoneS;
	this->zoneSqrdA = zoneA * zoneA;
	this->zoneSqrdC = zoneC * zoneC;
}

void ntSwarm::setAgents(std::vector<ntAgent*>* agents){
	this->agents = agents;
}
void ntSwarm::addAgents(Vec3 pos, ntVec3 heading, float vision, int num){
	for (int i = 0; i < num; i++){
		ntAgent a = ntAgent(&pos,heading,vision);
		agents->push_back(&a);
	}
	totalAgents = agents->size();
}
void ntSwarm::clearAgents(){
	agents->clear();
}

void ntSwarm::detectNeighbors(ntAgent* a){
	int countNeighbors = 0;
	
	//find all neighbors within distance separationMin
	for (int i = 0; i < agents->size(); i++){
		float distSqrd = agents->at(i)->pos->distSqrd(a->pos);
		if (distSqrd < zoneSqrdR & distSqrd > 0){
			a->addNeighbor(agents->at(i));
			countNeighbors++;
			//std::cout << "get neighbors";
		}
	}

	//find nearest neighbor
	float distNearest = 100000000;
	if (countNeighbors > 0){
		for (int i = 0; i < countNeighbors; i++){
			float distSqrd = a->neighbors.at(i)->pos->distSqrd(a->pos);
			if (distSqrd < distNearest){
				distNearest = distSqrd;
				a->nearestNeighbor = a->neighbors.at(i);
				//std::cout << "nearest neighbor" << a->nearestNeighbor << endl;
				//std::cout << "get nearest neighbor";
			}
		}
	}
}
void ntSwarm::applyAlignment(ntAgent* a){
	int n = a->neighbors.size();
	//copy heading for calculation purposes
	Vec3* heading0 = &Vec3(a->heading.x, a->heading.y, a->heading.z);
	heading0->unitize();
	//compute average heading of all neighbors
	for (int i = 0; i < n; i++){
		Vec3 heading1 = a->neighbors.at(i)->heading;
		heading1.unitize();
		heading0->add(&heading1);
	}
	heading0->div(n+1);
	heading0->unitize();
	///std::cout << "headingAvg A:  " << heading0->z << endl;
	//compare resulting rotation from original heading
	float turn = a->heading.dot(heading0);
	if (turn > turnMaxA){
		///turn = turnMaxA;
	}
	//calculate axis of rotation
	Vec3 axis = a->heading.cross(heading0);
	//apply rotation to vector
	ntMatrix4 transform = ntMatrix4(heading0);
	transform.rotate3d(turn, axis);
	a->heading.set(heading0);
	//a->heading.unitize();
	///std::cout << "heading A:  " << a->heading.z << endl;
}
void ntSwarm::applyCohesion(ntAgent* a){
	int n = a->neighbors.size();
	//copy heading for calculation purposes
	Vec3* heading0 = &Vec3(a->heading.x, a->heading.y, a->heading.z);
	heading0->unitize();
	//compute average bearing of each neighbor
	for (int i = 0; i < n; i++){
		Vec3* pos1 = new Vec3(0, 0, 0);
		pos1->set(a->neighbors.at(i)->getPos());
		pos1->sub(a->getPos());
		Vec3 bearing = Vec3(pos1->x,pos1->y,pos1->z);
		bearing.unitize();
		heading0->add(&bearing);
	}
	heading0->mult(1 / (n + 1));
	heading0->unitize();
	//compare resulting rotation from original heading
	float turn = a->heading.dot(heading0);
	if (turn > turnMaxC){
		///turn = turnMaxC;
	}
	//calculate axis of rotation
	Vec3 axis = a->heading.cross(heading0);
	//apply rotation to vector
	ntMatrix4 transform = ntMatrix4(heading0);
	transform.rotate3d(turn, axis);
	a->heading.set(heading0);
	//a->heading.unitize();
	///std::cout << "headingX C:  " << a->heading.z << endl;
}
void ntSwarm::applySeparation(ntAgent* a){
	//compute new heading based on heading of nearest neighbor
	Vec3* heading0 = new Vec3(a->heading.x, a->heading.y, a->heading.z);
	Vec3* heading1 = new Vec3(a->nearestNeighbor->heading.x, a->nearestNeighbor->heading.y, a->nearestNeighbor->heading.z);
	
	float turn = heading0->dot(heading1);
	Vec3 axis = heading0->cross(heading1);

	if (turn > turnMaxS){
		///turn = turnMaxS;
	}
	ntMatrix4 heading = ntMatrix4(heading0);
	heading.rotate3d(turn, axis);

	a->heading.set(heading0);
	///std::cout << "headingX S:  " << a->heading.z << endl;
	//a->heading.unitize();
}

void ntSwarm::setAlignmentTurnMax(float a){
	this->turnMaxA = a;
}
void ntSwarm::setCohesionTurnMax(float c){
	this->turnMaxC = c;
	}
void ntSwarm::setSeparationTurnMax(float s){
	this->turnMaxS = s;
}

void ntSwarm::run(){
	for (int i = 0; i < agents->size(); i++){
		ntAgent* a = agents->at(i);
		
		if (a->isFree()){
			detectNeighbors(a);
			int countNeighbors = a->neighbors.size();

			if (countNeighbors > 0){
				for (int j = 0; j < countNeighbors; j++){
					float distSqrd = a->pos->distSqrd(a->neighbors.at(j)->pos);
					
					if (distSqrd < zoneSqrdR){
						///float weight = distSqrd / zoneSqrd;
						if (distSqrd < zoneSqrdS){
							applySeparation(a);
						} 
						else if (distSqrd < zoneSqrdA){
							applyAlignment(a);
						}
						else if (distSqrd < zoneSqrdC){
							applyCohesion(a);
						}
					}
				}
			}
			//advance forward 1 step
			///this needs to be reconciled with
			///approach used for physics based particles
			///which are advance by the engine
			///APPLY HEADING TO VELOCITY

			float velocity = a->getVel()->mag();
			//std::cout << velocity << endl;
			//a->heading.mult(velocity);
			//a->getPos()->add(&a->heading);
		}
	}
	//std::cout << "runCylce: " << runCylce << endl;
	//runCylce++;
}