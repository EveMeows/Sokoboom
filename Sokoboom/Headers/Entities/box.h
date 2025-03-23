#pragma once

#include "entity.h"
#include "../utilities.h"

#include <filesystem>

#include <raylib.h>

class Box : public Entity 
{
private:
	Texture2D m_texture;

public:
	Box();

	void render() override;
	void leave() override;
};

inline Box::Box() : Entity(0)
{
	this->m_texture = utilities::load_relative(std::filesystem::path("Content/Entities/box.png"));
}

inline void Box::render()
{
	DrawTextureV(this->m_texture, this->position, WHITE);
}

inline void Box::leave()
{
	UnloadTexture(this->m_texture);
}