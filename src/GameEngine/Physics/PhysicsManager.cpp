#include "pch.h"

PhysicsManager* PhysicsManager::instance = nullptr;
PhysicsManager* PhysicsManager::GetInstance()
{
	static PhysicsManager* instance = new PhysicsManager();
	return instance;
}
PhysicsManager::~PhysicsManager()
{
	delete instance;
}


Body* PhysicsManager::CreateBody()
{
	Body* newBody = new Body();
	allBodies.push_back(newBody);
	return newBody;
}

void PhysicsManager::DeleteBody(Body* body)
{
	allBodies.erase(std::remove(allBodies.begin(), allBodies.end(), body), allBodies.end());
}

void PhysicsManager::Integrate()
{
	for (Body* b : allBodies)
	{
		b->Integrate();
	}
}

