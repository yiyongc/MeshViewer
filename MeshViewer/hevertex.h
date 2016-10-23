#pragma once

#ifndef HE_VERTEX_INCLUDED
#define HE_VERTEX_INCLUDED

//Forward declaration
class HE_edge;

class HE_vert {
public:
	HE_vert(int id, float x1, float y1, float z1);
	~HE_vert();
		
	int getID();
	void setEdge(HE_edge& e1);
	HE_edge* getEdge();

private:
	int vertexID;
	float x, y, z;  // the vertex coordinates
	HE_edge* edge;  // one of the half-edges emanating from the vertex

};


#endif //HE_VERTEX_INCLUDED