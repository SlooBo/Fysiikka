#include "opengl-template\FObject.h"

bool Edge::operator==(const CollisionEdge& right)
{
	//if (this->v1 == right.vertex[])

	return false;
}

bool Edge::operator==(const Edge& right)
{
	if (this == &right)
	{
		return true;
	}

	return false;
}

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

void FObject::addVertex(Vertex v, Vector vel)
{
	NeighborList *neighbors = new NeighborList;
	vertex.push_back(v);
	oldPos.push_back(v);

	velocity.push_back(vel);
	oldVelocity.push_back(vel);
	force_external.push_back(Vector(0, 0, 0));
	force_internal.push_back(Vector(0, 0, 0));
	acceleration.push_back(Vector(0, 0, 0));
	neighbours.push_back(neighbors);

	dep1.push_back(Vector(0, 0, 0));
	dep2.push_back(Vector(0, 0, 0));
	dep3.push_back(Vector(0, 0, 0));
}

void FObject::addEdge(int v1, int v2)
{
	Edge edge;

	edge.v1 = &vertex[v1];
	edge.v2 = &vertex[v2];

	if (!edgeExists(edge))
	{
		edges[eStart] = edge;

		eStart++;
	}	
}

bool FObject::edgeExists(Edge edge)
{
	for (int i = 0; i < eStart; i++)
	{
		if (edges[i] == edge)
		{
			return true;
		}
	}
	return false;
}

void FObject::addFace(int v1, int v2, int v3)
{
	Polygon *polygon = new Polygon;
	polygon->vertex[0] = &vertex[v1];
	polygon->vertex[1] = &vertex[v2];
	polygon->vertex[2] = &vertex[v3];
	polygon->normal = polygon->Normal();

	addEdge(v1, v2);
	addEdge(v2, v3);
	addEdge(v3, v1);

	face.push_back(polygon);

	neighbours[v1]->add(&vertex[v2], &vertex[v1], v2);
	neighbours[v1]->add(&vertex[v3], &vertex[v1], v3);
	Vector vector1 = vertex[v2] - vertex[v1];
	Vector vector2 = vertex[v3] - vertex[v1];

	//todo distances

	neighbours[v2]->add(&vertex[v1], &vertex[v2], v1);
	neighbours[v2]->add(&vertex[v3], &vertex[v2], v3);
	vector1 = vertex[v1] - vertex[v2];
	vector2 = vertex[v3] - vertex[v2];

	//distances

	neighbours[v3]->add(&vertex[v1], &vertex[v3], v1);
	neighbours[v3]->add(&vertex[v2], &vertex[v3], v2);
	vector1 = vertex[v1] - vertex[v3];
	vector2 = vertex[v2] - vertex[v3];

}

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
