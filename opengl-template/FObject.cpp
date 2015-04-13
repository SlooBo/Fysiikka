#include "FObject.h"

FObject::FObject()
{
	Vertex v1(1, 5, 3);
	Vertex v2(2, 4.5, 3);
	Vertex v3(3, 5, 3);
	Vertex v4(2, 3, 2);	

	addVertex(v1, Vector(0, 0, 0));
	addVertex(v2, Vector(0, 0, 0));
	addVertex(v3, Vector(0, 0, 0));
	addVertex(v4, Vector(0, 0, 0));

	addFace(2, 1, 0);
	addFace(0, 1, 3);
	addFace(3, 1, 2);
	addFace(0, 3, 2);

	gravity = Vector(0, -10, 0);

	mass = vertex.size();
}

void addVertex(){}