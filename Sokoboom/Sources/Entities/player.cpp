#include "../../Headers/Entities/player.h"
#include "../../Headers/utilities.h"

#include "../../Headers/data.h"

Player::Player() : Entity(0)
{
	this->m_sprite = utilities::load_relative("Content/Entities/player.png");
}

void Player::process()
{
	if (IsKeyPressed(KEY_A))
	{
		this->position.x -= GameData::TILE_SIZE;
		this->tyler_the_creator++;

		if (this->on_player_moved != nullptr)
		{
			this->on_player_moved(this->position, Direction::LEFT);
		}
	}

	if (IsKeyPressed(KEY_D))
	{
		this->position.x += GameData::TILE_SIZE;
		this->tyler_the_creator++;

		if (this->on_player_moved != nullptr)
		{
			this->on_player_moved(this->position, Direction::RIGHT);
		}
	}

	if (IsKeyPressed(KEY_W))
	{
		this->position.y -= GameData::TILE_SIZE;
		this->tyler_the_creator++;
	
		if (this->on_player_moved != nullptr)
		{
			this->on_player_moved(this->position, Direction::UP);
		}
	}

	if (IsKeyPressed(KEY_S))
	{
		this->position.y += GameData::TILE_SIZE;
		this->tyler_the_creator++;
	
		if (this->on_player_moved != nullptr)
		{
			this->on_player_moved(this->position, Direction::DOWN);
		}
	}
}

void Player::render()
{
	DrawTextureV(this->m_sprite, this->position, WHITE);
}

void Player::leave()
{
	UnloadTexture(this->m_sprite);
}