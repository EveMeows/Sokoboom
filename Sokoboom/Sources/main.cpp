#include <iostream>
#include <memory>

#include "../Headers/data.h"
#include "../Headers/States/game.h"

#include <raylib.h>

// Disable terminal in release mode only
#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif // !_DEBUG

int main()
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 600, "Sokoboom");
	SetTargetFPS(60);

	std::shared_ptr<GameData> data = std::make_shared<GameData>();
	data->state_handler->set(std::make_unique<Game>(Game(data)));

	std::cout << "INFO: Tile size: " << data->TILE_SIZE << "\n";
	std::cout << "INFO: Game resolution: " << data->GAME_SIZE.x << ", " << data->GAME_SIZE.y << "\n";

	while (!WindowShouldClose())
	{
		data->state_handler->process();
		data->state_handler->render();
	}

	CloseWindow();

	// According to C++ 11 and later
	// If a program reaches the end without return
	// It is implictly assumed as 0.
}