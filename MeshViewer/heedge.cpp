#include "heedge.h"

HE_edge::HE_edge(HE_vert* end) : 
vert(end),
pair(nullptr),
face(nullptr),
prev(nullptr),
next(nullptr)
{
}

HE_edge::HE_edge(HE_vert* end, HE_face* f1, HE_edge* pr1, HE_edge* n1) :
vert(end),
pair(nullptr),
face(f1),
prev(pr1),
next(n1)
{
}



HE_edge::~HE_edge() {
	delete vert;
	delete pair;
	delete face;
	delete prev;
	delete next;
}

void HE_edge::setPair(HE_edge& edge) {
	pair = &edge;
}

void HE_edge::setFace(HE_face& f1)  {
	face = &f1;
}

void HE_edge::setPrev(HE_edge& edge)  {
	prev = &edge;
}

void HE_edge::setNext(HE_edge& edge)  {
	next = &edge;
}



HE_vert* HE_edge::getVert() {
	return vert;
}

HE_face* HE_edge::getFace() {
	return face;
}

HE_edge* HE_edge::getPair() {
	return pair;
}

HE_edge* HE_edge::getPrev() {
	return prev;
}

HE_edge* HE_edge::getNext() {
	return next;
}