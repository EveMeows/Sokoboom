#include "../../Headers/States/game.h"
#include "../../Headers/utilities.h"

#include "../../Headers/Entities/box.h"
#include "../../Headers/Entities/goal.h"

Game::Game(std::shared_ptr<GameData> data, Map map) : m_data(data), m_map(map)
{
	for (int i = 0; i < this->m_map.layers.size(); i++)
	{
		std::vector<std::vector<int>> layer = this->m_map.layers[i];

		for (size_t row = 0; row < layer.size(); row++)
		{
			for (size_t col = 0; col < layer[row].size(); col++)
			{
				if (layer[row][col] == 0) continue;

				switch (layer[row][col])
				{
					// Box
					case 2:
					{
						Box box = Box();
						box.position = Vector2(
							row * this->m_map.tile_size.x,
							col * this->m_map.tile_size.y
						);

						this->m_entities.add(std::make_unique<Box>(box));
						this->m_map.set_at_position(row, col, i, 0);
						break;
					}

					// Goal
					case 3:
					{
						Goal goal = Goal();
						goal.position = Vector2(
							row * this->m_map.tile_size.x,
							col * this->m_map.tile_size.y
						);

						this->m_entities.add(std::make_unique<Goal>(goal));
						this->m_map.set_at_position(row, col, i, 0);
						break;
					}
				}
			}
		}
	}
}

void Game::process()
{
	this->m_entities.process();
}

void Game::render()
{
	ClearBackground(DARKBLUE);

	this->m_entities.render();
	this->m_map.draw();
}