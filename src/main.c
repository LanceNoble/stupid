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
#include "stdio.h"
#include <time.h>

int main ()
{
	int res = init();

	// Tell the window to use vysnc and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 800, "Hello Raylib");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");
	
	int joined = 0;
	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		// drawing
		BeginDrawing();

		// Setup the backbuffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		if (res != 0) {
			DrawText("Multiplayer Disabled: Failed to init WSA", 0, 0, 32, WHITE);
			goto drawEnd;
		}

		DrawRectangle(560, 350, 160, 100, WHITE);
		if (GetMouseX() > 560 && GetMouseX() < 720 &&
			GetMouseY() > 350 && GetMouseY() < 450 &&
			IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			if (!joined) {
				DrawText("JOINING", 0, 0, 32, WHITE);
				int resJoin = join("localhost", "30000");
				if (resJoin == 0)
					joined = 1;
			}
			else {
				DrawText("LEAVING", 0, 0, 32, WHITE);
				if (leave() == 0)
					joined = 0;
			}
		}

		if (!joined) 
			DrawText("DISCONNECTED: Click on the button to JOIN", 0, 0, 32, WHITE);
		else
			DrawText("CONNECTED: Click on the button to LEAVE", 0, 0, 32, WHITE);

	drawEnd:
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