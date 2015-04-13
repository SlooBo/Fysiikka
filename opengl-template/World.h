#pragma once
#include <vector>
#include "FObject.h"

class World
{
public:
	World();
	~World();

	void Update();

private:
	float deltaTime;
	void Acceleration(FObject obj);
	void Acceleration(FObject obj, glm::vec3 acceleration);
	const glm::vec3 gravity = glm::vec3(0, -9.81, 0);
	std::vector<FObject> objects;
};

