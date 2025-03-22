#pragma once

#include <iostream>

#include "state.h"
#include "../data.h"
#include "../map.h"

#include <raylib.h>

class Game : public State
{
private:
	std::shared_ptr<GameData> m_data;
	Map m_map;

	EntityController m_entities = EntityController();
public:
	Game(std::shared_ptr<GameData> data, Map map);

	void process() override;
	void render() override;
};