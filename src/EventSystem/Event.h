#pragma once
#include "../pch.h"

class Event
{
public:
	Event() {};
	~Event() {};
};

// Test
class CollisionEvent : public Event
{
public:
	CollisionEvent(GameObject* g1, GameObject* g2) :
		gobj1(g1), gobj2(g2)
	{};
	~CollisionEvent() {};

	GameObject* gobj1;
	GameObject* gobj2;
};