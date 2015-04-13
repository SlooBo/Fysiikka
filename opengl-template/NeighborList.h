#pragma once

#include <vector>

#include "Vertex.h"

class NeighborList
{
public:
	std::vector<Vertex*>vertexes;
	std::vector<int>indexes;
	std::vector<float>distance;

	float deltaDistance(Vertex v, int i);
	void add(Vertex *v, Vertex *v2, int index);
	float ratioDistance(Vertex v, int i);
};

