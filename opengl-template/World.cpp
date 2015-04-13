#include "World.h"


World::World(){}


World::~World(){}


void World::Update()
{

}


void World::Acceleration(FObject obj)
{
	for (int i = 0; i < obj.vertex.size(); i++)
	{
		obj.oldPos[i] = obj.vertex[i];

		// newPos = currentPos + Velocity * Time + 1/2 * Acceleration * Time^2
		obj.vertex[i].x += obj.velocity.x * deltaTime + gravity.x * (deltaTime * deltaTime) * 0.5;
		obj.vertex[i].y += obj.velocity.y * deltaTime + gravity.y * (deltaTime * deltaTime) * 0.5;
		obj.vertex[i].z += obj.velocity.z * deltaTime + gravity.z * (deltaTime * deltaTime) * 0.5;
	}
	obj.velocity.x += gravity.x * deltaTime;
}

void Acceleration(FObject obj, float acceleration)
{

}