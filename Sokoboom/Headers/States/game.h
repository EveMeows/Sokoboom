#pragma once

#include <iostream>
#include <memory>

#include "state.h"
#include "../data.h"
#include "../map.h"
#include "../Entities/player.h"
#include "../Entities/player_direction.h"

#include <raylib.h>

class Game : public State
{
private:
	std::shared_ptr<GameData> m_data;
	Map m_map;

	EntityController m_entities = EntityController();

	std::weak_ptr<Player> m_player;
	void on_player_moved(Vector2 position, Direction direction);
public:
	Game(std::shared_ptr<GameData> data, Map map) : m_data(data), m_map(map) {}

	void awake() override;
	void process() override;
	void render() override;
};