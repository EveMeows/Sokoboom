#pragma once

#include "Controllers/state_controller.h"
#include "map.h"

#include <raylib.h>

#include <memory>

struct MapData
{
	std::string name;
	Map map;
};

struct GameData
{
	bool mute_move = false;
	bool mute_sfx = false;

	int total_moves = 0;
	int active_map_index = 0;
	std::vector<MapData> maps;

	static constexpr int GAP = 7;
	static constexpr int TILE_SIZE = 8;
	static constexpr int MAP_SIZE = 10;

	static constexpr Vector2 GAME_SIZE = Vector2(TILE_SIZE * MAP_SIZE, TILE_SIZE * MAP_SIZE + GAP);

	const std::unique_ptr<StateController> state_handler = std::make_unique<StateController>();
	Vector2 virtual_mouse;

	bool exit = false;
};