#pragma once

#include <iostream>

#include "state.h"
#include "../data.h"

class Game : public State
{
private:
	std::shared_ptr<GameData> m_data;

public:
	Game(std::shared_ptr<GameData> data) : m_data(data) {};

	void process() override;
	void render() override;
};