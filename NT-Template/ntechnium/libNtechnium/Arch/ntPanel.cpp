#include "ntPanel.h"

ntPanel::ntPanel(){}

ntPanel::ntPanel(ntVec3* v0,ntVec3* v1,ntVec3* v2):
v0(v0),v1(v1),v2(v2){
	this->vecs[0] = v0;
	this->vecs[1] = v1;
	this->vecs[2] = v2;
	this->col = ntColor4f(1,1,1,1);
	this->col_0 = ntColor4f(1, 0, 0, 1);
	init();
}

void ntPanel::init(){
	//initialize vertex
	vert0 = new ntVertex (v0,col_0);
	vert1 = new ntVertex (v1,col);
	vert2 = new ntVertex (v2,col);
	verts.push_back(vert0);
	verts.push_back(vert1);
	verts.push_back(vert2);
	//initialize edges
	edges.push_back(ntEdge(v0,v1, col_0));
	edges.push_back(ntEdge(v1,v2,col));
	edges.push_back(ntEdge(v2,v0,col));
	//initialize centroid and normal
	calcCentroid();
	calcNorm();
	set_vG();
}
void ntPanel::calcCentroid(){
	cent = new ntVec3();
	cent->x = (v1->x + v2->x + v0->x)/3;
    cent->y = (v1->y + v2->y + v0->y)/3;
    cent->z = (v1->z + v2->z + v0->z)/3;
	centroid = ntVertex(cent);
	centroid.setSize(2);
	centroid.setColor(ntColor4f(1,1,1,0));
}
void ntPanel::calcNorm(){
	ntVec3* t1 = new ntVec3();
    t1->set(v2);
    t1->sub(v1);

    ntVec3* t2 = new ntVec3();
    t2->set(v0);
    t2->sub(v1);

    t1->unitize();
    t2->unitize();
	
	norm = t1->cross(t2);
	norm.unitize();
	//norm.invert();
	normal = ntNormal(*cent,norm,.05);
}

void ntPanel::setColor(ntColor4f col){
	this->col=col;
	for(int i = 0; i<verts.size(); i++){
		verts.at(i)->setColor(col);
	}
}
/////////////////////////////////////////////////PANEL FUNCTION NOT IN FACE CLASS
void ntPanel::set_ID(string panel_ID){
	this->panel_ID = panel_ID;
}
void ntPanel::set_nG(string n_G) {
	this->n_G = n_G;
}
void ntPanel::set_vG() {
	ntVec3* v_G0 = new ntVec3(v0->x, v0->y, v0->z);
	ntVec3* v_G1 = new ntVec3(v1->x, v1->y, v1->z);
	ntVec3* v_G2 = new ntVec3(v2->x, v2->y, v2->z);
	v_G.push_back(v_G0);
	v_G.push_back(v_G1);
	v_G.push_back(v_G2);
};
void ntPanel::set_pG(string p_G) {
	this->p_G = p_G;
};
void ntPanel::set_IMG(float val) {
	image_Val = val;
}
string ntPanel::get_ID() {
	return panel_ID;
}
string ntPanel::get_n_G() {
	return n_G;
}
string ntPanel::get_p_G() {
	return p_G;
}
std::vector<ntVec3*> ntPanel::get_v_G() {
	return v_G;
}
void ntPanel::display_Perf() {
	int n_seg = 36;

	float r = mapRange(r_Min, r_Max, 0, 1, image_Val);
	//r = r_Max;
	r = r * 0.015;///temporary scale solution
	

	for (int i = 0; i < p_Pos.size(); i++) {
		glBegin(GL_LINE_LOOP);
		//glBegin(GL_POINTS);
		//glPointSize(1);
		glColor4f(.25, .25, .25, 1);
		for (int j = 0; j<n_seg; ++j) {

			float cx = p_Pos.at(i).x;
			float cy = p_Pos.at(i).y;

			float theta = 2.0f * 3.1415926f * j / n_seg;//get the current angle
			float x = r * cosf(theta);//calculate the x component
			float y = r * sinf(theta);//calculate the y component

			glVertex2f(x + cx, y + cy);//output vertex
		}
		glEnd();
	}
}
void ntPanel::calc_PerfPos() {

	float w = (v1->x - v0->x) - (edge_Offset * 2);
	float sp = (w / p_Div);
	float y;
	float x;

	for (int i = 0; i < p_Div; i++) {
		for (int j = 0; j < p_Div; j++) {
			if (j % 2 == 0) {
				x = v2->x - (sp * p_Div*0.5) + (sp * i) + (sp/2);//(sp * i) + edge_Offset + r_Max;//cent->x - (sp * p_Div*0.5) + (sp * i); 
			}
			else {
				x = v2->x - (sp * p_Div*0.5) + (sp * i);//(sp * i) + edge_Offset + r_Max;//cent->x - (sp * p_Div*0.5) + (sp * i); 
			}
			y = (sp * j) + edge_Offset + r_Max;//cent->y - (sp * p_Div*0.5) + (sp * j); 

			if (pt_isInside(Vec3(x, y, 0))) {
				p_Pos.push_back(Vec3(x, y, 0));
			}
		}
	}
}

bool ntPanel::pt_isInside(Vec3 point) {

	int i, j, nvert = verts.size();
	bool c = false;

	for (i = 0, j = nvert - 1; i < nvert; j = i++) {
		if (((vecs[i]->y >= point.y) != (vecs[j]->y >= point.y)) &&
			(point.x <= (vecs[j]->x - vecs[i]->x) * (point.y - vecs[i]->y) / (vecs[j]->y - vecs[i]->y) + vecs[i]->x)
			)
			c = !c;
	}

	return c;
}

void ntPanel::display(){
	display_Perf();
	//glBegin(GL_TRIANGLES);
	//glColor4f(vert0->col.r, vert0->col.g, vert0->col.b, vert0->col.a);
	//glVertex3f(v0->x, v0->y, v0->z);
	//glColor4f(vert1->col.r, vert1->col.g, vert1->col.b, vert1->col.a);
	//glVertex3f(v1->x, v1->y, v1->z);	
	//glColor4f(vert2->col.r, vert2->col.g, vert2->col.b, vert2->col.a);
	//glVertex3f(v2->x, v2->y, v2->z);
	//glEnd();
	///TODO centroid functionality needs work
	//centroid.display();
}
