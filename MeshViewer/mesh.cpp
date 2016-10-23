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

void Mesh::addVertex(HE_vert* v1){
	vertices.push_back(v1);
}

void Mesh::addFace(HE_face* f1){
	faces.push_back(f1);
}

void Mesh::addEdge(HE_edge* e1){
	edges.push_back(e1);
}