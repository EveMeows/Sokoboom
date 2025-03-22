#pragma once

#include "entity.h"
#include "../utilities.h"

#include <filesystem>

#include <raylib.h>

class Goal : public Entity 
{
private:
	Texture2D m_texture;
public:
	Goal();

	void process() override {};
	void render() override;
};

inline Goal::Goal() : Entity(0)
{
	this->m_texture = utilities::load_relative("Content/Entities/goal.png");
}

inline void Goal::render()
{
	DrawTextureV(this->m_texture, this->position, WHITE);
}