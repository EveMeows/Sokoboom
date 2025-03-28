#include "../../Headers/States/game.h"

#include "../../Headers/States/end.h"
#include "../../Headers/States/menu.h"
#include "../../Headers/utilities.h"
#include "../../Headers/data.h"

#include <cmath>
#include <stdexcept>
#include <format>

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

	bool hit_corner = false;
	bool hit_box = false;

	Vector2 player_grid = Vector2Scale(position, 1.0f / GameData::TILE_SIZE);

	if (this->m_finished)
	{
		// Reached gate.
		if (player_grid.x == 9 && (player_grid.y == 8 || player_grid.y == 7))
		{
			player->locked = true;
			this->m_data->total_moves += player->tyler_the_creator;

			this->m_data->state_handler->set(std::make_unique<End>(this->m_data));
		}
	}

	// Player hits wall
	if (
		this->m_map.map.get_at_position(
			(int)player_grid.x, (int)player_grid.y,
			SOLID_LAYER
		) == SOLID_WALL
	)
	{
		switch (direction)
		{
		case Direction::LEFT:
			player->position.x += GameData::TILE_SIZE;
			player->tyler_the_creator--;

			break;

		case Direction::RIGHT:
			player->position.x -= GameData::TILE_SIZE;
			player->tyler_the_creator--;

			break;

		case Direction::UP:
			player->position.y += GameData::TILE_SIZE;
			player->tyler_the_creator--;

			break;

		case Direction::DOWN:
			player->position.y -= GameData::TILE_SIZE;
			player->tyler_the_creator--;

			break;
		}

		hit_corner = true;
	}

	if (!this->m_finished)
	{
		std::shared_ptr<Box> box = this->m_box.lock();
		if (!box)
		{
			std::cerr << "CRITICAL: Failed to get box.\n";
			return;
		}

		Vector2 box_grid = Vector2Scale(box->position, 1.0f / GameData::TILE_SIZE);

		// Box moves
		if (player_grid == box_grid)
		{
			switch (direction)
			{
			case Direction::LEFT:
				if (this->m_map.map.get_at_position((int)box_grid.x - 1, (int)box_grid.y, SOLID_LAYER) == SOLID_WALL)
				{
					player->position.x += GameData::TILE_SIZE;
					player->tyler_the_creator--;

					hit_box = true;
					break;
				}

				box->position.x -= GameData::TILE_SIZE;

				break;

			case Direction::RIGHT:
				if (this->m_map.map.get_at_position((int)box_grid.x + 1, (int)box_grid.y, SOLID_LAYER) == SOLID_WALL)
				{
					player->position.x -= GameData::TILE_SIZE;
					player->tyler_the_creator--;

					hit_box = true;
					break;
				}

				box->position.x += GameData::TILE_SIZE;

				break;

			case Direction::UP:
				if (this->m_map.map.get_at_position((int)box_grid.x, (int)box_grid.y - 1, SOLID_LAYER) == SOLID_WALL)
				{
					player->position.y += GameData::TILE_SIZE;
					player->tyler_the_creator--;

					hit_box = true;
					break;
				}

				box->position.y -= GameData::TILE_SIZE;

				break;

			case Direction::DOWN:
				if (this->m_map.map.get_at_position((int)box_grid.x, (int)box_grid.y + 1, SOLID_LAYER) == SOLID_WALL)
				{
					player->position.y -= GameData::TILE_SIZE;
					player->tyler_the_creator--;

					hit_box = true;
					break;
				}

				box->position.y += GameData::TILE_SIZE;

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

	if (!hit_box && !hit_corner)
	{
		if (!this->m_data->mute_sfx && !this->m_data->mute_move) PlaySound(this->m_move);
	}
}

void Game::undo()
{
	if (this->m_finished) return;

	if (!this->m_undos.empty())
	{
		int last_idx = (int)this->m_undos.size() - 1;
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
	// Sound 
	this->m_move = utilities::load_sound_relative(std::filesystem::path("Content/Audio/move.wav"));
	this->m_next = utilities::load_sound_relative(std::filesystem::path("Content/Audio/next.wav"));
	this->m_explode = utilities::load_sound_relative(std::filesystem::path("Content/Audio/explosion.wav"));

	for (int i = 0; i < this->m_map.map.layers.size(); i++)
	{
		std::vector<std::vector<int>> layer = this->m_map.map.layers[i];

		for (int row = 0; row < layer.size(); row++)
		{
			for (int col = 0; col < layer[row].size(); col++)
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

	this->m_font = utilities::load_font_relative(std::filesystem::path("Content/pico-8.ttf"));

	// Pause UI
	Vector2 resume_dim = MeasureTextEx(this->m_font, "resume", 10.0f, 0.1f);
	Button resume = Button(
		this->m_font,
		"resume", 10.0f,
		Vector2(
			(GameData::GAME_SIZE.x - resume_dim.x) / 2,
			40
		)
	);

	resume.on_click = [this](Button* self) {
		this->m_paused = false;
	};

	this->m_buttons.push_back(resume);

	Vector2 menu_dim = MeasureTextEx(this->m_font, "menu", 10.0f, 0.1f);
	Button menu = Button(
		this->m_font,
		"menu", 10.0f,
		Vector2(
			(GameData::GAME_SIZE.x - menu_dim.x) / 2,
			50
		)
	);

	menu.on_click = [this](Button* self) {
		this->m_data->active_map_index = 0;
		this->m_data->total_moves = 0;

		this->m_data->state_handler->set(
			std::make_unique<Menu>(this->m_data)
		);
	};

	this->m_buttons.push_back(menu);
}

void Game::process()
{
	if (IsKeyPressed(KEY_ESCAPE))
	{
		this->m_paused = !this->m_paused;
	}

	if (this->m_paused)
	{
		for (Button& btn : this->m_buttons)
		{
			btn.process(this->m_data->virtual_mouse);
		}

		return;
	}

	if (!this->m_finished)
	{
		this->m_ticks++;
	}

	this->m_entities.process();

	if (IsKeyPressed(KEY_R))
	{
		if (!this->m_switched)
		{
			this->m_data->state_handler->set(
				std::make_unique<Game>(
					this->m_data, this->m_data->maps[this->m_data->active_map_index]
				)
			);

			this->m_switched = true;
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
				int last_idx = (int)this->m_undos.size() - 1;
				MoveData last = this->m_undos[last_idx];
				if (this->m_undos.size() > 1 && player->position == last.player_position)
				{
					this->m_undos.erase(this->m_undos.begin() + last_idx);
				}
			}
			else
			{
				std::cerr << "CRITICAL: Failed to get player.\n";
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

	if (this->m_ticks == 30 && !this->m_finished)
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
			if (!this->m_switched)
			{
				// The end
				if (this->m_data->active_map_index == 11)
				{
					this->m_entities.remove<Box>();

					this->m_switched = true;
					this->m_finished = true;

					// Gate
					this->m_map.map.set_at_position(9, 8, 0, 0);
					this->m_map.map.set_at_position(9, 7, 0, 0);

					if (!this->m_data->mute_sfx) PlaySound(this->m_explode);

					return;
				}

				if (std::shared_ptr<Player> player = this->m_player.lock())
				{
					this->m_data->total_moves += player->tyler_the_creator;
				}

				if (!this->m_data->mute_sfx) PlaySound(this->m_next);

				int max = (int)this->m_data->maps.size();
				this->m_data->active_map_index++;

				this->m_data->state_handler->set(
					std::make_unique<Game>(
						this->m_data, this->m_data->maps[this->m_data->active_map_index]
					)
				);

				this->m_switched = true;
			}
		}
	}
}

void Game::render()
{
	ClearBackground(DARKBLUE);

	this->m_entities.render();
	this->m_map.map.draw();

	if (std::shared_ptr<Player> player = this->m_player.lock())
	{
		DrawRectangle(
			0, (int)GameData::GAME_SIZE.y - GameData::GAP, (int)GameData::GAME_SIZE.x, GameData::GAP, GRAY
		);

		DrawTextPro(
			this->m_font, 
			std::format("{:03} MOVES", player->tyler_the_creator).c_str(), 
			Vector2(1, GameData::GAME_SIZE.y - GameData::GAP + 1), 
			Vector2Zero(), 0, 5.0f, 0.1f, WHITE
		);
	
		std::string name = std::format("-{}-", this->m_map.name);
		Vector2 name_dim = MeasureTextEx(this->m_font, name.c_str(), 5.0f, 0.1f);

		DrawTextPro(
			this->m_font, 
			name.c_str(), 
			Vector2(
				floor(GameData::GAME_SIZE.x - name_dim.x), 
				floor(GameData::GAME_SIZE.y - GameData::GAP + 1)
			),
			Vector2Zero(), 0, 5.0f, 0.1f, WHITE
		);
	}
	else
	{
		std::cerr << "CRITICAL: Failed to get player.\n";
		return;
	}

	if (this->m_paused)
	{
		DrawRectangleV(
			Vector2Zero(), GameData::GAME_SIZE,
			Fade(BLACK, 0.7f)
		);

		Vector2 dim = MeasureTextEx(this->m_font, "PAUSED", 10.0f, 0.1f);
		DrawTextPro(
			this->m_font,
			"PAUSED",
			Vector2(
				(GameData::GAME_SIZE.x - dim.x) / 2,
				5
			),
			Vector2Zero(),
			0, 10.0f, 0.1f, WHITE
		);

		for (Button& btn : this->m_buttons)
		{
			btn.render();
		}
	}
}

void Game::leave()
{
	this->m_entities.leave();
	
	// This breaks the game if you press R
	// for some reason
	// this->m_map.map.leave();

	UnloadSound(this->m_next);
	UnloadSound(this->m_move);
	UnloadSound(this->m_explode);

	UnloadFont(this->m_font);
}