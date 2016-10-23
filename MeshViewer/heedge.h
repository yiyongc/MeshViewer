#pragma once

#ifndef HE_EDGE_INCLUDED
#define HE_EDGE_INCLUDED

#include "hevertex.h"

//Forward declaration
class HE_face;


class HE_edge {

public:
	HE_edge(HE_vert* end);
	HE_edge(HE_vert* end, HE_face* face, HE_edge* prev = nullptr, HE_edge* next = nullptr);
	~HE_edge();

	void setPair(HE_edge& edge);
	void setFace(HE_face& face);
	void setPrev(HE_edge& edge);
	void setNext(HE_edge& edge);

	HE_vert* getVert();
	HE_face* getFace();
	HE_edge* getPair();
	HE_edge* getPrev();
	HE_edge* getNext();

private:
	HE_vert* vert;  // vertex at the end of the half-edge
	HE_edge* pair;  // oppositely oriented half-edge
	HE_face* face;  // the incident face
	HE_edge* prev;  // previous half-edge around the face
	HE_edge* next;  // next half-edge around the face

};






#endif //HE_EDGE_INCLUDED