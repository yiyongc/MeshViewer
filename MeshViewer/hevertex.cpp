#include "hevertex.h"
//
HE_vert::HE_vert(int id, float x1, float y1, float z1) :
vertexID(id),
x(x1),
y(y1),
z(z1),
edge(nullptr)
{

}

HE_vert::~HE_vert() {
	delete edge;
}

int HE_vert::getID() {
	return vertexID;
}

void HE_vert::setEdge(HE_edge& e1) {
	edge = &e1;
}

HE_edge* HE_vert::getEdge() {
	return edge;
}

float HE_vert::getX() {
	return x;
}

float HE_vert::getY() {
	return y;
}

float HE_vert::getZ() {
	return z;
}

