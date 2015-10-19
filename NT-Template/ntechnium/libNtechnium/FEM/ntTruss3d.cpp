
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
	using namespace arma;

	elementCnt = elements.size();
	nodeCnt = nodes.size();
	for (int i = 0; i < nodeCnt; i++){
		rows += nodes.at(i)->get_DOF();
		DOF = nodes.at(i)->get_DOF();
	}
	cols = rows;

	F = zeros<mat>(rows, 1);
	Q = zeros<mat>(rows, 1);
	K = zeros<mat>(rows, cols);
	Kb = zeros<mat>(rows, cols);
	P = zeros<mat>(elementCnt, 2);
	S = zeros<mat>(elementCnt, 1);

	def_FG();
	def_KG();
	def_BC();

	//eval_GPUcs();
	eval_GPU();
	eval_CPU();


	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////// REMOVE BC ROUNDING ERRORS
	for (int i = 0; i < rows; i++){
		if (F(i, 0) < .00000001 && F(i, 0) > -.00000001){
			F(i, 0) = abs(round(F(i, 0)));
		}
		if (Q(i, 0) < .00000001 && Q(i, 0) > -.00000001){
			Q(i, 0) = abs(round(Q(i, 0)));
		}
	}
	///////////////////////////////////////////////////////////////////////////
	////////////////////////////////////// UPDATE NODE WITH DISPLACEMENT VALUES
	for (int i = 0; i < nodeCnt; i++){
		DOF = nodes.at(i)->get_DOF();
		int u = i * DOF;
		int v = i * DOF + 1;
		int w = i * DOF + 2;
		nodes.at(i)->Q.x = Q(u, 0);
		nodes.at(i)->Q.y = Q(v, 0);
		nodes.at(i)->Q.z = Q(w, 0);
	}
	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////// DEFINE AXIAL FORCE MATRIX
	for (int i = 0; i < elementCnt; i++){
		arma::mat q = zeros<mat>( 6, 6);
		float f0 = elements.at(i)->get_p().at(0,0);
		float f1 = elements.at(i)->get_p().at(1,0);
		P(i, 0) = f0;
		P(i, 1) = f1;
	}
	///////////////////////////////// DEFINE ELEMENT STRESS MATRIX
	sMax = 0;
	sMin = 0;
	for (int i = 0; i < elementCnt; i++){
		float a = elements.at(i)->get_A();
		S(i, 0) = P(i, 1) / a;
		if (sMax < S(i, 0)){
			sMax = S(i, 0);
			eMax = i;
		}
		if (sMin > S(i, 0)){
			sMin = S(i, 0);
			eMin = i;
		}
	}
}
void ntTruss::eval(){
	///CALL FROM OPTIMIZATION ROUTINE
}

