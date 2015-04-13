#include "FObject.h"

void FObject::SetPosition(Vertex p)
{
	float xDiff = center.x - p.x;
	float yDiff = center.y - p.y;
	float zDiff = center.z - p.z;

	this->center = p;

	for (unsigned int i = 0; i < vertex.size(); i++) {
		vertex[i].x -= xDiff;
		vertex[i].y -= yDiff;
		vertex[i].z -= zDiff;
	}

}