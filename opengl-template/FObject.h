#include <GL\glew.h>
#include <GL\GL.h>
#include "Vertex.h"
#include "Vector.h"
#include "NeighborList.h"
#include "Polygon.h"

class CollisionEdge;

class Edge
{
public:
	static enum states {None, Processing, Doned};

	Vertex *v1;
	Vertex *v2;

	states state;

	bool outerEdge;
	bool operator==(const CollisionEdge&);
	bool operator==(const Edge&);
};

class CollisionEdge
{
public:
	Vertex* vertex[2];
	int collisionIdx;

	bool operator==(const CollisionEdge&);
	bool operator==(const Edge&);
	bool interior;
};

class FObject
{
	public:
		FObject();

		std::vector<Vertex>vertex;
		std::vector<Vertex>oldPos;

		std::vector<Polygon*>face;
		std::vector<NeighborList*>neighbours;

		std::vector<Vector>velocity, oldVelocity;
		std::vector<Vector>acceleration;
		std::vector<Vector>force_external;
		std::vector<Vector>force_internal;
		std::vector<Vector>dep1;
		std::vector<Vector>dep2;
		std::vector<Vector>dep3;

		Vertex center;
		std::vector<float>distanceToCenter;

		Edge *edges;

		float k; //elasticity
		float mass;
		float cdRadius;

		int eSize, eStart;

		Vector gravity;

		float edge(Vertex v1, Vertex v2);

		void addVertex(Vertex v, Vector vel);
		void addEdge(int v1, int v2);
		void addFace(int v1, int v2, int v3);
		bool edgeExists(Edge edge);
		void SetPosition(Vertex pos);

	private:

};