void ntTruss::eval_GPUcs() {
	////////////////////////////////////////////////////////////////
	//t_copy = clock();
	///af::timer start1 = af::timer::start();
	float *Q_vals;
	Q_vals = new float[Q.size()];
	float *F_vals;
	F_vals = new float[F.size()];
	float *K_vals;
	K_vals = new float[K.size()];
	float *Kb_vals;
	Kb_vals = new float[Kb.size()];

	////////////////////////////////////////////// Q
	for (int i = 0; i < Q.size(); i++) {
		Q_vals[i] = Q(i);
	}
	af::array Q_AF(rows, 1, Q_vals);
	////////////////////////////////////////////// F
	for (int i = 0; i < F.size(); i++) {
		F_vals[i] = F(i);
	}
	af::array F_AF(rows, 1, F_vals);
	////////////////////////////////////////////// K
	for (int i = 0; i < K.size(); i++) {
		K_vals[i] = K(i);
	}
	af::array K_AF(rows, cols, K_vals);
	////////////////////////////////////////////// Kb
	for (int i = 0; i < Kb.size(); i++) {
		Kb_vals[i] = Kb(i);
	}
	af::array Kb_AF(rows, cols, Kb_vals);

	//size_t bytes;
	//size_t buffers;
	//size_t lock_bytes;
	//size_t lock_buffers;

	//af::deviceMemInfo(&bytes, &buffers, &lock_bytes, &lock_buffers);
	//print("\n% bytes\n",bytes);
	//print("\n% buffers\n", buffers);
	//print("\n% l_bytes\n", lock_bytes);
	//print("\n% l_buffers\n", lock_buffers);

	//t_copy = clock() - t_copy;
	///printf("copy time: %g\n", af::timer::stop(start1));

	////////////////////////////////////////////////////////////////
	///af::timer start2 = af::timer::start();
	t_GPU_cs = clock();
	//////////////////////////////////////////////////////////////////////////////////
	Q_AF = af::solve(Kb_AF, F_AF);					//dense system of linear equations
	af::matmul(K_AF, Q_AF);							//F = K * Q;
	//////////////////////////////////////////////////////////////////////////////////
	t_GPU_cs = clock() - t_GPU_cs;
	///printf("gpu time: %g\n", af::timer::stop(start2));
}
void ntTruss::eval_GPUsp() {

}
void ntTruss::eval_GPU() {
	////////////////////////////////////////////////////////////////
	t_copy = clock();
	af::array Q_AF(rows, 1, Q.memptr());
	af::array F_AF(rows, 1, F.memptr());
	af::array K_AF(rows, cols, K.memptr());
	af::array Kb_AF(rows, cols, Kb.memptr());

	t_copy = clock() - t_copy;
	//////////////////////////////////////////// SOLVE FOR DISPLACEMENT AND REACTIIONS
	t_GPU = clock();
	//////////////////////////////////////////////////////////////////////////////////
	Q_AF = af::solve(Kb_AF, F_AF);					//dense system of linear equations
	F_AF = af::matmul(K_AF, Q_AF);					//F = K * Q;
	//////////////////////////////////////////////////////////////////////////////////
	t_GPU = clock() - t_GPU;

	///////////////////////////////////////////////////////////// UPDATE HOST MATRICES
	Q_AF.host((void*)Q.memptr());
	F_AF.host((void*)F.memptr());

	iterations_GPU.push_back((float)t_GPU);
	iterations_GPU.push_back((float)t_GPU);
	iterations_GPU.push_back((float)t_GPU);
	iterations_copy.push_back((float)t_copy);
	iterations_copy.push_back((float)t_copy);
	iterations_copy.push_back((float)t_copy);
}
void ntTruss::eval_CPU() {
	t_CPU = clock();			//START TIMER
	using namespace arma;
	Q = solve(Kb, F);
	F = K * Q;
	t_CPU = clock() - t_CPU;	//END TIMER
	iterations_CPU.push_back((float)t_CPU);
	iterations_CPU.push_back((float)t_CPU);
	iterations_CPU.push_back((float)t_CPU);
	iterations_CPU.push_back((float)t_CPU);
}

