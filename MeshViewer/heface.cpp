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