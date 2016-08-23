///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////// ntBaseGeom.h
// openGl scratch libs							///////////////////
// BaseGeom Class for all derived geometry		///////////////////
// created by James Warton on 05/29/2014		///////////////////
///////////////////////////////////////////////////////////////////

#ifndef BASE_GEOM_JPW_NTECHNIUM
#define BASE_GEOM_JPW_NTECHNIUM

#include <GL/glew.h>
#include <iostream>
#include <vector>

#include "ntVertex.h"
#include "ntTup3i.h"
#include "ntEdge.h"
#include "ntFace3.h"
#include "ntBaseShape.h"

class ntGeom: public BaseShape{
protected:
	std::string mapURL;
	//ntMap bumpMap;
	//ntMap diffuseMap;
	//ntMap reflectMap;
	//float mapScale;

	//virtual void init();
	//virtual void calcVerts() = 0;
	//virtual void calcInds() = 0;
	//virtual void calcFaces();
	//virtual void calcPrimitives();
	//virtual void addMap();

	//void fillDisplayLists();

	//
	//std::vector<ntVec3*>	vecs;
	//std::vector<ntVertex*>	verts;
	//std::vector<ntTup3i>	inds;
	//std::vector<ntEdge>		edges;
	//std::vector<ntFace3>	faces;

	//PRIMITIVES
	std::vector<float> vertPrims;
	std::vector<unsigned int> indPrims;
	std::vector<float> normPrims;
	std::vector<float> colorPrims;
	std::vector<float> mapPrims;
	std::vector<float> interleavedPrims;

	GLuint displayListIndex;
	GLuint vboID, indexVboID;

	float* sharedMemPointer;///this approach seems odd

	///std::vector<ntGeomSet> geomSets;
	///////////////////////////////////////////////////////////////
	////////////////////////////////////////////MATERIAL PROPERTIES
	GLfloat shininess[1];  //0-128
	GLfloat specularCol[4];
	GLfloat emissiveCol[4];

	///////////////////////////////////////////////////////////////
	///////////////////////////////////////// UPDATE BUFFER FOR GPU
	bool isMapEnabled;
	void updateBuffer();			//review ira's implementation
	void updateMapBuffer();			//review ira's implementation
	void updateColorBuffer();

public:
	ntGeom();
	//virtual ~ntGeom();

	//ADD TRANSFORM METHODS

	std::vector<ntFace3>* getFaces();
	std::vector<ntVertex>* getVertices();
	std::vector<float>* getInterleavedPrimitives();

	//virtual void sortFaces();

	GLuint getVboID() const;
	GLuint getIndexVboID() const;
	int getIndicesSize() const;

	////SET MATERIAL PARAMETERS
	void setShininess(float shininess = 45);
	void setSpecularCol(const Col4& specularCol = Col4(1, 1, 1, 1));
	void setEmissiveCol(const Col4& emissiveCol = Col4(0, 0, 0, 1));
	//diffuse
	//ambient

	static GLuint textureID;
	///ntMap getMap()const;
	void enableMaps();
	void disableMaps();

	void setMapScale(float scale);
	float getMapScale() const;

	///setBumpMap(ntMap map);
	///getBumpMap();

	//ADD STL EXPORT FUNCTION
};

#endif

