
#include "ntTruss3d.h"

ntTruss::ntTruss(){}
ntTruss::ntTruss(std::vector <ntElement*> elements):
elements(elements){
	for (int i = 0; i<elements.size(); i++){
		elements.at(i)->w=1;
		elements.at(i)->col = ntColor4f(1,.2,.5);
	}
	init();
}
ntTruss::ntTruss(std::vector <ntNode*> nodes, std::vector <ntElement*> elements) :
nodes(nodes),elements(elements){
	for (int i = 0; i<nodes.size(); i++){
		nodes.at(i)->col = ntColor4f(1, .1, .2);
	}
	init();
}
void ntTruss::init(){

	elementCnt = elements.size();
	nodeCnt = nodes.size();
	for (int i = 0; i < nodeCnt; i++){
		rows += nodes.at(i)->get_DOF();
		DOF = nodes.at(i)->get_DOF();
	}
	cols = rows;

	using namespace arma;
	F_host  = zeros<mat>(rows, 1);
	Q_host  = zeros<mat>(rows, 1);
	K_host  = zeros(rows, cols);
	Kb_host = zeros<mat>(rows, cols);
	P_host  = zeros<mat>(elementCnt, 2);
	S_host  = zeros<mat>(elementCnt, 1);

	eval_GPU();
	//eval_CPU();
	copy_host();

	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////// REMOVE BC ROUNDING ERRORS
	float Bc = 0.1;
	float Bd = -0.1;

	std::array <bool, 1> lt;
	std::array <bool, 1> gt;

	for (int i = 0; i < rows; i++) {

		af::operator<(F(i, 0), Bc).host((void*)&lt);
		af::operator>(F(i, 0), Bd).host((void*)&gt);
		if (lt[0] && gt[0]) {
			F(i, 0) = af::abs(af::round(F(i, 0)));
		}
	}
	Bc =  0.00001;
	Bd = -0.00001;
	for (int i = 0; i < rows; i++) {
		af::operator<(Q(i, 0), Bc).host((void*)&lt);
		af::operator>(Q(i, 0), Bd).host((void*)&gt);
		if (lt[0] && gt[0]) {
			Q(i, 0) = af::abs(round(Q(i, 0)));
		}
	}

	///////////////////////////////////////////////////////////////////////////
	////////////////////////////////////// UPDATE NODE WITH DISPLACEMENT VALUES
	for (int i = 0; i < nodeCnt; i++){
		DOF = nodes.at(i)->get_DOF();
		int u = i * DOF;
		int v = i * DOF + 1;
		int w = i * DOF + 2;
		nodes.at(i)->Q.x = Q_h[u];
		nodes.at(i)->Q.y = Q_h[v];
		nodes.at(i)->Q.z = Q_h[w];
	}
	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////// DEFINE AXIAL FORCE MATRIX
	for (int i = 0; i < elementCnt; i++) {
		float f0 = elements.at(i)->get_p().at(0, 0);
		float f1 = elements.at(i)->get_p().at(1, 0);
		P_h[i] = f0;
		P_h[i + elementCnt] = f1;
	}
	///////////////////////////////// DEFINE ELEMENT STRESS MATRIX
	sMax = 0;
	sMin = 0;
	for (int i = 0; i < elementCnt; i++) {
		float a = elements.at(i)->get_A();
		S(i, 0) = P(i, 1) / a;
		if (sMax < S_h[i]) {
			sMax = S_h[i];
			eMax = i;
		}
		if (sMin > S_h[i]) {
			sMin = S_h[i];
			eMin = i;
		}
	}
}

