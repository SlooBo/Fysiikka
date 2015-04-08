#include <vector>
#include <GL\glew.h>
#include <GL\GL.h>

#include "Vertex.h"

class Edge
{
public:
	Vertex *v1;
	Vertex *v2;

	bool outerEdge;
};

class FObject
{
	public:
		std::vector<Vertex>vertex;
		std::vector<Vertex>oldPos;

	private:

};