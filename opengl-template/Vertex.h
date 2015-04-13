#include <GLM\glm.hpp>
#include <vector>

class Vertex
{
public:
	Vertex(){ this->x = 0; this->y = 0; this->z = 0; }

	Vertex(float x, float y, float z){ this->x = x, this->y = y, this->z = z; }
	
	Vertex operator+(const Vertex &v){ return Vertex(x + v.x, y + v.y, z + v.z); };

	Vertex operator-(const Vertex &v){ return Vertex(x - v.x, y - v.y, z - v.z); };

	bool operator==(const Vertex &v){ return (x == v.x, y == v.y, z == v.z); }

	float getDistance(Vertex& v){ return (float)glm::sqrt(glm::pow(x - v.x, 2) + glm::pow(y - v.y, 2) + glm::pow(z - v.z, 2)); }

	float x, y, z;
};