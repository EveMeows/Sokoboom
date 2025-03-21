#include "../Headers/map.h"

#include "../Headers/utilities.h"

#include <iostream>

Map::Map(const std::filesystem::path& path)
{
	std::filesystem::path full = GetApplicationDirectory() / path;
	std::ifstream file(full);
	this->m_data = nlohmann::json::parse(file);
	file.close();

	this->layers = this->m_data["layers"];
	this->tile_size = Vector2(this->m_data["tile_x"], this->m_data["tile_y"]);

	this->m_tiles[1] = utilities::load_relative(std::filesystem::path("Content/Props/wall.png"));
}

void Map::draw()
{
	for (const std::vector<std::vector<int>>& layer : this->layers)
	{
		for (size_t row = 0; row < layer.size(); row++)
		{
			for (size_t col = 0; col < layer[row].size(); col++)
			{
				if (layer[row][col] == 0) continue;

				DrawTexture(
					this->m_tiles[layer[row][col]],
					row * this->tile_size.x, col * this->tile_size.y,
					WHITE
				);
			}
		}
	}
}