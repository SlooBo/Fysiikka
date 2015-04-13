#include "Polygon.h"

bool Polygon::isIn(Vertex *v)
{
	for (int i = 0; i < 3; i++)
	{
		if (v == vertex[i])
		{
			return true;
		}
	}
	return false;
}

void Polygon::getOtherPoints(Vertex *vs[2], Vertex *v)
{
	if (v == vertex[0])
	{
		vs[0] = vertex[1];
		vs[1] = vertex[2];
	}
	else if (v == vertex[1])
	{
		vs[0] = vertex[0];
		vs[1] = vertex[2];
	}
	else if (v == vertex[2])
	{
		vs[0] = vertex[0];
		vs[1] = vertex[1];
	}
}

//not doned
Vector Polygon::normal()
{
	//Vector vector1 = *vertex[1] - *vertex[0];
	//Vector vector2 = *vertex[1] - *vertex[2];

	//return vector2.CrossProduct(vector1).Normalize();
}