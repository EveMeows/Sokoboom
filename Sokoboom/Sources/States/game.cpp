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
	if (
		this->m_map->get_at_position(
			position.x / GameData::TILE_SIZE, 
			position.y / GameData::TILE_SIZE, 
			SOLID_LAYER
		) == SOLID_WALL
	)
	{
		switch (direction)
		{
		case Direction::LEFT:
			this->m_player->position.x += GameData::TILE_SIZE;
			break;

		case Direction::RIGHT:
			this->m_player->position.x -= GameData::TILE_SIZE;
			break;

		case Direction::UP:
			this->m_player->position.y += GameData::TILE_SIZE;
			break;

		case Direction::DOWN:
			this->m_player->position.y -= GameData::TILE_SIZE;
			break;
		}
	}
}

Game::Game(std::shared_ptr<GameData> data, Map* map) : m_data(data), m_map(map)
{
	for (int i = 0; i < this->m_map->layers.size(); i++)
	{
		std::vector<std::vector<int>> layer = this->m_map->layers[i];

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
							row * this->m_map->tile_size.x,
							col * this->m_map->tile_size.y
						);

						this->m_entities.add(std::make_unique<Box>(box));
						this->m_map->set_at_position(row, col, i, 0);
					} break;

					case GOAL_ID: {
						Goal goal = Goal();
						goal.position = Vector2(
							row * this->m_map->tile_size.x,
							col * this->m_map->tile_size.y
						);

						this->m_entities.add(std::make_unique<Goal>(goal));
						this->m_map->set_at_position(row, col, i, 0);
					} break;

					case PLAYER_ID: {
						Player player_t = Player(PLAYER_ID);
						player_t.position = Vector2(
							row * this->m_map->tile_size.x,
							col * this->m_map->tile_size.y
						);

						player_t.on_player_moved = std::bind_front(&Game::on_player_moved, this);
						this->m_entities.add(std::make_unique<Player>(player_t));

						// ???
						// How do I make this not use a raw pointer...
						// Player* m_player
						this->m_player = this->m_entities.get_id<Player>(PLAYER_ID);

						this->m_map->set_at_position(row, col, i, 0);
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
	this->m_map->draw();
}