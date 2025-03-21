#pragma once

#include "Controllers/state_controller.h"

#include <raylib.h>

#include <memory>

struct GameData
{
	static constexpr int TILE_SIZE = 8;
	static constexpr int MAP_SIZE = 10;

	static constexpr Vector2 GAME_SIZE = Vector2(TILE_SIZE * MAP_SIZE, TILE_SIZE * MAP_SIZE);

	const std::unique_ptr<StateController> state_handler = std::make_unique<StateController>();
};