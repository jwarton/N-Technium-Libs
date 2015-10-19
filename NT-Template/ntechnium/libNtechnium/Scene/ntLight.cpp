#include "ntLight.h"
///////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////// DEFAULT
ntLight::ntLight()
{
}
///////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////// AMBIENT
ntLight::ntLight(const ntColor4f& ambient) :
pos(ntVec3()), diffuse(ntColor4f()), ambient(ambient), specular(ntColor4f()){
		init();
}
///////////////////////////////////////////////////////////////
////////////////////////////////////////////////// DFS LIGHTING
ntLight::ntLight(const ntVec3& pos, const ntColor4f& diffuse, const ntColor4f& ambient, const ntColor4f& specular):
	pos(pos),diffuse(diffuse),ambient(ambient),specular(specular){
	init();
}

void ntLight::init(){
	isOn = false;
}


//////////////////////////////////////////////////// DESTRUCTOR
ntLight::~ntLight(){}