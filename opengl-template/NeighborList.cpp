#include "NeighborList.h"

float NeighborList::deltaDistance(Vertex v, int i)
{
	float dist;

	dist = v.getDistance(*vertexes[i]);

	return dist - distance[i];
}

void NeighborList::add(Vertex *v, Vertex *v2, int index)
{
	vertexes.push_back(v);
	float dist = v->getDistance(*v2);
	distance.push_back(dist);
	indexes.push_back(index);
}

float NeighborList::ratioDistance(Vertex v, int i)
{
	float dist;

	dist = v.getDistance(*vertexes[i]);

	return dist / distance[i];
}
