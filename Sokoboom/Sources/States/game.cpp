#include "../../Headers/States/game.h"

#include "../../Headers/utilities.h"

Game::Game(std::shared_ptr<GameData> data) : m_data(data)
{
}

void Game::process()
{
}

void Game::render()
{
	ClearBackground(SKYBLUE);
}