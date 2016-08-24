#include "ntSquare.h"

ntSquare::ntSquare() {}
ntSquare::ntSquare(float dim):
ntRectangle(dim , dim){
	//init();
}
ntSquare::ntSquare(const ntVec3 & pos, float dim):
ntRectangle(pos, dim, dim) {

}
