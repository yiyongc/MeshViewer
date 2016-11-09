#pragma once

#ifndef HE_FACE_INCLUDED
#define HE_FACE_INCLUDED

#include "heedge.h"


class HE_face {

public: 
	HE_face();
	HE_face(HE_edge* e1);
	~HE_face();

	QVector3D computeNormal();
	HE_edge* getEdge();

private:
	HE_edge* edge;  // one of the half-edges bordering the face
};






#endif //HE_FACE_INCLUDED