void ntTruss::def_KG(){
	for (int i = 0; i < elementCnt; i++){
		//ELEMENT STIFFNESS MATRIX(FIG. 2.52)
		arma::mat ke = elements.at(i)->get_k();
		
		int row_ID;
		int col_ID;
		int r_dof = 0;
		int c_dof = 0;

		for (int j = 0; j < ke.n_rows; j++){
			if (r_dof >= DOF){
				r_dof = 0;
			}
			for (int k = 0; k < ke.n_cols; k++){

				float val = ke(j, k);	//contributing element stiffness 

				if (c_dof >= DOF){ 
					c_dof = 0;
				}

				if (j < DOF & k < DOF){
					row_ID = (DOF * elements.at(i)->get_nodeInd()[0]) + r_dof;
					col_ID = (DOF * elements.at(i)->get_nodeInd()[0]) + c_dof;
				} else if (j < DOF & k >= DOF){
					row_ID = (DOF * elements.at(i)->get_nodeInd()[1]) + r_dof;
					col_ID = (DOF * elements.at(i)->get_nodeInd()[0]) + c_dof;
				} else if (j >= DOF & k < DOF){
					row_ID = (DOF * elements.at(i)->get_nodeInd()[0]) + r_dof;
					col_ID = (DOF * elements.at(i)->get_nodeInd()[1]) + c_dof;
				} else if (j >= DOF & k >= DOF){
					row_ID = (DOF * elements.at(i)->get_nodeInd()[1]) + r_dof;
					col_ID = (DOF * elements.at(i)->get_nodeInd()[1]) + c_dof;
				}
				K(row_ID, col_ID) += ke(j, k);
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
		F(n    , 0) += f.x;
		F(n + 1, 0) += f.y;
		F(n + 2, 0) += f.z;
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

void ntTruss::write_txt(std::string s){
	timeStamp = time(0);

	localtime_s(&timeData, &timeStamp);
	string Y = to_string(timeData.tm_year + 1900);
	string M = to_string(timeData.tm_mon + 1);
	string D = to_string(timeData.tm_mday);
	string Hr = to_string(timeData.tm_hour);
	string Mn = to_string(timeData.tm_min);
	string Sc = to_string(timeData.tm_sec);
	string date = "DATE: " + Y + ":" + M + ":" + D + " | " + Hr + ":" + Mn + ":" + Sc + "\n";

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

	file << s << "\n";
	file << "GENERATED WITH NT LIBS\n";
	file << "AUTHORED BY JAMES WARTON\n" + date;
	file << "//////////////////////////////////////////////////////////////\n";
	file << "/////////////////////////////////////// EVALUATION PERFORMANCE\n\n";

	//file << "CALCULATION TIME  [CLICKS]:   " << t_CPU << "\n";
	file << "EVAL_CPU TIME  [SECONDS]:  " << ((float)t_CPU) / CLOCKS_PER_SEC << "\n";
	file << "EVAL_GPU1 TIME  [SECONDS]:  " << ((float)t_GPU) / CLOCKS_PER_SEC << "\n";
	file << "EVAL_GPU2 TIME  [SECONDS]:  " << ((float)t_GPU_cs) / CLOCKS_PER_SEC << "\n";
	file << "COPY TO GPU TIME  [SECONDS]:  " << ((float)t_copy) / CLOCKS_PER_SEC << "\n";
	//file << "EVAL ON GPU SPARSE[SECONDS]:  " << ((float)t_GPU_sp) / CLOCKS_PER_SEC << "\n";
	//file << "COPY TO GPU SPARSE[SECONDS]:  " << ((float)t_copy_sp) / CLOCKS_PER_SEC << "\n";
	file << "K MATRIX  [ELEMENTS]: " << K.size() << "\n";
	file << "K MATRIX  [ROWS]:     " << K.n_rows << "\n";
	file << "K MATRIX  [COLUMNS]:  " << K.n_cols << "\n";
	file << "Q MATRIX  [ELEMENTS]: " << Q.size() << "\n";
	file << "F MATRIX  [ELEMENTS]: " << F.size() << "\n\n";

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
		if (Q(i, 0) >= 0){ sp = "  "; }
		if (Q(i, 0) < 0){ sp = " "; }

		if (node < 10){
			file << "Q   " << node << "_" << axis << sp <<Q(i, 0) << endl;
		}
		else if (node < 100){
			file << "Q  " << node << "_" << axis << sp << Q(i, 0) << endl;
		}
		else {
			file << "Q " << node << "_" << axis << sp << Q(i, 0) << endl;
		}
		val += 1;
	}
	file << "\n//////////////////////////////////////////////////////////////\n";
	file << "///////////////////////////////////////////// ELEMENT STRESSES\n";
	for (int i = 0; i < n; i++){

		if (i < 9){
			if (S(i, 0) >= 0){ sp = ":   "; }
			if (S(i, 0) < 0){ sp = ":  "; }
			file << "S     " << i + 1 << sp << S(i, 0) << endl;
		}
		else if (i < 99){
			if (S(i, 0) >= 0){ sp = ":   "; }
			if (S(i, 0) < 0){ sp = ":  "; }
			file << "S    " << i + 1 << sp << S(i, 0) << endl;
		}
		else {
			if (S(i, 0) >= 0){ sp = ":   "; }
			if (S(i, 0) < 0){ sp = ":  "; }
			file << "S   " << i + 1 << sp << S(i, 0) << endl;
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
		if (F(i, 0) >= 0){ sp = "  "; }
		if (F(i, 0) < 0){ sp = " "; }

		if (node < 10){
			file << "F   " << node << "_" << axis << sp << F(i, 0) << endl;
		}
		else if (node < 100){
			file << "F  " << node << "_" << axis << sp << F(i, 0) << endl;
		}
		else {
			file << "F " << node << "_" << axis << sp << F(i, 0) << endl;
		}
		val += 1;
	}
	file << "\n//////////////////////////////////////////////////////////////\n";
	file << "///////////////////////////////////////////////// AXIAL FORCES\n";
	for (int i = 0; i < n ; i++){

		if (i < 9){
			if (P(i, 0) >= 0){ sp = "  "; }
			if (P(i, 0) < 0){ sp = " "; }
			file << "P   " << i + 1 << "_n0:" << sp << P(i, 0) << endl;
			if (P(i, 1) >= 0){ sp = "  "; }
			if (P(i, 1) < 0){ sp = " "; }
			file << "P   " << i + 1 << "_n1:" << sp << P(i, 1) << endl;
		}
		else if (i < 99){
			if (P(i, 0) >= 0){ sp = "  "; }
			if (P(i, 0) < 0){ sp = " "; }
			file << "P  " << i + 1 << "_n0:" << sp << P(i, 0) << endl;
			if (P(i, 1) >= 0){ sp = "  "; }
			if (P(i, 1) < 0){ sp = " "; }
			file << "P  " << i+1 << "_n1:" << sp << P(i, 1) << endl;
		}
		else {
			if (P(i, 0) >= 0){ sp = "  "; }
			if (P(i, 0) < 0){ sp = " "; }
			file << "P " << i+1 << "_n0:" << sp << P(i, 0) << endl;
			if (P(i, 1) >= 0){ sp = "  "; }
			if (P(i, 1) < 0){ sp = " "; }
			file << "P " << i + 1 << "_n1:" << sp << P(i, 1) << endl;
		}
	}
	file.close();
}
vector <float> ntTruss::get_t_CPU() {
	return iterations_CPU;
}
vector <float> ntTruss::get_t_GPU() {
	return iterations_GPU;
}
vector <float> ntTruss::get_t_copy() {
	return iterations_copy;
}

int ntTruss::get_dim_Q(){
	return Q.size();
}
int ntTruss::get_dim_F() {
	return F.size();
}
int ntTruss::get_dim_K() {
	return K.size();
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

	arma::mat qM = arma::zeros<arma::mat>(n, 1);
	arma::mat fM = arma::zeros<arma::mat>(n, 1);

	qMax = 0;
	fMax = 0;

	///////////////////////////////////////////////////////////////
	/////////////////////// DEFINE MAPPING BEHAVIOR OF DISPLAY MODE
	for (int i = 0; i < n; i++){
		float u = std::abs(Q.at(i * DOF + 0));
		float v = std::abs(Q.at(i * DOF + 1));
		float w = std::abs(Q.at(i * DOF + 2));

		float x = std::abs(F.at(i * DOF + 0));
		float y = std::abs(F.at(i * DOF + 1));
		float z = std::abs(F.at(i * DOF + 2));

		ntVec3 dQ = ntVec3(u, v, w);
		ntVec3 dF = ntVec3(x, y, z);
		float q = dQ.mag();
		float f = dF.mag();

		qM.at(i) = q;
		fM.at(i) = f;

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
			float s = abs(S.at(i));
			float r = mapRange(0.1, 1, 0, sMax, s);
			float g = mapRange(0.1, 1, 0, sMax, s);
			float b = mapRange(0.1, 1, 0, sMax, s);
			float a = mapRange(1, 1, 0, sMax, s);
			//float r = mapRange(0.3, 1, 0, sMax, s);
			//float g = mapRange(.35, .45, 0, sMax, s, false);
			//float b = mapRange(0, .55, 0, sMax, s, false);
			float w = mapRange(.5, 1.5, 0, sMax, s);
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