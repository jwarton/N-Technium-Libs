#include "ntGraph.h"

ntGraph::ntGraph(){}

ntGraph::ntGraph(std::string data) {
	this->data = data;
	init();
}
ntGraph::ntGraph(float x, float y, float z) {
	this->dimX = x;
	this->dimY = y;
	this->dimZ = z;
	this->dim->x = x;
	this->dim->y = y;
	this->dim->y = z;
}
ntGraph::ntGraph(ntVec3* pos, ntVec3* dim, std::vector <float> vals):
pos(pos), dim(dim), vals(vals){
	this->dimX = dim->x;
	this->dimY = dim->y;
	this->dimZ = dim->z;
}
ntGraph::ntGraph(ntVec3* pos, ntVec3* dim, graph_A axis) :
	pos(pos), dim(dim), vals(vals), axis(axis) {
	this->dimX = dim->x;
	this->dimY = dim->y;
	this->dimZ = dim->z;
}
ntGraph::ntGraph(ntVec3* pos, ntVec3* dim, std::vector <float> vals, graph_A axis) :
	pos(pos), dim(dim), vals(vals), axis (axis){
	this->dimX = dim->x;
	this->dimY = dim->y;
	this->dimZ = dim->z;
}

void ntGraph::sort() {
	std::sort(vals.begin(), vals.end());
}
void ntGraph::init() {
	if (vals.size() > dimX) {
		std::vector <float> elem;
		int dim = ceil(vals.size() / dimX);
		for (int i = 0; i < vals.size(); i++) {
			if (i % dim == 0) {
				elem.push_back(vals.at(i));
			}
		}
		vals.swap(elem);
	}

	if(vals.size()>0){
		//parse_Data();
		plot_Data();
	}
	else {
		std::cout << "NO DATA PROVIDED FOR GRAPH TO PLOT" << endl;
	}
}

