#pragma once

#include <vector>

#include "Vertex.h"

class NeighborList
{
public:
	std::vector<Vertex*>vertexes;
	std::vector<int>indexes;
	std::vector<float>distance;

	float DeltaDistance(Vertex v, int i);
	void Add(Vertex *v, Vertex *v2, int index);
	float RatioDistance(Vertex v, int i);
};

