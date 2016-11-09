#include "heface.h"

HE_face::HE_face()
{

}

HE_face::HE_face(HE_edge* e1) :
edge(e1)
{

}


HE_face::~HE_face() {
	delete edge;
}


HE_edge* HE_face::getEdge() {
	return edge;
}



QVector3D HE_face::computeNormal() {
	HE_vert* v1 = edge->getVert();
	HE_vert* v2 = edge->getNext()->getVert();
	HE_vert* v3 = edge->getPrev()->getVert();

	QVector3D vector1, vector2;

	vector1 = QVector3D(v2->getX() - v1->getX(), v2->getY() - v1->getY(), v2->getZ() - v1->getZ());
	vector2 = QVector3D(v3->getX() - v1->getX(), v3->getY() - v1->getY(), v3->getZ() - v1->getZ());

	return QVector3D::crossProduct(vector1, vector2);
}