void ntTruss::eval_GPU(){
	//COPY TO GPU
	F = af::constant(0, rows, 1);
	Q = af::constant(0, rows, 1);
	K = af::constant(0, rows, cols);
	Kb = af::constant(0, rows, cols);
	P = af::constant(0, elementCnt, 2);
	S = af::constant(0, elementCnt, 1);

	def_FG();
	def_KG();
	def_BC();

	/////COPY HOST TO GPU

	Q = af::solve(Kb, F);									//dense system of linear equations
	F = af::matmul(K, Q);									//F = K * Q;
}
void ntTruss::eval_CPU() {
}
void ntTruss::copy_host() {

	std::array <float, 1> val;

	int Q_dim = Q.elements();
	for (int i = 0; i < Q_dim; i++) {
		Q(i).host((void*)&val);
		Q_h.push_back(val[0]);
	}

	int F_dim = F.elements();
	for (int i = 0; i < F_dim; i++) {
		F(i).host((void*)&val);
		F_h.push_back(val[0]);
	}

	int S_dim = S.elements();
	for (int i = 0; i < S_dim; i++) {
		S(i).host((void*)&val);
		//ROUND OFF NEGIGIBLE VALUES
		if (val[0] < .01 && val[0] > -.01) {
			val[0] = abs(round(val[0]));
		}
		S_h.push_back(val[0]);
	}

	int P_dim = P.elements();
	for (int i = 0; i < P_dim; i++) {
		P(i).host((void*)&val);
		//ROUND OFF NEGIGIBLE VALUES
		if (val[0] < .01 && val[0] > -.01) {
			val[0] = abs(round(val[0]));
		}
		P_h.push_back(val[0]);
	}
}

