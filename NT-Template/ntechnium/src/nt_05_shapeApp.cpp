#include "nt_05_shapeApp.h"

void shapeApp::init() {
	///////////////////////////////////////////////////////////////
	////////////////////////////////////////////// OBJECT POSITIONS
	std::vector <Vec3*> grid;
	int cols = 4;
	int rows = 4;
	int x_sp = 10;
	int y_sp = 10;
	int z_sp = 10;

	for (int i = 0; i < cols; i++) {
		for (int j = 0; j < rows; j++) {
			int x = (i * x_sp) - (x_sp * cols * 0.5);
			int y = (j * y_sp) - (y_sp * rows * 0.5);
			int z = 0;
			ntVec3 * pt = new ntVec3(x, y, z);
			grid.push_back(pt);
		}
	}
	///////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////// OBJECTS

	float radius = x_sp * 0.45;
	float dim0 = x_sp * 0.9;
	float dim1 = y_sp * 0.5;

	triangle =	ntPolygon(grid[0], radius, 3);
	square =	ntSquare(grid[1], dim0);
	rectangle = ntRectangle(grid[2], dim0, dim1);
	circle =	ntCircle(grid[3], radius);

	tetra_00 = ntTetra(grid[4], radius);
	tetra_01 = ntTetra(grid[5], radius);
	tetra_02 = ntTetra(grid[6], radius);
	tetra_03 = ntTetra(grid[7], radius);

	tetra_00.set_edge_color(Col4f(1.0, 1.0, 1.0, 0.2));
	tetra_01.set_edge_color(Col4f(1.0, 0.0, 0.0, 0.5));

	cube_00 = ntCube(grid[8], dim0);
	cube_01 = ntCube(grid[9], dim0);
	cube_02 = ntCube(grid[10], dim0);
	cube_03 = ntCube(grid[11], dim0);

	cube_00.set_edge_color(Col4f(1.0, 1.0, 1.0, 0.2));
	cube_01.set_edge_color(Col4f(1.0, 0.0, 0.0, 0.5));

	sphere_00 = ntSphere(grid[12], radius, 50, 50);
	sphere_01 = ntSphere(grid[13], radius, 50, 50);
	sphere_02 = ntSphere(grid[14], radius, 35, 35);
	sphere_03 = ntSphere(grid[15], radius, 25, 25);

	///////////////////////////////////////////////////////////////
	/////////////////////////////////////// OBJECTS COLOR BY VERTEX
	/// CUBE COLOR MAPPING-- VERTEX PTR WITHIN FACE CONSTRUCTOR NEEDS WORK
	int cnt_cv = cube_03.verts.size();
	for (int i = 0; i < cnt_cv; i++) {
		float r = mapRange(0.0, 1.0, 0, cnt_cv, i);
		float g = mapRange(0.5, 1.0, 0, cnt_cv, i);
		float b = mapRange(0.0, 1.0, 0, cnt_cv, i);
		float a = mapRange(0.0, 1.0, 0, cnt_cv, i);
		cube_03.verts.at(i)->set_color(Col4f(r, g, b, a));
	}
	/// SPHERE COLOR MAPPING
	int cnt_verts = sphere_03.verts.size();
	for (int i = 0; i < cnt_verts; i++) {
		float r = mapRange(0.0, 1.0, 0, cnt_verts, i); 
		float g = mapRange(0.5, 1.0, 0, cnt_verts, i);
		float b = mapRange(0.0, 1.0, 0, cnt_verts, i);
		float a = mapRange(0.0, 1.0, 0, cnt_verts, i);
		sphere_03.verts.at(i)->set_color(Col4f(r, g, b, a));
	}

	int val = sphere_00.verts.size();
	for (int i = 0; i<val; i++){
		float r = mapRange( 0.0, 1.0, 0, val, i);
		float g = 0;
		float b = mapRange(0.25, 0.5, 0, val, i,false);
		float a = mapRange( 0.0, 1.0, 0, val, i);
		sphere_00.verts.at(i)->set_color(Col4f(r, g, b, a));
	}
}

void shapeApp::run(){
	display();
}

void shapeApp::display(){
	///////////////////////////////////////////////////////////////
	//
	triangle.display();
	triangle.display_verts();
	square.display();
	//square.display_verts();
	rectangle.display();
	//rectangle.display_verts();
	circle.display();
	//circle.display_verts();

	cube_00.display_verts();
	cube_00.display_edges(0.5);
	cube_01.display_norms(1);
	cube_01.display_edges();
	cube_02.display_edges();
	cube_03.display();

	tetra_00.display_verts();
	tetra_00.display_edges(0.5);
	tetra_01.display_norms(1);
	tetra_01.display_edges();
	tetra_02.display_edges();
	tetra_03.display();

	sphere_00.display_verts();
	sphere_01.display_norms(.1);
	sphere_02.display_edges(0.5);
	sphere_03.display();
	///////////////////////////////////////////////////////////////
}