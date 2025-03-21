#pragma once

#include <iostream>

#include "state.h"
#include "../data.h"

#include <raylib.h>

class Game : public State
{
private:
	std::shared_ptr<GameData> m_data;
public:
	Game(std::shared_ptr<GameData> data);

	void process() override;
	void render() override;
};