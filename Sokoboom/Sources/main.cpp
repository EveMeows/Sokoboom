#include <iostream>
#include <memory>
#include <cmath>
#include <map>

#include "../Headers/map.h"
#include "../Headers/data.h"
#include "../Headers/States/game.h"
#include "../Headers/States/menu.h"

#include <raylib.h>
#include <raymath.h>

// Disable terminal in release mode only
#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif // !_DEBUG

int main()
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 600, "Sokoboom");
	SetExitKey(KEY_NULL);

	InitAudioDevice();

	std::shared_ptr<GameData> data = std::make_shared<GameData>();

	// Load settings
	std::filesystem::path path = GetApplicationDirectory() / std::filesystem::path("Content/settings.json");
	std::ifstream f(path);
	if (f.is_open())
	{
		nlohmann::json json = nlohmann::json::parse(f);

		data->mute_sfx = json["mute_sfx"];
		data->mute_move = json["mute_move"];

		f.close();
	}

	// Create maps
	data->maps.push_back(MapData("INTRO", Map(std::filesystem::path("Content/Maps/intro.p8m"))));

	data->maps.push_back(MapData("1", Map(std::filesystem::path("Content/Maps/one.p8m"))));
	data->maps.push_back(MapData("2", Map(std::filesystem::path("Content/Maps/two.p8m"))));
	data->maps.push_back(MapData("3", Map(std::filesystem::path("Content/Maps/three.p8m"))));
	data->maps.push_back(MapData("4", Map(std::filesystem::path("Content/Maps/four.p8m"))));
	data->maps.push_back(MapData("5", Map(std::filesystem::path("Content/Maps/five.p8m"))));
	data->maps.push_back(MapData("6", Map(std::filesystem::path("Content/Maps/six.p8m"))));
	data->maps.push_back(MapData("7", Map(std::filesystem::path("Content/Maps/seven.p8m"))));
	data->maps.push_back(MapData("8", Map(std::filesystem::path("Content/Maps/eight.p8m"))));
	data->maps.push_back(MapData("9", Map(std::filesystem::path("Content/Maps/nine.p8m"))));
	data->maps.push_back(MapData("10", Map(std::filesystem::path("Content/Maps/ten.p8m"))));

	data->maps.push_back(MapData("END", Map(std::filesystem::path("Content/Maps/the_end.p8m"))));

	data->state_handler->set(std::make_unique<Menu>(data));

	RenderTexture2D renderer = LoadRenderTexture((int)GameData::GAME_SIZE.x, (int)GameData::GAME_SIZE.y);
	SetTextureFilter(renderer.texture, TEXTURE_FILTER_POINT);

	std::cout << "INFO: Tile size: " << GameData::TILE_SIZE << "\n";
	std::cout << "INFO: Game resolution: " << GameData::GAME_SIZE.x << ", " << GameData::GAME_SIZE.y << "\n";

	std::cout << "INFO: Muted SFX: " << data->mute_sfx << "\n";
	std::cout << "INFO: Muted Move: " << data->mute_move << "\n";

	while (!data->exit)
	{
		if (WindowShouldClose())
		{
			data->exit = true;
			continue;
		}

		// Screen scale
		float scale = fmin(
			GetScreenWidth() / GameData::GAME_SIZE.x,
			GetScreenHeight() / GameData::GAME_SIZE.y
		);

		Vector2 mouse = GetMousePosition();
		Vector2 unclamped = Vector2(
			(mouse.x - (GetScreenWidth() - (GameData::GAME_SIZE.x * scale)) * 0.5f) / scale,
			(mouse.y - (GetScreenHeight() - (GameData::GAME_SIZE.y * scale)) * 0.5f) / scale
		);

		data->virtual_mouse = Vector2(
			Vector2Clamp(
				unclamped,
				Vector2Zero(), GameData::GAME_SIZE
			)
		);

		data->state_handler->process();

		BeginTextureMode(renderer);
		{
			data->state_handler->render();

			if (data->state_handler->switching)
			{
				DrawRectangleV(Vector2Zero(), GameData::GAME_SIZE, data->state_handler->colour());
			}
		}
		EndTextureMode();

		BeginDrawing();
		{
			ClearBackground(BLACK);

			// Fucked up screen scaling calculations
			DrawTexturePro(
				renderer.texture,
				Rectangle(0, 0, (float)renderer.texture.width, (float)-renderer.texture.height),
				Rectangle(
					(GetScreenWidth() - (GameData::GAME_SIZE.x * scale)) * 0.5f,
					(GetScreenHeight() - (GameData::GAME_SIZE.y * scale)) * 0.5f,
					GameData::GAME_SIZE.x * scale, GameData::GAME_SIZE.y * scale
				),
				Vector2Zero(), 0, WHITE
			);
		}
		EndDrawing();
	}
	
	CloseAudioDevice();
	CloseWindow();

	// Release data
	data->state_handler->leave();
	for (MapData map_data : data->maps)
	{
		std::cout << "INFO: Removing data from map " << map_data.name << std::endl;
		map_data.map.leave();
	}

	// According to C++ 11 and later
	// If a program reaches the end without return
	// It is implictly assumed as 0.
}