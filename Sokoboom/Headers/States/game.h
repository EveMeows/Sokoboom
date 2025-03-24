#pragma once

#include <vector>
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

static struct MoveData
{
	Vector2 player_position;
	Vector2 box_position;
};

class Game : public State
{
private:
	int m_ticks = 0;

	Font m_font;

	std::shared_ptr<GameData> m_data;
	MapData m_map;

	EntityController m_entities = EntityController();

	std::weak_ptr<Player> m_player;
	
	std::weak_ptr<Box> m_box;
	std::weak_ptr<Goal> m_goal;

	std::vector<MoveData> m_undos;

	void on_player_moved(Vector2 position, Direction direction);

	bool m_switched = false;

	float m_time = 0;
	float m_undo_delay = 0.35f;
	bool m_undoing = false;
	void undo();
public:
	Game(std::shared_ptr<GameData> data, MapData map) : m_data(data), m_map(map) {}

	void awake() override;
	void process() override;
	void render() override;
	void leave() override;
};