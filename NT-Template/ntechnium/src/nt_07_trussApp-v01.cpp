#include "nt_07_trussApp-v01.h"


void trussApp01::init() {

	///////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////// OBJECTS
	float pi = M_PI;
	float E = 3e7;					//Young's Modulus	    //psi       
	float v = 0.3;					//Poisson's Ratio
	float d = 2;					//Diameter				//inches
	float a = pi * pow((d / 2), 2);	//Area					//Square Inches
	float rho = pow(7.3, -4);		//Mass Density			//(lb*sec ^ 2) / in ^ 4 

	////////////////////////vector of  nodes within 3d truss system
	std::vector <ntVec3*> vecs;
	std::vector <ntNode*> nodes;

	Vec3* v01 = new Vec3(-18, 0, 96);
	Vec3* v02 = new Vec3(18, 0, 96);
	Vec3* v03 = new Vec3(-18, 18, 48);
	Vec3* v04 = new Vec3(18, 18, 48);
	Vec3* v05 = new Vec3(18, -18, 48);
	Vec3* v06 = new Vec3(-18, -18, 48);
	Vec3* v07 = new Vec3(-48, 48, 0);
	Vec3* v08 = new Vec3(48, 48, 0);
	Vec3* v09 = new Vec3(48, -48, 0);
	Vec3* v10 = new Vec3(-48, -48, 0);

	vecs.push_back(v01);
	vecs.push_back(v02);
	vecs.push_back(v03);
	vecs.push_back(v04);
	vecs.push_back(v05);
	vecs.push_back(v06);
	vecs.push_back(v07);
	vecs.push_back(v08);
	vecs.push_back(v09);
	vecs.push_back(v10);

	ntNode* n01 = new ntNode(v01);
	ntNode* n02 = new ntNode(v02);
	ntNode* n03 = new ntNode(v03);
	ntNode* n04 = new ntNode(v04);
	ntNode* n05 = new ntNode(v05);
	ntNode* n06 = new ntNode(v06);
	ntNode* n07 = new ntNode(v07);
	ntNode* n08 = new ntNode(v08);
	ntNode* n09 = new ntNode(v09);
	ntNode* n10 = new ntNode(v10);

	nodes.push_back(n01);
	nodes.push_back(n02);
	nodes.push_back(n03);
	nodes.push_back(n04);
	nodes.push_back(n05);
	nodes.push_back(n06);
	nodes.push_back(n07);
	nodes.push_back(n08);
	nodes.push_back(n09);
	nodes.push_back(n10);

	std::vector <std::array <int, 2>> node_Rel;
	//element nodes:  [v0, v1]
	std::array<int, 2> elemN_01 = { 0, 1 };
	std::array<int, 2> elemN_02 = { 0, 3 };
	std::array<int, 2> elemN_03 = { 1, 2 };
	std::array<int, 2> elemN_04 = { 0, 4 };
	std::array<int, 2> elemN_05 = { 1, 5 };
	std::array<int, 2> elemN_06 = { 1, 3 };
	std::array<int, 2> elemN_07 = { 1, 4 };
	std::array<int, 2> elemN_08 = { 0, 2 };
	std::array<int, 2> elemN_09 = { 0, 5 };
	std::array<int, 2> elemN_10 = { 2, 5 };
	std::array<int, 2> elemN_11 = { 3, 4 };
	std::array<int, 2> elemN_12 = { 2, 3 };
	std::array<int, 2> elemN_13 = { 4, 5 };
	std::array<int, 2> elemN_14 = { 2, 9 };
	std::array<int, 2> elemN_15 = { 5, 6 };
	std::array<int, 2> elemN_16 = { 3, 8 };
	std::array<int, 2> elemN_17 = { 4, 7 };
	std::array<int, 2> elemN_18 = { 3, 6 };
	std::array<int, 2> elemN_19 = { 2, 7 };
	std::array<int, 2> elemN_20 = { 4, 9 };
	std::array<int, 2> elemN_21 = { 5, 8 };
	std::array<int, 2> elemN_22 = { 5, 9 };
	std::array<int, 2> elemN_23 = { 2, 6 };
	std::array<int, 2> elemN_24 = { 3, 7 };
	std::array<int, 2> elemN_25 = { 4, 8 };

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
	node_Rel.push_back(elemN_11);
	node_Rel.push_back(elemN_12);
	node_Rel.push_back(elemN_13);
	node_Rel.push_back(elemN_14);
	node_Rel.push_back(elemN_15);
	node_Rel.push_back(elemN_16);
	node_Rel.push_back(elemN_17);
	node_Rel.push_back(elemN_18);
	node_Rel.push_back(elemN_19);
	node_Rel.push_back(elemN_20);
	node_Rel.push_back(elemN_21);
	node_Rel.push_back(elemN_22);
	node_Rel.push_back(elemN_23);
	node_Rel.push_back(elemN_24);
	node_Rel.push_back(elemN_25);

	//APPLY BOUNDARY CONDITIONS
	n07->set_BC(FIX);
	n08->set_BC(FIX);
	n09->set_BC(FIX);
	n10->set_BC(FIX);

	//APPLY FORCE COMPONENT AT EACH NODE
	//float F1[] = { 0, 60000, 0 };
	//float F2[] = { 0, 60000, 0 };
	//af::array f1(3,1, F1);
	//af::array f2(3,1, F2);

	Vec3 f1= Vec3(0, 60000, 0);
	Vec3 f2 = Vec3(0, 60000, 0);

	n01->set_FC(f1);
	n02->set_FC(f2);
	
	//CONSTRUCT ELEMENTS FROM NODE RELATIONS
	std::vector <ntElement*> elements;
	int elemCnt = node_Rel.size();
	for (int i= 0; i < elemCnt; ++i){
		int n0_i = node_Rel.at(i)[0];
		int n1_i = node_Rel.at(i)[1];
		ntElement* elem = new ntElement(nodes.at(n0_i), nodes.at(n1_i), a, E, node_Rel.at(i));
		elements.push_back(elem);
	}

	//CONSTRUCT SPACE TRUSS FROM ELEMENTS
	truss00 = ntTruss(nodes, elements);
	truss00.display_mode(vF);
	truss00.write_txt("FEM_001");
	///////////////////////////////////////////////////////////////
	/////////////////////////////////////// OBJECTS COLOR BY VERTEX
	int n = nodes.size();
	for (int i = 0; i < n; i++){
		ntMatrix4 mat = ntMatrix4( nodes.at(i)->pos );
		mat.scale3d(0.01);
	}

	glRotatef(-60, 1.f, 0.f, 0.f);
	glRotatef(45, 0.f, 0.f, 1.f);

}

void trussApp01::run(){
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
		truss00.display_mode(vQ);
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS){
		truss00.display_mode(vF);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
		truss00.display_mode(vS);
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS){
		truss00.display_mode(vP);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
		truss00.display_mode(vW);
	}
	display();
}

void trussApp01::display(){
	//glRotatef(-.01, 0.f, 0.f, 1.f);
	truss00.display();
}