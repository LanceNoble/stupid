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

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#include "client.h"
#include <stdio.h>
#include <time.h>

int main ()
{
	int initwsares = initwsa();
	int joined = 0;
	int players[3] = { 0, 0, 0 };

	// Tell the window to use vysnc and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 800, "Hello Raylib");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");
	
	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		// drawing
		BeginDrawing();

		// Setup the backbuffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		if (joined) goto game;

		DrawText("Multiplayer Test", 0, 0, 64, WHITE);
		if (initwsares != 0) {
			DrawText("Multiplayer Disabled: WSA Init Fail", 0, 64, 32, WHITE);
			goto end;
		}

		// Button
		DrawRectangle(640 - (MeasureText("JOIN", 64) + 64) / 2, 400 - 32, MeasureText("JOIN", 64) + 64, 64, WHITE);
		DrawText("JOIN", 640 - MeasureText("JOIN", 64) / 2, 400 - 32, 64, BLACK);
		if (GetMouseX() > 640 - (MeasureText("JOIN", 64) + 64) / 2 &&
			GetMouseX() < 640 + (MeasureText("JOIN", 64) + 64) / 2 &&
			GetMouseY() > 400 - 32 &&
			GetMouseY() < 400 + 32 &&
			IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			int pipe[4];
			int data[5];
			join("73.119.107.1", "3490", pipe, data);
			for (int i = 2, j = 0; i < 5; i++) { players[j] = data[i]; j++; }
			printf("%i, %i, %i", players[0], players[1], players[2]);
			for (int i = 0; i < 4; i++) if (pipe[i] != 0) return 1;
			joined = 1;
		}

		if (!joined) goto end;
		int data[5] = { 0,0,0,0,0 };
	game:
		sync(data);
		//printf("%lu, %lu, %lu\n\n", players[0], players[1], players[2]);
		if (data[0] == 0) {
			DrawText("Disconnected", 0, 0, 64, WHITE);
			leave();
			joined = 0;
			goto end;
		}
		else if (data[0] == 1 && data[1] == 0) {
			//printf("received\n");
			for (int i = 2, j = 0; i < 5; i++) { 
				players[j] = data[i]; 
				j++; 
			}
		}
		else if (data[1] == 1) {
			for (int i = 0; i < 3; i++) {
				if (players[i] == 0) { 
					players[i] = data[2]; 
					break; 
				}
			}
		}
			
		else if (data[1] == 2) {
			for (int i = 0; i < 3; i++) {
				if (players[i] == data[2]) { 
					players[i] = 0; 
					break; 
				}
			}
		}
			//printf("received\n");
		

		int y = 0;
		for (int i = 0; i < 3; i++) {
			if (players[i] != 0) { DrawText(TextFormat("Player %i", players[i]), 0, y, 32, WHITE); y += 32; }
		}

		DrawRectangle(640 - (MeasureText("LEAVE", 64) + 64) / 2, 400 - 32, MeasureText("LEAVE", 64) + 64, 64, WHITE);
		DrawText("LEAVE", 640 - MeasureText("LEAVE", 64) / 2, 400 - 32, 64, BLACK);
		if (GetMouseX() > 640 - (MeasureText("LEAVE", 64) + 64) / 2 &&
			GetMouseX() < 640 + (MeasureText("LEAVE", 64) + 64) / 2 &&
			GetMouseY() > 400 - 32 &&
			GetMouseY() < 400 + 32 &&
			IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
			leave();
			joined = 0;
		}
	end:
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(wabbit);

	// destory the window and cleanup the OpenGL context
	CloseWindow();

	clean();
	return 0;
}