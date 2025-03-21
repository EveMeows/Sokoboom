#include "../../Headers/States/game.h"

#include "../../Headers/utilities.h"

Game::Game(std::shared_ptr<GameData> data, Map map) : m_data(data), m_map(map)
{
}

void Game::process()
{
}

void Game::render()
{
	ClearBackground(DARKBLUE);
	this->m_map.draw();
}