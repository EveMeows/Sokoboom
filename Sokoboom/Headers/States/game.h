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
	Map* m_map = nullptr;

	EntityController m_entities = EntityController();

	Player* m_player = nullptr;
	void on_player_moved(Vector2 position, Direction direction);
public:
	Game(std::shared_ptr<GameData> data, Map* map);

	void process() override;
	void render() override;
};