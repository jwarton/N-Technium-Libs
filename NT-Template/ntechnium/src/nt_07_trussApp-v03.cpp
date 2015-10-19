#include "nt_07_trussApp-v03.h"


void trussApp03::init() {


	///////////////////////////////////////////////////////////////
	//////////////////////////////////////////// ELEMENT PARAMETERS
	float E = 2e11;															//Young's Modulus	    //Pa       
	float d = 0.05;															//Diameter				//meters
	float a = M_PI * pow((d / 2), 2);										//Area					//sq meters

	ntVec3 Fz1 = Vec3(0, 0, -3000);											//Point Load			//N

	///////////////////////////////////////////////////////////////
	////////////////////////// DEFINE VECTORS FOR TRUSS CONSTRUCTOR
	std::vector <ntVec3*>* vecs;
	std::vector <ntNode*> nodes;
	std::vector <std::array <int, 2>> node_Rel;
	std::vector <ntElement*> elements;

	ntTextIO ptData00 = ntTextIO("ptPos_01_spaceTruss.txt");
	mesh00 = ntMeshPts(ptData00.getVecsV());								//GENERIC MESH VIEW
	vecs = ptData00.getVecsV();

	int nodes_Unique = 2;
	int nodes_Corners  = 4;
	int node_N = nodes_Corners + nodes_Unique;								//NODES/ CELL
	int rows_X = ptData00.get_U();											//POINTS IN X
	int rows_Y = ptData00.get_V();											//POINTS IN Y
	int Umax = rows_X - 1;													// CELLS IN U
	int Vmax = rows_Y - 1;													// CELLS IN V
	int cell_T = ((Umax) * (Vmax));											//TOTAL CELLS
	int Ui = 1;																//U INDEX 
	int Vi = 1;																//V INDEX
	int Ci = 0;																//CELL INDEX
	int i = 0;

	///////////////////////////////////////////////////////////////
	////////////////////////////////// INDEX ALL NODES AND ELEMENTS
	while (Ci < cell_T){
		i += 1;

		int ind_01;
		int ind_02;
		int ind_03;
		int ind_04;

		if (fmod(i, rows_X) != 0){
			///////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////// DEFINE CELL
			Ci += 1;
	
			ind_01 = i;
			ind_02 = i - 1;
			ind_03 = i - 1 + rows_X;
			ind_04 = i + rows_X;

			///////////////////////////////////////////////////////////////
			/////////////////////////// DEFINE COORDINATES FOR UNIQUE NODES
			float dx = abs(vecs->at(ind_01)->x - vecs->at(ind_02)->x);
			float dy = abs(vecs->at(ind_04)->y - vecs->at(ind_01)->y);
			float dz = abs(vecs->at(ind_04)->z - vecs->at(ind_01)->z);

			float posX = vecs->at(ind_01)->x - (dx * 0.5);
			float posY = vecs->at(ind_04)->y - (dy * 0.5);
			float posZ = vecs->at(ind_04)->z - (dz * 0.5);

			dz += dx * 0.1; ///REFACTOR USING SURFACE NORMAL

			Vec3* v05 = new Vec3(posX, posY, posZ + (dz));
			Vec3* v06 = new Vec3(posX, posY, posZ - (dz));

			///////////////////////////////////////////////////////////////
			////////////////////////////////////////////////// DEFINE NODES
			if (Vi == 1){																		// PUSH ON FIRST ROW ONLY
				ntNode* n01 = new ntNode(vecs->at(ind_01));
				nodes.push_back(n01);
			}
			if (Ui == 1 && Vi == 1){															// PUSH ON FIRST CELL OF EACH ROW
				ntNode* n02 = new ntNode(vecs->at(ind_02));
				ntNode* n03 = new ntNode(vecs->at(ind_03));
				nodes.push_back(n02);
				nodes.push_back(n03);
			} else if (Ui == 1){																// PUSH ON FIRST CELL OF EACH ROW
				ntNode* n03 = new ntNode(vecs->at(ind_03));
				nodes.push_back(n03);
			}
																								// PUSH REMAINING NODES FOR ALL CELLS
			ntNode* n04 = new ntNode(vecs->at(ind_04));
			ntNode* n05 = new ntNode(v05);
			ntNode* n06 = new ntNode(v06);

			nodes.push_back(n04);
			nodes.push_back(n05);
			nodes.push_back(n06);

			///////////////////////////////////////////////////////////////
			////////////////////////////////////// CALCULATE NODE RELATIONS

			int n_T = nodes.size();
			int n0 = 0;
			int n1 = 0;
			int n2 = 0;
			int n3 = n_T - 3;
			int n4 = n_T - 2;
			int n5 = n_T - 1;
			int n6 = 0;
			int n7 = 0;
			int n8 = 0;
			int n9 = 0;

			if (Vi == 1 && Ui == 1){															// FIRST UV 
				n0 = 0;
				n1 = 1;
				n2 = 2;
			}
			if (Vi == 1 && Ui == 2){															// SECOND CELL OF FIRST ROW
				n0 = n_T - 4;
				n1 = n_T - 10;
				n2 = n_T - 7;
				n8 = n_T - 6;
				n9 = n_T - 5;
			}
			if (Vi == 1 && Ui > 2){																// REMAINING CELLS OF ROW
				n0 = n_T - 4;
				n1 = n_T - 8;
				n2 = n_T - 7;
				n8 = n_T - 6;
				n9 = n_T - 5;
			}
			if (Vi ==2 && Ui == 1){																// FIRST CELL OF SECOND ROW
				n0 = n_T - (4 * Umax) - nodes_Unique - 1;
				n1 = n_T - (4 * Umax) - nodes_Unique - 2;
				n2 = n_T - 4;
				n6 = n_T - (4 * Umax) - 1;
				n7 = n_T - (4 * Umax) - 2;
			}
			if (Vi ==2 && Ui >= 2){																// REMAINING CELLS OF SECOND ROW
				n0 = n_T - (4 * (Umax - Ui) + (3 * Ui + 1)) - nodes_Unique - 1;
				n1 = n_T - (4 * (Umax - Ui) + (3 * Ui + 1)) - nodes_Unique - 5;
				n2 = n_T - 6;
				n6 = n_T - (4 * (Umax - Ui) + (3 * Ui + 1)) - 1;
				n7 = n_T - (4 * (Umax - Ui) + (3 * Ui + 1)) - 2;
				n8 = n_T - 5;
				n9 = n_T - 4;
			}
			if (Vi > 2 && Ui == 1){																// FIRST CELL OF REMAINING ROWS
				n0 = n_T - (3 * Umax + 1) - nodes_Unique - 1;
				n1 = n_T - (3 * Umax + 1) - nodes_Unique - 2;
				n2 = n_T - 4;
				n6 = n_T - (3 * Umax + 1) - 1;
				n7 = n_T - (3 * Umax + 1) - 2;
			}
			if (Vi > 2 && Ui >= 2){																// ALL REMAINING CELLS
				n0 = n_T - (3 * Umax + 1) - nodes_Unique - 1;
				n1 = n_T - (3 * Umax + 1) - nodes_Unique - 4;
				n2 = n_T - 6;
				n6 = n_T - (3 * Umax + 1) - 1;
				n7 = n_T - (3 * Umax + 1) - 2;
				n8 = n_T - 5;
				n9 = n_T - 4;
			}
			
			///////////////////////////////////////////////////////////////
			///////////////// NODE RELATIONSHIP FOR EACH ELEMENT:  [v0, v1]
			std::array<int, 2> elemN_01 = { n0, n1 };
			std::array<int, 2> elemN_02 = { n1, n2 };
			std::array<int, 2> elemN_05 = { n0, n4 };
			std::array<int, 2> elemN_06 = { n1, n4 };
			std::array<int, 2> elemN_07 = { n2, n4 };
			std::array<int, 2> elemN_08 = { n3, n4 };
			std::array<int, 2> elemN_09 = { n0, n5 };
			std::array<int, 2> elemN_10 = { n1, n5 };
			std::array<int, 2> elemN_11 = { n2, n5 };
			std::array<int, 2> elemN_12 = { n3, n5 };
			std::array<int, 2> elemN_13 = { n4, n5 };

			node_Rel.push_back(elemN_01);
			node_Rel.push_back(elemN_02);

			if (Vi == Vmax){
				std::array<int, 2> elemN_03 = { n2, n3 };
				node_Rel.push_back(elemN_03);
			}
			if (Ui == Umax){
				std::array<int, 2> elemN_04 = { n3, n0 };
				node_Rel.push_back(elemN_04);
			}

			node_Rel.push_back(elemN_05);
			node_Rel.push_back(elemN_06);
			node_Rel.push_back(elemN_07);
			node_Rel.push_back(elemN_08);
			node_Rel.push_back(elemN_09);
			node_Rel.push_back(elemN_10);
			node_Rel.push_back(elemN_11);
			node_Rel.push_back(elemN_12);
			node_Rel.push_back(elemN_13);

			if (n6 != 0){
				std::array<int, 2> elemN_14 = { n5, n6 };
				node_Rel.push_back(elemN_14);
			}
			if (n7 != 0){
				std::array<int, 2> elemN_15 = { n4, n7 };
				node_Rel.push_back(elemN_15);
			}
			if (n8 != 0){
				std::array<int, 2> elemN_16 = { n4, n8 };
				node_Rel.push_back(elemN_16);
			}
			if (n9 != 0){
				std::array<int, 2> elemN_17 = { n5, n9 };
				node_Rel.push_back(elemN_17);
			}
			///////////////////////////////////////////////////////////////
			//////////////////////////////////////APPLY BOUNDARY CONDITIONS

			if (Ui == 1 && Vi == 1){
				//nodes.at(n0)->set_BC(FIX);
				nodes.at(n1)->set_BC(FIX);
				//nodes.at(n2)->set_BC(FIX);
				//nodes.at(n3)->set_BC(FIX);
			}
			if (Ui == 1 && Vi == Vmax){
				//nodes.at(n0)->set_BC(FIX);
				//nodes.at(n1)->set_BC(FIX);
				nodes.at(n2)->set_BC(FIX);
				//nodes.at(n3)->set_BC(FIX);
			}
			if (Ui == Umax && Vi == 1){
				nodes.at(n0)->set_BC(FIX);
				//nodes.at(n1)->set_BC(FIX);
				//nodes.at(n2)->set_BC(FIX);
				//nodes.at(n3)->set_BC(FIX);
			}
			if (Ui == Umax && Vi == Vmax){
				//nodes.at(n0)->set_BC(FIX);
				//nodes.at(n1)->set_BC(FIX);
				//nodes.at(n2)->set_BC(FIX);
				nodes.at(n3)->set_BC(FIX);
			}
			///////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////// APPLY FORCE
			nodes.at(n4)->set_FC(Fz1);
			nodes.at(n5)->set_FC(Fz1);

			///////////////////////////////////////////////////////////////
			////////////////////////////////////////////////// INCREMENT UV
			if (Ui < Umax){
				Ui += 1;
			} else if (Ui == Umax){
				Ui = 1;
				Vi += 1;
			}
		}
	}
	///////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////// DEFINE ELEMENTS
	int elemCnt = node_Rel.size();
	for (int i= 0; i < elemCnt; i++){
		int n0_i = node_Rel.at(i)[0];
		int n1_i = node_Rel.at(i)[1];
		ntElement* elem = new ntElement(nodes.at(n0_i), nodes.at(n1_i), a, E, node_Rel.at(i));
		elements.push_back(elem);
	}
	///////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////// DEFINE SPACE TRUSS
	truss00 = ntTruss(nodes, elements);
	truss00.display_mode(vQ);
	string filename = "FEM_" + to_string(Umax) + "x" + to_string(Vmax);
	truss00.write_txt(filename);
	///////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////// GRAPH EVAL PERFORMANCE
	graph00 = ntGraph();
	std::cout << "from trussApp:             "  << truss00.get_t_CPU().at(0) << endl;
	graph00.set_Param(truss00.get_t_CPU(), 1);
	graph00.set_Param(truss00.get_t_GPU(), 1);
	graph00.set_Param(truss00.get_t_copy(), 1);
	graph00.init();
	///////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////// CAMERA OREINTATION
	int n = nodes.size();
	for (int i = 0; i < n; i++){
		ntMatrix4 mat = ntMatrix4(nodes.at(i)->pos);
		mat.scale3d(0.1);
	}

	glRotatef(55, 1.f, 0.f, 0.f);
	glRotatef(-50, 0.f, 0.f, 1.f);
}

void trussApp03::run(){
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

void trussApp03::display(){
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS){
		//mesh00.display();
		//mesh00.displayVerts(3);
		mesh00.displayEdges(.1);
	}
	else {
		truss00.display();
		graph00.display();
	}
}