void ntTruss::def_KG(){
	for (int i = 0; i < elementCnt; i++) {
		//ELEMENT STIFFNESS MATRIX(FIG. 2.52)
		arma::mat ke = elements.at(i)->get_k();

		int row_ID;
		int col_ID;
		int r_dof = 0;
		int c_dof = 0;

		for (int j = 0; j < ke.n_rows; j++) {
			if (r_dof >= DOF) {
				r_dof = 0;
			}
			for (int k = 0; k < ke.n_cols; k++) {

				float val = ke(j, k);	//contributing element stiffness 

				if (c_dof >= DOF) {
					c_dof = 0;
				}

				if (j < DOF & k < DOF) {
					row_ID = (DOF * elements.at(i)->get_nodeInd()[0]) + r_dof;
					col_ID = (DOF * elements.at(i)->get_nodeInd()[0]) + c_dof;
				}
				else if (j < DOF & k >= DOF) {
					row_ID = (DOF * elements.at(i)->get_nodeInd()[1]) + r_dof;
					col_ID = (DOF * elements.at(i)->get_nodeInd()[0]) + c_dof;
				}
				else if (j >= DOF & k < DOF) {
					row_ID = (DOF * elements.at(i)->get_nodeInd()[0]) + r_dof;
					col_ID = (DOF * elements.at(i)->get_nodeInd()[1]) + c_dof;
				}
				else if (j >= DOF & k >= DOF) {
					row_ID = (DOF * elements.at(i)->get_nodeInd()[1]) + r_dof;
					col_ID = (DOF * elements.at(i)->get_nodeInd()[1]) + c_dof;
				}
				K_host(row_ID, col_ID) += ke(j, k);
				c_dof += 1;
			}
			r_dof += 1;
		}
	}
}
void ntTruss::def_FG(){
	for (int i = 0; i < nodeCnt; i++){
		Vec3 f = nodes.at(i)->F;
		int n = i * 3;
		F_h[n    ] += f.x;
		F_h[n + 1] += f.y;
		F_h[n + 2] += f.z;
	}
}
void ntTruss::def_BC(){
	Kb = K;

	int n = nodes.size();
	for (int i = 0; i < n; i++){
		int bc = nodes.at(i)->get_BC();	//number of bc's at node
		BCs += bc;						//add boundary conditions to total
		///define matrix id for bc's within global K
		if (nodes.at(i)->free == false){
			int dofX = i * DOF + 0;
			int dofY = i * DOF + 1;
			int dofZ = i * DOF + 2;
			if (nodes.at(i)->fixed == true){
				bc_Id.push_back(dofX);
				bc_Id.push_back(dofY);
				bc_Id.push_back(dofZ);
			} else {
				if (nodes.at(i)->fixed_X == true){
					bc_Id.push_back(dofX);
				}
				if (nodes.at(i)->fixed_Y == true){
					bc_Id.push_back(dofY);
				}
				if (nodes.at(i)->fixed_Z == true){
					bc_Id.push_back(dofZ);
				}
			}
		}		
	}
	////IMPLEMENT BIG SPRING FACTOR 
	float bsFX = pow(10, 20); // Big Spring Factor
	for (int i = 0; i < BCs; i++){
		int index = bc_Id.at(i);
		Kb(index, index) = Kb(index, index)  + bsFX;
		F(index, 0) = 0;
	}
}
/*
void ntTruss::opt_Size(float sMax, float dMax, float QgMax){
	///SET DESIGN CRITERIA
	this->SeMax = sMax;
	this->QnMax = dMax;
	this->QgMax = QgMax;
	bool isValid;

	///CHECK ELEMENT STRESSES
	int n = elements.size();
	for (int i = 0; i < n; i++){
		if (S.at(i) > sMax){
			float a;
			float ds = sMax - S.at(i);
			//a = P.at(i) / 
			///REDUCE AREA
		} else {
			///INCREASE AREA
		}
	}

	///RE-EVALUATE DISPLACEMENT AND STRESS
	if (isValid == false){
		eval();
	}
}
*/
void ntTruss::write_txt(std::string s){
	///////////////////////////////// OUTPUT ALL RESULTS TO TEXT FILE TABLE
	std::string path = nt_Utility::getPathToOutput();
	std::string pathExtension = "fem\\";
	std::string fileName = s;
	std::string fileExt = ".txt";
	std::string	url = path + pathExtension + fileName + fileExt;
	std::ofstream file(url);

	std::string axis = "U";
	std::string sp;
	int val = 0;
	int node = 0;
	int n = elementCnt;

	file << "//////////////////////////////////////////////////////////////\n";
	file << "/////////////////////////////////// STRUCTURAL MATRIX ANALYSIS\n\n";
	file << "NODES:     " << nodeCnt << "\n";
	file << "ELEMENTS:  " << elementCnt << "\n\n";
	
	//file << "ELEMENT PROPERTIES:\n";
	file << "SAFETY FACTOR:  \n";
	file << "MAXIMUM DISPLACEMENT:        " << qMax << "\nAT NODE:                     " << nMax << "\n";
	/// MAX DISPLACEMENT VALUE AT NODE i
	file << "MAXIMUM TENSILE STRESS:      " << sMax << "\nAT ELEMENT:                  " << eMax << "\n";
	file << "MAXIMUM COMPRESSION STRESS:  " << abs(sMin) << "\nAT ELEMENT:                  " << eMin << "\n";
	//file << "SUMMATION:  0\n";
	/// DEFINE STRESS ABOVE SAFETY FACTOR
	file << "\n//////////////////////////////////////////////////////////////\n";
	file << "/////////////////////////////////////////// NODAL DISPLACEMENT\n";
	for (int i = 0; i < rows; i++){
		if (fmod(i, DOF) == 0){
			val = 0;
			node += 1;
		}
		if (val == 0){ axis = "u: "; }
		if (val == 1){ axis = "v: "; }
		if (val == 2){ axis = "w: "; }
		if (Q_h[i] >= 0){ sp = "  "; }
		if (Q_h[i] < 0){ sp = " "; }

		if (node < 10){
			file << "Q   " << node << "_" << axis << sp << Q_h[i] << endl;
		}
		else if (node < 100){
			file << "Q  " << node << "_" << axis << sp << Q_h[i] << endl;
		}
		else {
			file << "Q " << node << "_" << axis << sp << Q_h[i] << endl;
		}
		val += 1;
	}
	file << "\n//////////////////////////////////////////////////////////////\n";
	file << "///////////////////////////////////////////// ELEMENT STRESSES\n";
	for (int i = 0; i < n; i++){

		if (i < 9){
			if (S_h[i] >= 0){ sp = ":   "; }
			if (S_h[i] < 0){ sp = ":  "; }
			file << "S     " << i + 1 << sp << S_h[i] << endl;
		}
		else if (i < 99){
			if (S_h[i] >= 0){ sp = ":   "; }
			if (S_h[i] < 0){ sp = ":  "; }
			file << "S    " << i + 1 << sp << S_h[i] << endl;
		}
		else {
			if (S_h[i] >= 0){ sp = ":   "; }
			if (S_h[i] < 0){ sp = ":  "; }
			file << "S   " << i + 1 << sp << S_h[i] << endl;
		}
	}
	file << "\n//////////////////////////////////////////////////////////////\n";
	file << "///////////////////////////////////////////// FORCE COMPONENTS\n";
	node = 0;
	for (int i = 0; i < rows; i++){
		if (fmod(i, DOF) == 0){
			val = 0;
			node += 1;
		}
		if (val == 0){ axis = "u: "; }
		if (val == 1){ axis = "v: "; }
		if (val == 2){ axis = "w: "; }
		if (F_h[i] >= 0){ sp = "  "; }
		if (F_h[i] < 0){ sp = " "; }

		if (node < 10){
			file << "F   " << node << "_" << axis << sp << F_h[i] << endl;
		}
		else if (node < 100){
			file << "F  " << node << "_" << axis << sp << F_h[i] << endl;
		}
		else {
			file << "F " << node << "_" << axis << sp << F_h[i] << endl;
		}
		val += 1;
	}
	file << "\n//////////////////////////////////////////////////////////////\n";
	file << "///////////////////////////////////////////////// AXIAL FORCES\n";
	for (int i = 0; i < n ; i++){

		if (i < 9){
			if (P_h[i] >= 0){ sp = "  "; }
			if (P_h[i] < 0){ sp = " "; }
			file << "P   " << i + 1 << "_n0:" << sp << P_h[i] << endl;
			if (P_h[i + n] >= 0){ sp = "  "; }
			if (P_h[i + n] < 0){ sp = " "; }
			file << "P   " << i + 1 << "_n1:" << sp << P_h[i + n] << endl;
		}
		else if (i < 99){
			if (P_h[i] >= 0){ sp = "  "; }
			if (P_h[i] < 0){ sp = " "; }
			file << "P  " << i + 1 << "_n0:" << sp << P_h[i] << endl;
			if (P_h[i + n] >= 0){ sp = "  "; }
			if (P_h[i + n] < 0){ sp = " "; }
			file << "P  " << i + 1 << "_n1:" << sp << P_h[i+n] << endl;
		}
		else {
			///if (P(i, 0) >= 0){ sp = "  "; }
			///if (P(i, 0) < 0){ sp = " "; }
			///file << "P " << i+1 << "_n0:" << sp << P(i, 0) << endl;
			///if (P(i, 1) >= 0){ sp = "  "; }
			///if (P(i, 1) < 0){ sp = " "; }
			///file << "P " << i + 1 << "_n1:" << sp << P(i, 1) << endl;
		}
	}
	file.close();

}

