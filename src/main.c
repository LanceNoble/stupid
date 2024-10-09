/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

For a C++ project simply rename the file to .cpp and re-run the build script 

-- Copyright (c) 2020-2024 Jeffery Myers
--
--This software is provided "as-is", without any express or implied warranty. In no event 
--will the authors be held liable for any damages arising from the use of this software.

--Permission is granted to anyone to use this software for any purpose, including commercial 
--applications, and to alter it and redistribute it freely, subject to the following restrictions:

--  1. The origin of this software must not be misrepresented; you must not claim that you 
--  wrote the original software. If you use this software in a product, an acknowledgment 
--  in the product documentation would be appreciated but is not required.
--
--  2. Altered source versions must be plainly marked as such, and must not be misrepresented
--  as being the original software.
--
--  3. This notice may not be removed or altered from any source distribution.

*/

#include "raylib.h"

#include "client.h"

int main ()
{
	initwsa();
	
	int joined = 0;
	struct player players[MAX_PLAYERS];
	struct player* you = NULL;

	// Tell the window to use vysnc and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 800, "Hello Raylib");
	
	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		// drawing
		BeginDrawing();

		// Setup the backbuffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		if (joined) goto game;

		// Button
		DrawRectangle(640 - (MeasureText("JOIN", 64) + 64) / 2, 400 - 32, MeasureText("JOIN", 64) + 64, 64, WHITE);
		DrawText("JOIN", 640 - MeasureText("JOIN", 64) / 2, 400 - 32, 64, BLACK);
		if (GetMouseX() > 640 - (MeasureText("JOIN", 64) + 64) / 2 &&
			GetMouseX() < 640 + (MeasureText("JOIN", 64) + 64) / 2 &&
			GetMouseY() > 400 - 32 &&
			GetMouseY() < 400 + 32 &&
			IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			join("localhost", "3490", players, &you);
			joined = 1;
		}

		if (!joined) goto end;

	game:
		sync(players, &you);

		if (you != NULL) {
			int x = 0;
			int y = 0;
			if (IsKeyDown(KEY_W))
				y -= 2;
			if (IsKeyDown(KEY_A))
				x -= 2;
			if (IsKeyDown(KEY_S))
				y += 2;
			if (IsKeyDown(KEY_D))
				x += 2;
			if (x != 0 || y != 0)
				notify(you, x, y);
		}

		DrawRectangle(1280 - (MeasureText("LEAVE", 64) + 64), 0, MeasureText("LEAVE", 64) + 64, 64, WHITE);
		DrawText("LEAVE", 1280 - (MeasureText("LEAVE", 64) + 32), 0, 64, BLACK);
		if (GetMouseX() > 1280 - (MeasureText("LEAVE", 64) + 64) &&
			GetMouseX() < 1280 &&
			GetMouseY() > 0 &&
			GetMouseY() < 64 &&
			IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
			leave();
			you = NULL;
			joined = 0;
			goto end;
		}

		for (int i = 0; i < MAX_PLAYERS; i++)
			if (players[i].id != VACANT_PLAYER) 
				DrawText(TextFormat("Player %i", players[i].id), 0, i * 32, 32, WHITE);
		for (int i = 0; i < MAX_PLAYERS; i++)
			if (players[i].id != VACANT_PLAYER)
				DrawRectangle(players[i].x, players[i].y, 30, 30, WHITE);
	end:

		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// destory the window and cleanup the OpenGL context
	CloseWindow();
	clean();
	return 0;
}