#include "opengl-template\NeighborList.h"

float NeighborList::DeltaDistance(Vertex v, int i)
{
	float dist;

	dist = v.GetDistance(*vertexes[i]);

	return dist - distance[i];
}

void NeighborList::Add(Vertex *v, Vertex *v2, int index)
{
	vertexes.push_back(v);
	float dist = v->GetDistance(*v2);
	distance.push_back(dist);
	indexes.push_back(index);
}

float NeighborList::RatioDistance(Vertex v, int i)
{
	float dist;

	dist = v.GetDistance(*vertexes[i]);

	return dist / distance[i];
}
