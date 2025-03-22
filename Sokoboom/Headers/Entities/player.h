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

	Player(int id);

	void process() override;
	void render() override;
};