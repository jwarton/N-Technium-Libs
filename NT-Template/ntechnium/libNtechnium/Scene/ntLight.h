///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////// nt_BaseApp.h
// openGl scratch libs							///////////////////
// BaseApp Class for all derived applications	///////////////////
// created by James Warton on 03/20/2014		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef LIGHT_JPW_NTECHNIUM
#define LIGHT_JPW_NTECHNIUM

#include <iostream>
#include <vector>
#include "ntVec3.h"
#include "ntColor4f.h"

using namespace jpw;

class ntLight{
private:
	ntVec3 pos;
	ntColor4f diffuse, ambient, specular;

	bool isOn;
	void init();

public:
	ntLight();
	ntLight(const ntColor4f& ambient);
	ntLight(const ntVec3& pos, const ntColor4f& diffuse);
	ntLight(const ntVec3& pos, const ntColor4f& diffuse, const ntColor4f& ambient, const ntColor4f& specular);
	virtual ~ntLight();

	enum ligthPosMode{
		DIRECTIONAL_INFINITE,
		POSITIONAL
	};

	///////////////////////////////////////////////////CONSTRUCTORS
	void setPos(const ntVec3& pos);
	const ntVec3& getPos() const;

	void setDiffuse(const ntColor4f& diffuse);
	const ntColor4f& getDiffuse() const;

	void setAmbient(const ntColor4f& ambient);
	const ntColor4f& getAmbient() const;

	void setSpecular(const ntColor4f& specular);
	const ntColor4f& getSpecular() const;

	///////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////// USER CONTROLS
	void on();
	void off();
	bool getIsOn() const;

};
//position
inline void ntLight::setPos(const ntVec3& pos){
	this->pos = pos;
}
inline const ntVec3& ntLight::getPos() const{
	return pos;
}
//diffuse
inline void ntLight::setDiffuse(const ntColor4f& diffuse){
	this->diffuse = diffuse;
}
inline const ntColor4f& ntLight::getDiffuse() const{
	return diffuse;
}
//ambient
inline void ntLight::setAmbient(const ntColor4f& ambient){
	this->ambient = ambient;
}
inline const ntColor4f& ntLight::getAmbient() const{
	return ambient;
}
//specular
inline void ntLight::setSpecular(const ntColor4f& specular){
	this->specular = specular;
}
inline const ntColor4f& ntLight::getSpecular() const{
	return specular;
}

inline void ntLight::on(){
	isOn = true;
}
inline void ntLight::off(){
	isOn = false;
}
inline bool ntLight::getIsOn() const{
	return isOn;

}
#endif