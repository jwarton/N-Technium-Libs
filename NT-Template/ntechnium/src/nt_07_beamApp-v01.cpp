#include "nt_07_beamApp-v01.h"


void beamApp01::init() {
	///////////////////////////////////////////////////////////////
	//////////////////////////////////////////// ELEMENT PARAMETERS
	float E = 3e8;															//Young's Modulus	    //psi

	ntVec3 F1 = Vec3(0, 2400, 0);											//Point Load			//lbs
	ntVec3 F2 = Vec3(0, 1200, 0);											//Point Load			//lbs
	ntVec3 M3 = Vec3(0,-3000, 0);											//Coupling Load			//ft*lbs

	///////////////////////////////////////////////////////////////
	////////////////////////// DEFINE VECTORS FOR TRUSS CONSTRUCTOR
	std::vector <ntNode*> nodes;
	std::vector <std::array <int, 2>> node_Rel;
	std::vector <ntElement*> elements;

	///////////////////////////////////////////////////////////////
	/////////////////////////// DEFINE COORDINATES FOR UNIQUE NODES
	int x1 = 0;
	int x2 = 12 * 25;
	int z1 = 0;
	int z2 = 12 * 16;
	int z3 = 12 * 32;

	Vec3* v01 = new Vec3( x1, 0, z1);
	Vec3* v02 = new Vec3( x2, 0, z1);
	Vec3* v03 = new Vec3( x1, 0, z2);
	Vec3* v04 = new Vec3( x2, 0, z2);
	Vec3* v05 = new Vec3( x1, 0, z3);
	Vec3* v06 = new Vec3( x2, 0, z3);

	///////////////////////////////////////////////////////////////
	////////////////////////////////////////////////// DEFINE NODES
	ntNode* n01 = new ntNode(v01, 6);
	ntNode* n02 = new ntNode(v02, 6);
	ntNode* n03 = new ntNode(v03, 6);
	ntNode* n04 = new ntNode(v04, 6);
	ntNode* n05 = new ntNode(v05, 6);
	ntNode* n06 = new ntNode(v06, 6);

	nodes.push_back(n01);
	nodes.push_back(n02);
	nodes.push_back(n03);
	nodes.push_back(n04);
	nodes.push_back(n05);
	nodes.push_back(n06);

	///////////////////////////////////////////////////////////////
	////////////////////////////////////// CALCULATE NODE RELATIONS
	int n1 = 0;
	int n2 = 1;
	int n3 = 2;
	int n4 = 3;
	int n5 = 4;
	int n6 = 5;

	///////////////////////////////////////////////////////////////
	///////////////// NODE RELATIONSHIP FOR EACH ELEMENT:  [v0, v1]
	std::array<int, 2> elemN_01 = { n1, n3 };
	std::array<int, 2> elemN_02 = { n2, n4 };
	std::array<int, 2> elemN_03 = { n3, n5 };
	std::array<int, 2> elemN_04 = { n4, n6 };
	std::array<int, 2> elemN_05 = { n3, n4 };
	std::array<int, 2> elemN_06 = { n5, n6 };
	std::array<int, 2> elemN_07 = { n1, n4 };
	std::array<int, 2> elemN_08 = { n2, n3 };
	std::array<int, 2> elemN_09 = { n3, n6 };
	std::array<int, 2> elemN_10 = { n4, n5 };

	node_Rel.push_back(elemN_01);
	node_Rel.push_back(elemN_02);
	node_Rel.push_back(elemN_03);
	node_Rel.push_back(elemN_04);
	node_Rel.push_back(elemN_05);
	node_Rel.push_back(elemN_06);
	node_Rel.push_back(elemN_07);
	node_Rel.push_back(elemN_08);
	node_Rel.push_back(elemN_09);
	node_Rel.push_back(elemN_10);

	///////////////////////////////////////////////////////////////
	//////////////////////////////////////APPLY BOUNDARY CONDITIONS
	nodes.at(n1)->set_BC(PIN);
	nodes.at(n2)->set_BC(PIN);
	
	///////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////// APPLY FORCE
	nodes.at(n1)->set_FC(F1);
	nodes.at(n3)->set_FC(F2);
	nodes.at(n3)->set_MC(M3);

	///////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////// DEFINE ELEMENTS
	//COLUMN ELEMENTS
	ntElement* elem01 = new ntElement(nodes.at(n1), nodes.at(n3), 10, 150, E, node_Rel.at(0));
	ntElement* elem02 = new ntElement(nodes.at(n2), nodes.at(n4), 10, 150, E, node_Rel.at(1));
	ntElement* elem03 = new ntElement(nodes.at(n3), nodes.at(n5), 10, 150, E, node_Rel.at(2));
	ntElement* elem04 = new ntElement(nodes.at(n4), nodes.at(n6), 10, 150, E, node_Rel.at(3));
	//BEAM ELEMENTS
	ntElement* elem05 = new ntElement(nodes.at(n3), nodes.at(n4), 12, 300, E, node_Rel.at(4));
	ntElement* elem06 = new ntElement(nodes.at(n5), nodes.at(n6), 12, 200, E, node_Rel.at(5));
	//BRACING ELEMENTS
	ntElement* elem07 = new ntElement(nodes.at(n1), nodes.at(n4), 2, E, node_Rel.at(6));
	ntElement* elem08 = new ntElement(nodes.at(n2), nodes.at(n3), 2, E, node_Rel.at(7));
	ntElement* elem09 = new ntElement(nodes.at(n3), nodes.at(n6), 2, E, node_Rel.at(8));
	ntElement* elem10 = new ntElement(nodes.at(n4), nodes.at(n5), 2, E, node_Rel.at(9));
	
	elements.push_back(elem01);
	elements.push_back(elem02);
	elements.push_back(elem03);
	elements.push_back(elem04);
	elements.push_back(elem05);
	elements.push_back(elem06);
	elements.push_back(elem07);
	elements.push_back(elem08);
	elements.push_back(elem09);
	elements.push_back(elem10);

	///////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////// DEFINE SPACE TRUSS
	frame00 = ntTruss(nodes, elements);
	///frame00.display_mode(vQ);
	///frame00.write_txt("FEM_004");

	///////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////// CAMERA OREINTATION
	int n = nodes.size();
	for (int i = 0; i < n; i++){
		ntMatrix4 mat = ntMatrix4(nodes.at(i)->pos);
		mat.scale3d(0.05);
	}

	//glRotatef(-90, 1.f, 0.f, 0.f);
	glRotatef(-30, 1.f, 0.f, 0.f);
	//glRotatef(15, 0.f, 0.f, 1.f);
}

void beamApp01::run(){
	display();
}

void beamApp01::display(){
	//glRotatef(-.01, 0.f, 0.f, 1.f);
	///frame00.display();
}