///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// ntTetra.h
// openGl scratch libs							///////////////////
// ntTetra.h extends ntBaseShape.h				///////////////////
// created by James Warton on 02/23/2014		///////////////////
///////////////////////////////////////////////////////////////////
#ifndef TETRAHEDRON_JPW_NTECHNIUM
#define TETRAHEDRON_JPW_NTECHNIUM
#define _USE_MATH_DEFINES

#include <iostream>
#include "ntBaseShape.h"

class ntTetra : public BaseShape {
    
public:
    ntTetra();
    ntTetra(const ntVec3& pos, const ntVec3&  rot, const ntVec3&  dim, const ntColor4f& fillCol, const ntColor4f& strokeCol, std::string name);
    
    void display();
    void displayNorms(float len = 3);

private:
    
    std::string name;
    void init();
};
#endif 