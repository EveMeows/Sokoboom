#include <iostream>

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

	while (!WindowShouldClose())
	{
		BeginDrawing();
		{
			ClearBackground(RAYWHITE);
		}
		EndDrawing();
	}

	CloseWindow();

	// According to C++ 11 and later
	// If a program reaches the end without return
	// It is implictly assumed as 0.
}