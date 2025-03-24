#include "../../Headers/States/game.h"
#include "../../Headers/utilities.h"

#include "../../Headers/data.h"

#include <cmath>
#include <stdexcept>

#include <raymath.h>

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

	std::shared_ptr<Box> box = this->m_box.lock();
	if (!box)
	{
		std::cerr << "CRITICAL: Failed to get box.\n";
		return;
	}

	Vector2 player_grid = Vector2Scale(position, 1.0f / GameData::TILE_SIZE);
	Vector2 box_grid = Vector2Scale(box->position, 1.0f / GameData::TILE_SIZE);

	// Box moves
	if (player_grid == box_grid)
	{
		switch (direction)
		{
		case Direction::LEFT:
			if (this->m_map.map.get_at_position(box_grid.x - 1, box_grid.y, SOLID_LAYER) == SOLID_WALL)
			{
				player->position.x += GameData::TILE_SIZE;
				player->moves--;
				break;
			}

			box->position.x -= GameData::TILE_SIZE;

			break;

		case Direction::RIGHT:
			if (this->m_map.map.get_at_position(box_grid.x + 1, box_grid.y, SOLID_LAYER) == SOLID_WALL)
			{
				player->position.x -= GameData::TILE_SIZE;
				player->moves--;
				break;
			}

			box->position.x += GameData::TILE_SIZE;

			break;

		case Direction::UP:
			if (this->m_map.map.get_at_position(box_grid.x, box_grid.y - 1, SOLID_LAYER) == SOLID_WALL)
			{
				player->position.y += GameData::TILE_SIZE;
				player->moves--;
				break;
			}

			box->position.y -= GameData::TILE_SIZE;

			break;

		case Direction::DOWN:
			if (this->m_map.map.get_at_position(box_grid.x, box_grid.y + 1, SOLID_LAYER) == SOLID_WALL)
			{
				player->position.y -= GameData::TILE_SIZE;
				player->moves--;
				break;
			}

			box->position.y += GameData::TILE_SIZE;

			break;
		}
	}

	// Player hits wall
	if (
		this->m_map.map.get_at_position(
			player_grid.x, player_grid.y,
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

	if (this->m_undos.empty())
	{
		this->m_undos.push_back(
			MoveData(player->position, box->position)
		);
	}
	else
	{
		MoveData last = this->m_undos[this->m_undos.size() - 1];
		if (player->position != last.player_position)
		{
			this->m_undos.push_back(
				MoveData(player->position, box->position)
			);
		}
	}
}

void Game::undo()
{
	if (!this->m_undos.empty())
	{
		int last_idx = this->m_undos.size() - 1;
		MoveData last = this->m_undos[last_idx];

		std::shared_ptr<Box> box = this->m_box.lock();
		if (!box)
		{
			std::cerr << "CRITICAL: Failed to get box.\n";
			return;
		}

		std::shared_ptr<Player> player = this->m_player.lock();
		if (!player)
		{
			std::cerr << "CRITICAL: Failed to get goal.\n";
			return;
		}

		player->position = last.player_position;
		box->position = last.box_position;

		if (this->m_undos.size() > 1)
		{
			this->m_undos.erase(this->m_undos.begin() + last_idx);
		}
	}
}

void Game::awake()
{
	for (int i = 0; i < this->m_map.map.layers.size(); i++)
	{
		std::vector<std::vector<int>> layer = this->m_map.map.layers[i];

		for (size_t row = 0; row < layer.size(); row++)
		{
			for (size_t col = 0; col < layer[row].size(); col++)
			{
				if (layer[row][col] == 0) continue;

				switch (layer[row][col])
				{
					case BOX_ID: {
						std::shared_ptr<Box> box = std::make_shared<Box>();
						box->position = Vector2(
							row * this->m_map.map.tile_size.x,
							col * this->m_map.map.tile_size.y
						);

						this->m_box = box;
						this->m_entities.add(box);
						
						this->m_map.map.set_at_position(row, col, i, 0);
					} break;

					case GOAL_ID: {
						std::shared_ptr<Goal> goal = std::make_shared<Goal>();
						goal->position = Vector2(
							row * this->m_map.map.tile_size.x,
							col * this->m_map.map.tile_size.y
						);

						this->m_goal = goal;
						this->m_entities.add(goal);

						this->m_map.map.set_at_position(row, col, i, 0);
					} break;

					case PLAYER_ID: {
						std::shared_ptr<Player> player_t = std::make_shared<Player>();
						player_t->position = Vector2(
							row * this->m_map.map.tile_size.x,
							col * this->m_map.map.tile_size.y
						);

						player_t->on_player_moved = 
							std::bind_front(&Game::on_player_moved, this);
					
						this->m_entities.add(player_t);
						this->m_player = player_t;

						this->m_map.map.set_at_position(row, col, i, 0);
					} break;
				}
			}
		}
	}

	std::shared_ptr<Player> player = this->m_player.lock();
	if (!player)
	{
		std::cerr << "CRITICAL: Failed to get player.\n";
		return;
	}

	std::shared_ptr<Box> box = this->m_box.lock();
	if (!box)
	{
		std::cerr << "CRITICAL: Failed to get box.\n";
		return;
	}

	this->m_undos.push_back(
		MoveData(player->position, box->position)
	);
}

void Game::process()
{
	this->m_ticks++;
	this->m_entities.process();

	if (IsKeyPressed(KEY_R))
	{
		if (!this->switched)
		{
			this->m_data->state_handler->set(
				std::make_unique<Game>(
					this->m_data, this->m_data->maps[this->m_data->active_map_index]
				)
			);

			this->switched = true;
		}
	}

	// Low budget pressed repeat function
	if (IsKeyDown(KEY_Z))
	{
		if (!this->m_undoing)
		{
			// Remove the last stored movement if its the same position.
			if (std::shared_ptr<Player> player = this->m_player.lock())
			{
				int last_idx = this->m_undos.size() - 1;
				MoveData last = this->m_undos[last_idx];
				if (this->m_undos.size() > 1 && player->position == last.player_position)
				{
					this->m_undos.erase(this->m_undos.begin() + last_idx);
				}
			}
			else
			{
				std::cerr << "CRITICAL: Failed to get goal.\n";
				return;
			}

			this->undo();	
			this->m_undoing = true;
		}
	}
	else
	{
		if (this->m_undoing)
		{
			this->m_time = 0;
			this->m_undoing = false;
			this->m_undo_delay = 0.35f;
		}
	}

	if (this->m_undoing)
	{
		this->m_time += GetFrameTime();
		if (this->m_time >= this->m_undo_delay)
		{
			this->m_time = 0;
			this->m_undo_delay = 0.05f;
			this->undo();
		}
	}

	if (this->m_ticks == 30)
	{
		this->m_ticks = 0;

		std::shared_ptr<Box> box = this->m_box.lock();
		if (!box)
		{
			std::cerr << "CRITICAL: Failed to get box.\n";
			return;
		}

		std::shared_ptr<Goal> goal = this->m_goal.lock();
		if (!goal)
		{
			std::cerr << "CRITICAL: Failed to get goal.\n";
			return;
		}

		if (
			Vector2Scale(box->position, 1.0f / GameData::TILE_SIZE) ==
			Vector2Scale(goal->position, 1.0f / GameData::TILE_SIZE)
			)
		{
			if (!this->switched)
			{
				int max = this->m_data->maps.size();
				this->m_data->active_map_index++;

				if (this->m_data->active_map_index >= max)
				{
					// TODO: win
					return;
				}

				this->m_data->state_handler->set(
					std::make_unique<Game>(
						this->m_data, this->m_data->maps[this->m_data->active_map_index]
					)
				);

				this->switched = true;
			}
		}
	}
}

void Game::render()
{
	ClearBackground(DARKBLUE);

	this->m_entities.render();
	this->m_map.map.draw();
}

void Game::leave()
{
	this->m_entities.leave();
}