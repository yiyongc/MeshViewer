#ifndef MESH_H
#define MESH_H

#include "heface.h"
#include <vector>

class Mesh {
public:
	Mesh();
	~Mesh(); 

	std::vector<HE_face*> getFaces();
	std::vector<HE_vert*> getVertices();
	std::vector<HE_edge*> getEdges();
	std::vector<QVector3D> getNormals();
	
	void addVertex(HE_vert* v1);
	void addFace(HE_face* f1);
	void addEdge(HE_edge* e1);
	void computeVertexNormals();

private:
	std::vector<HE_face*> faces;
	std::vector<HE_vert*> vertices;
	std::vector<HE_edge*> edges;
	std::vector<QVector3D> vertNormals;

	

};


#endif //MESH_H