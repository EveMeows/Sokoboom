#pragma once

#include <map>
#include <vector>
#include <filesystem>

#include <fstream>
#include <sstream>

#include <json.hpp>
#include <raylib.h>

class Map
{
private:
	std::map<int, Texture2D> m_tiles;
	nlohmann::json m_data;
public:
	Map(const std::filesystem::path& path);

	std::vector<std::vector<std::vector<int>>> layers;
	Vector2 tile_size;

	void draw();
	void leave();

	int get_at_position(int x, int y, int layer);
	void set_at_position(int x, int y, int layer, int id);
};