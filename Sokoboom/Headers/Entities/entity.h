#pragma once

#include <raylib.h>

class Entity 
{
public:
	int id;

	Vector2 position = Vector2(0, 0);
	Vector2 velocity = Vector2(0, 0);

	Entity(int id) : id(id) {};
};