void ntTruss::run(){


}
void ntTruss::display_mode(D_mode m){
	///////////////////////////////////////////////////////////////
	///////////////////////////////////////////// UPDATE COLOR MODE
	this->m = m;
	int n = nodes.size();
	int e = elements.size();
	int rMax = 0;
	int gMax = 0;
	int bMax = 0;

	std::vector <float> qM;
	std::vector <float> fM;

	qMax = 0;
	fMax = 0;

	//UPDATE HOST COPY IF VALUES HAVE BEEN 
	if (isCurrent == false) {
		copy_host();
		isCurrent = true;
	}

	///////////////////////////////////////////////////////////////
	/////////////////////// DEFINE MAPPING BEHAVIOR OF DISPLAY MODE
	for (int i = 0; i < n; i++){
		float u = abs(Q_h.at(i * DOF + 0));
		float v = abs(Q_h.at(i * DOF + 1));
		float w = abs(Q_h.at(i * DOF + 2));

		float x = abs(F_h.at(i * DOF + 0));
		float y = abs(F_h.at(i * DOF + 1));
		float z = abs(F_h.at(i * DOF + 2));

		ntVec3 dQ = ntVec3(u, v, w);
		ntVec3 dF = ntVec3(x, y, z);
		float q = dQ.mag();
		float f = dF.mag();

		qM.push_back(q);
		fM.push_back(f);

		if (qMax < q){
			qMax = q;
			nMax = i + 1; ///REVISE TO INCLUDE NODES WITH EQUAL DISPLACEMENT
		}

		if (fMax < f){ fMax = f; }
		if (rMax < u){ rMax = u; }
		if (gMax < v){ gMax = v; }
		if (bMax < w){ bMax = w; }
	}
	///////////////////////////////////////////////////////////////
	////////////////////////////////////////////////// UPDATE COLOR
	if (m == vW) {
		for (int i = 0; i < n; i++){
			float r = .9;
			float g = .9;
			float b = .9;
			nodes.at(i)->col = Col4(r, g, b);
		}
		for (int i = 0; i < e; i++){
			elements.at(i)->w = .5;
		}
	}

	if (m == vQ) {
		for (int i = 0; i < n; i++){
			float q = qM.at(i);
			//float r = mapRange(0.3, 1, 0, qMax, q);
			//float g = mapRange(.35, .45, 0, qMax, q, false);
			//float b = mapRange(  0,.55, 0, qMax, q, false);
			float r = mapRange(0.3, 1, 0, qMax, q);
			float g = mapRange(0, .6, 0, qMax, q, false);
			float b = mapRange(0.1, .85, 0, qMax, q, false);
			nodes.at(i)->col = Col4(r, g, b);
		}
		for (int i = 0; i < e; i++){
			elements.at(i)->w = .5;
		}
	}
	if (m == vF) {
		for (int i = 0; i < n; i++){
			float q = fM.at(i);
			float r = mapRange( 0, 1, 0, qMax / 10000, q);
			float g = 0;//mapRange(0, .65, 0, qMax / 100000, q / 1000);
			float b = mapRange(0.75, .95, 0, qMax / 100000, q, false);
			if (r > .9){ g = 0;}
			if (b > .85){ g = 0; }
			nodes.at(i)->col = Col4(r, g, b);
		}
		for (int i = 0; i < e; i++){
			elements.at(i)->w = .5;
		}
	}
	if (m == vS) {
		for (int i = 0; i<n; i++){
			nodes.at(i)->col = ntCol4(1, 0, 0);
		}
		for (int i = 0; i < e; i++){
			float s = abs(S_h[i]);
			float r = mapRange(0.1, 1, 0, sMax, s);
			float g = mapRange(0.1, 1, 0, sMax, s);
			float b = mapRange(0.1, 1, 0, sMax, s);
			float a = mapRange(1, 1, 0, sMax, s);
			//float r = mapRange(0.3, 1, 0, sMax, s);
			//float g = mapRange(.35, .45, 0, sMax, s, false);
			//float b = mapRange(0, .55, 0, sMax, s, false);
			float w = mapRange(1, 1.5, 0, sMax, s);
			elements.at(i)->colE = Col4(r, g, b, a);
			elements.at(i)->colS = Col4(r, g, b, a);
			elements.at(i)->w = w;
		}
	}
}
void ntTruss::display(){
	
	for (int i = 0; i<elements.size(); i++){
		if (m == vS) {
			elements.at(i)->display();
		} else {
			elements.at(i)->display_X();
		}
	}
	for (int i = 0; i<nodes.size(); i++){
		if (nodes.at(i)->is_Free() == false){
			nodes.at(i)->display(1);
		}
	}

}
