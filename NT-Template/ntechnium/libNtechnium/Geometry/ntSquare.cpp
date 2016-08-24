#include "ntSquare.h"

ntSquare::ntSquare() {}
ntSquare::ntSquare(float dim):
ntRectangle(dim , dim){
}
ntSquare::ntSquare(const ntVec3 & pos, float dim):
ntRectangle(pos, dim, dim) {
}

ntSquare::ntSquare(float dim, ntCol4 color):
ntRectangle(dim, dim, color) {
}

ntSquare::ntSquare(const ntVec3 & pos, float dim, ntCol4 color):
ntRectangle(pos, dim, dim, color){
}
