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
		
		void SetPosition(Vertex pos);
		
		
		
		
		
		
		
		
		std::vector<Vertex>vertex;
		std::vector<Vertex>oldPos;

		Vertex center;

		//math variables
		glm::vec3 velocity;
		float k; // Variable for elasticity
		float mass;
	private:

};