#pragma once

#include "Vertex.h"
#include "Vector.h"

class Polygon
{
public:
	Vertex *vertex[3];
	Vector normal;

	bool isIn(Vertex *v);

	void getOtherPoints(Vertex *vs[2], Vertex *v);

	Vector normal();
};

