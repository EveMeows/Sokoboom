#pragma once

#include "entity.h"
#include "player_direction.h"

#include <functional>

#include <raylib.h>

class Player : public Entity
{
private:
	Texture2D m_sprite;

public:
	std::function<void(Vector2, Direction)> on_player_moved = nullptr;
	int tyler_the_creator = 0;

	bool locked = false;

	Player();

	void process() override;
	void render() override;
	void leave() override;
};