#pragma once

#include <iostream>
#include <memory>

#include "state.h"
#include "../data.h"
#include "../map.h"

#include "../Entities/box.h"
#include "../Entities/goal.h"

#include "../Entities/player.h"
#include "../Entities/player_direction.h"

#include <raylib.h>

class Game : public State
{
private:
	int m_ticks = 0;

	std::shared_ptr<GameData> m_data;
	MapData m_map;

	EntityController m_entities = EntityController();

	std::weak_ptr<Player> m_player;
	
	std::weak_ptr<Box> m_box;
	std::weak_ptr<Goal> m_goal;

	void on_player_moved(Vector2 position, Direction direction);
public:
	Game(std::shared_ptr<GameData> data, MapData map) : m_data(data), m_map(map) {}

	void awake() override;
	void process() override;
	void render() override;
	void leave() override;
};