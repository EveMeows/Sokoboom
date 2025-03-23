#include "../../Headers/States/game.h"
#include "../../Headers/utilities.h"

#include "../../Headers/Entities/box.h"
#include "../../Headers/Entities/goal.h"

#include "../../Headers/data.h"

#include <cmath>
#include <stdexcept>

static constexpr int BOX_ID = 2;
static constexpr int GOAL_ID = 3;
static constexpr int PLAYER_ID = 4;

static constexpr int SOLID_LAYER = 0;
static constexpr int SOLID_WALL = 1;

void Game::on_player_moved(Vector2 position, Direction direction)
{
	std::shared_ptr<Player> player = this->m_player.lock();
	if (!player)
	{
		std::cerr << "CRITICAL: Failed to get player.\n";
		return;
	}

	std::cout << "INFO: Player weak link uses: " << player.use_count() << std::endl;

	if (
		this->m_map.get_at_position(
			position.x / GameData::TILE_SIZE, 
			position.y / GameData::TILE_SIZE, 
			SOLID_LAYER
		) == SOLID_WALL
	)
	{
		switch (direction)
		{
		case Direction::LEFT:
			player->position.x += GameData::TILE_SIZE;
			player->moves--;

			break;

		case Direction::RIGHT:
			player->position.x -= GameData::TILE_SIZE;
			player->moves--;

			break;

		case Direction::UP:
			player->position.y += GameData::TILE_SIZE;
			player->moves--;

			break;

		case Direction::DOWN:
			player->position.y -= GameData::TILE_SIZE;
			player->moves--;

			break;
		}
	}
}

void Game::awake()
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
					case BOX_ID: {
						Box box = Box();
						box.position = Vector2(
							row * this->m_map.tile_size.x,
							col * this->m_map.tile_size.y
						);

						this->m_entities.add(std::make_unique<Box>(box));
						this->m_map.set_at_position(row, col, i, 0);
					} break;

					case GOAL_ID: {
						Goal goal = Goal();
						goal.position = Vector2(
							row * this->m_map.tile_size.x,
							col * this->m_map.tile_size.y
						);

						this->m_entities.add(std::make_unique<Goal>(goal));
						this->m_map.set_at_position(row, col, i, 0);
					} break;

					case PLAYER_ID: {
						std::shared_ptr<Player> player_t = std::make_shared<Player>();
						player_t->position = Vector2(
							row * this->m_map.tile_size.x,
							col * this->m_map.tile_size.y
						);

						player_t->on_player_moved = 
							std::bind_front(&Game::on_player_moved, this);
					
						this->m_entities.add(player_t);
						this->m_player = player_t;

						this->m_map.set_at_position(row, col, i, 0);
					} break;
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

void Game::leave()
{
	this->m_entities.leave();
}