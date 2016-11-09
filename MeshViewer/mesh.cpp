#include "mesh.h"


Mesh::Mesh() {

}

Mesh::~Mesh() {

}


std::vector<HE_face*> Mesh::getFaces(){
	return faces;
}

std::vector<HE_vert*> Mesh::getVertices(){
	return vertices;
}

std::vector<HE_edge*> Mesh::getEdges(){
	return edges;
}

std::vector<QVector3D> Mesh::getNormals() {
	return vertNormals;
}

void Mesh::addVertex(HE_vert* v1){
	vertices.push_back(v1);
}

void Mesh::addFace(HE_face* f1){
	faces.push_back(f1);
}

void Mesh::addEdge(HE_edge* e1){
	edges.push_back(e1);
}

void Mesh::computeVertexNormals() {
	//For each vertex, find the faces associated with the vertex and 
	//compute the normal before storing it into the vector vertexNormals
	HE_vert* currVert;
	HE_edge* currEdge, *startEdge;

	for (int i = 0; i < vertices.size(); i++) {
		currVert = vertices.at(i);
		currEdge = currVert->getEdge();
		startEdge = currVert->getEdge();
		QVector3D vertNormal = QVector3D();
		int count = 0;

		do {
			vertNormal += currEdge->getFace()->computeNormal();
			count++;
			if (currEdge->getVert() == currVert)
				currEdge = currEdge->getNext();
			else {
				if (currEdge->getPair() != NULL)
					currEdge = currEdge->getPair();
				else
					break;
			}
		} while (currEdge != startEdge);

		vertNormal /= count;
		vertNormal.normalize();

		vertNormals.push_back(vertNormal / count);
	}
}