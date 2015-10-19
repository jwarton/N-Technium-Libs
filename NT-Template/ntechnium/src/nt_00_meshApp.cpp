#include "nt_00_meshApp.h"

void meshImportApp::init() {
	//glRotatef( 90, 1.f, 0.f, 0.f);
	//glRotatef( -10, 0.f, 1.f, 0.f);
	glRotatef(-90, 0.f, 0.f, 1.f);
	///////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////// OBJECTS
	ntTextIO ptData00 = ntTextIO("pointPos_00.txt");
	ntTextIO ptData01 = ntTextIO("pointPos_01.txt");
	ntTextIO ptData02 = ntTextIO("pointPos_02.txt");
	ntTextIO ptData03 = ntTextIO("pointPos_03.txt");

	mesh00 = ntMeshPts(ptData00.getVecsV());
	mesh01 = ntMeshPts(ptData01.getVecsV());
	mesh02 = ntMeshPts(ptData02.getVecsV());
	mesh03 = ntMeshPts(ptData03.getVecsV());

	///////////////////////////////////////////////////////////////
	/////////////////////////////////////// OBJECTS COLOR BY VERTEX
	for (int i = 0; i < mesh00.verts.size(); i++){
		int val = mesh00.faces.size();
		float r = mapRange(.5,  1, 0, val, i, true);
		float g = mapRange(.25, .4, 0, val, i,false);
		float b = mapRange( 0, .3, 0, val, i, true);
		float a = mapRange(-.25, 1, 0, val, i, true);
		float s = mapRange(1, 3.5, 0, val, i, true);
		mesh00.verts.at(i)->setSize(s);
		mesh01.verts.at(i)->setSize(s);
		mesh02.verts.at(i)->setSize(s);
		mesh03.verts.at(i)->setSize(s);
		mesh00.verts.at(i)->setColor(ntColor4f(r, g, b, 1));
		mesh01.verts.at(i)->setColor(ntColor4f(r, g, b, 1));
		mesh02.verts.at(i)->setColor(ntColor4f(r, g, b, 1));
		mesh03.verts.at(i)->setColor(ntColor4f(r, g, b, 1));
	}
	
	for (int i = 0; i < mesh00.faces.size(); i++){
		int val = mesh00.faces.size();
		float n = mapRange(.01,.25, 0, val, i, false);
		//float w = mapRange(.01, 3, 0, val, i, false);
		float r = mapRange(.25, .5, 0, val, i, false);
		float g = mapRange(0, .25, 0, val, i, true);
		float b = mapRange(0, .8, 0, val, i, true);
		float a = mapRange(.5, 1, 0, val, i, false);
		mesh00.faces.at(i).normal.setLength(n);
		mesh01.faces.at(i).normal.setLength(n);
		mesh02.faces.at(i).normal.setLength(n);
		mesh03.faces.at(i).normal.setLength(n);
		mesh00.faces.at(i).normal.colS = ntColor4f(.55, .65, .50, a);
		mesh01.faces.at(i).normal.colS = ntColor4f(.55, .65, .50, a);
		mesh02.faces.at(i).normal.colS = ntColor4f(.55, .65, .50, a);
		mesh03.faces.at(i).normal.colS = ntColor4f(.55, .65, .50, a);
	}
}

void meshImportApp::run(){
	display();
}

void meshImportApp::display(){
	///////////////////////////////////////////////////////////////
	
	//mesh00.display();
	//mesh01.display();
	//mesh02.display();
	//mesh03.display();

	mesh00.displayVerts();
	mesh01.displayVerts();
	mesh02.displayVerts();
	mesh03.displayVerts();

	//mesh00.displayNorms();
	//mesh01.displayNorms();
	//mesh02.displayNorms();
	//mesh03.displayNorms();

	//mesh00.displayEdges(.1);
	//mesh01.displayEdges(.2);
	//mesh02.displayEdges(.2);
	//mesh03.displayEdges(.2);

	///////////////////////////////////////////////////////////////
}