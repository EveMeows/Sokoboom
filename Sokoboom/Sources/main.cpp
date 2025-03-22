#include <iostream>
#include <memory>
#include <cmath>

#include "../Headers/data.h"
#include "../Headers/States/game.h"

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
	SetTargetFPS(60);

	std::cout << "INFO: Tile size: " << GameData::TILE_SIZE << "\n";
	std::cout << "INFO: Game resolution: " << GameData::GAME_SIZE.x << ", " << GameData::GAME_SIZE.y << "\n";

	std::shared_ptr<GameData> data = std::make_shared<GameData>();
	Map map = Map(std::filesystem::path("Content/Maps/intro.p8m"));
	
	data->state_handler->set(
		std::make_unique<Game>(Game(data, &map))
	);

	RenderTexture2D renderer = LoadRenderTexture(GameData::GAME_SIZE.x, GameData::GAME_SIZE.y);
	SetTextureFilter(renderer.texture, TEXTURE_FILTER_POINT);

	while (!WindowShouldClose())
	{
		float scale = fmin(
			GetScreenWidth() / GameData::GAME_SIZE.x,
			GetScreenHeight() / GameData::GAME_SIZE.y
		);

		Vector2 mouse = GetMousePosition();
		data->virtual_mouse = Vector2(
			Vector2Clamp(
				Vector2(
					(mouse.x - (GetScreenWidth() - (GameData::GAME_SIZE.x * scale)) * 0.5f) / 2, 
					(mouse.y - (GetScreenHeight() - (GameData::GAME_SIZE.y * scale)) * 0.5f) / 2
				),
				Vector2Zero(), GameData::GAME_SIZE
			)
		);

		data->state_handler->process();

		BeginTextureMode(renderer);
		{
			data->state_handler->render();
		}
		EndTextureMode();

		BeginDrawing();
		{
			ClearBackground(BLACK);

			DrawTexturePro(
				renderer.texture,
				Rectangle(0, 0, renderer.texture.width, -renderer.texture.height),
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

	CloseWindow();

	// According to C++ 11 and later
	// If a program reaches the end without return
	// It is implictly assumed as 0.
}