void ntGraph::parse_Data() {
	///SET PARAMETER VALUES FROM TXT INPUT
	///MUST BE FORMATED PROPERLY
}
void ntGraph::plot_Data() {
	//FIND MAXIUMUM PARAMETER VALUE
	auto it_0 = max_element(std::begin(vals), std::end(vals));
	//auto it_1 = max_element(std::begin(vals_1), std::end(vals_1));
	//auto it_2 = max_element(std::begin(vals_2), std::end(vals_2));
	//auto it_3 = max_element(std::begin(vals_3), std::end(vals_3));

	float max_0 = *it_0;
	//float max_1 = *it_1;
	//float max_2 = *it_2;
	//float max_3 = *it_3;
	float max_B = max_0;

	//if (max_1 >= max_2 && max_1 >= max_3) {
	//	max_B = max_1;
	//}
	//if (max_2 >= max_1 && max_2 >= max_3) {
	//	max_B = max_2;
	//}
	//if (max_3 >= max_1 && max_3 >= max_2) {
	//	max_B = max_3;
	//}
	//////////////////////////////////////////MAP PARAMETERS TO VERTEX COORDINATES

	float val;
	float step;
	float axis_Major;
	float axis_Minor;

	if (axis == X_axis) {
		axis_Major = dimX;
		axis_Minor = dimY;
	}
	if (axis == Y_axis) {
		axis_Major = dimY;
		axis_Minor = dimX;
	}

	for (int i = 0; i < vals.size(); i++) {
			val = vals.at(i);
			step = mapRange(0, axis_Major, 0, vals.size(), i);
			val =  mapRange(0, axis_Minor, 0, max_B, val);

		if (is_ColSet == false) {
			colT.r = mapRange( 0.4, 1.0, 0.0, vals.size(), i);
			colT.g = mapRange( 0.2, 0.7, 0.0, vals.size(), i);
			colT.b = mapRange( 0.3, 0.9, 0.0, vals.size(), i);
			colT.a = 1;
		}
		if (i % 2 == 0) {
			float x0, y0, z0;
			float x1, y1, z1;

			if (axis == X_axis) {
				x0 = pos->x + step;
				y0 = pos->y + 0;
				z0 = pos->z + 0;
				x1 = pos->x + step;
				y1 = pos->y + val;
				z1 = pos->z + 0;
			}
			if (axis == Y_axis) {
				x0 = pos->x + 0;
				y0 = pos->y + step;
				z0 = pos->z + 0;
				x1 = pos->x + val;
				y1 = pos->y + step;
				z1 = pos->z + 0;
			}

			Vec3 * vecS = new Vec3(x0, y0, z0);
			Vec3 * vecE = new Vec3(x1, y1, z1);
			ntEdge* bar = new ntEdge(vecE, vecS, Col4(colT.r, colT.g, colT.b, colT.a));
			bars.push_back(bar);
		}
	}

	for (int i = 0; i < vals_1.size(); i++) {
		val = vals_1.at(i);
		step = mapRange(0, axis_Major, 0, vals_1.size(), i);
		val = mapRange(0, axis_Minor, 0, max_B, val);

		if (is_ColSet == false) {
			colT.r = mapRange(0.85, 1.0, 0.0, vals_1.size(), i);
			colT.g = mapRange(0.575, 0.7, 0.0, vals_1.size(), i);
			colT.b = mapRange(0.75, 0.9, 0.0, vals_1.size(), i);
			colT.a = 1;
		}
		float x0, y0, z0;
		float x1, y1, z1;

		if (axis == X_axis) {
			x0 = pos->x + step;
			y0 = pos->y + 0;
			z0 = pos->z + 0;
			x1 = pos->x + step;
			y1 = pos->y + val;
			z1 = pos->z + 0;
		}
		if (axis == Y_axis) {
			x0 = pos->x + 0;
			y0 = pos->y + step;
			z0 = pos->z + 0;
			x1 = pos->x + val;
			y1 = pos->y + step;
			z1 = pos->z + 0;
		}

		Vec3 * vecS = new Vec3(x0, y0, z0);
		Vec3 * vecE = new Vec3(x1, y1, z1);
		ntEdge* bar = new ntEdge(vecE, vecS, Col4(colT.r, colT.g, colT.b, colT.a));
		bars.push_back(bar);
	}
	//for (int i = 0; i < vals_1.size(); i++) {
	//	y = vals_1.at(i);
	//	x = mapRange(0, dimX, 0, vals_1.size(), i);
	//	y = mapRange(0, dimY, 0, max_B, y);
	//	r = 1;
	//	g = 0;
	//	b = mapRange(.2, 1, 0, 200, i);
	//	a = 1;

	//	Vec3 * vecS = new Vec3(x, 0, 0);
	//	Vec3 * vecE = new Vec3(x, y, 0);
	//	ntEdge* bar = new ntEdge(vecE, vecS,Col4(r,g,b,a));
	//	bars.push_back(bar);
	//}
	///int count = vals_2.size();
	//if (i % 2 == 0)
	//for (int i = 0; i < count; i++) {
	//		y = vals_2.at(i);
	//		x = mapRange(0, dimX, 0, vals_2.size(), i) + (.5 * step);
	//		y = mapRange(0, dimY, 0, max_B, y);
	//		r = .1;
	//		g = 0;
	//		b = mapRange(0.5, 1, 0, 200, i);
	//		a = 1;

	//		Vec3 * vecS = new Vec3(x, 0, 0);
	//		Vec3 * vecE = new Vec3(x, y, 0);
	//		ntEdge* bar = new ntEdge(vecE, vecS, Col4(r, g, b, a));
	//		bars.push_back(bar);
	//}

	//for (int i = 0; i < vals_2.size(); i++) {
	//	y = vals_2.at(i);
	//	x = mapRange(0, dimX, 0, 1, i)+.01;
	//	y = mapRange(0, dimY, 0, max_B, y);

	//	Vec3 * vecS = new Vec3(x, 0, 0);
	//	Vec3 * vecE = new Vec3(x, y, 0);
	//	ntEdge* bar = new ntEdge(vecE, vecS);
	//	bars.push_back(bar);
	//}	
	//for (int i = 0; i < vals_3.size(); i++) {
	//	y = vals_3.at(i);
	//	x = mapRange(0, dimX, 0, 1, i)+.02;
	//	y = mapRange(0, dimY, 0, max_B, y);

	//	Vec3 * vecS = new Vec3(x, 0, 0);
	//	Vec3 * vecE = new Vec3(x, y, 0);
	//	ntEdge* bar = new ntEdge(vecE, vecS);
	//	bars.push_back(bar);
	//}
}
void ntGraph::test_Data(){
	for (int i = 0; i < 50; i++){
		float val_1 = rand()*10;
		float val_2 = rand() *i;
		float val_3 = rand() * 10;
		vals_1.push_back(val_1);
		vals_2.push_back(val_2);
		vals_3.push_back(val_3);
	}
}
void ntGraph::set_Data(std::string data) {

}
void ntGraph::set_Param(float val, int param){
	if (param == 0) {
		vals.push_back(val);
	} else if (param == 1) {
		vals_1.push_back(val);
	} else if (param == 2) {
		//vals_2.push_back(val);
	} else if (param == 3) {
		//vals_3.push_back(val);
	}
}
void ntGraph::set_Param(std::vector <float> vals, int param) {
	if (param == 1) {
		this->vals_1 = vals;
	}
	else if (param == 2) {
		this->vals_2 = vals;
	}
	else if (param == 3) {
		this->vals_3 = vals;
	}
	//std::cout << "from ntGraph.set_Param():  " << vals_1.at(0) << endl;
}
void ntGraph::set_Color(ntColor4f colT) {
	this->colT = colT;
	is_ColSet = true;
}
void ntGraph::set_Width(int w) {
	this->w = w;
}

void ntGraph::run() {

}

void ntGraph::display() {
	//bars.at(0)->display();
	if (mode == BAR) {
		for (int i = 0; i < bars.size(); i++) {
			bars.at(i)->display(w);
		}
	}
	if (mode == LINE) {
		for (int i = 0; i<lines.size(); i++) {
			lines.at(i)->display();
		}
	}
	//std::cout << bars.size() << endl;
	//for (int i = 0; i<lines.size(); i++) {
	//	//lines.at(i)->display(1);
	//}
}