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

int main ()
{
	init();

	// Tell the window to use vysnc and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 800, "Hello Raylib");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");
	
	Color colStatus = GRAY;
	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		// drawing
		BeginDrawing();

		// Setup the backbuffer for drawing (clear color and depth buffers)
		ClearBackground(GRAY);

		// draw some text using the default font
		DrawText("Multiplayer Test", 0, 0, 64, BLACK);

		Color colBut = BLACK;
		Color colTxtBut = WHITE;
		if (GetMouseX() > 560 && GetMouseX() < 720 && GetMouseY() > 350 && GetMouseY() < 450) {
			colBut = WHITE;
			colTxtBut = BLACK;
		}
		const char* text = "Join";
		int fontSize = 64;
		DrawRectangle(560, 350, 160, 100, colBut);
		DrawText(text, 640 - MeasureText(text, fontSize) / 2, 370, fontSize, colTxtBut);
		if (GetMouseX() > 560 && GetMouseX() < 720 && GetMouseY() > 350 && GetMouseY() < 450 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			colStatus = BLACK;
		}

		const char* textStatus = "Joining...";
		int szTxtStat = 32;
		DrawText(textStatus, 640 - MeasureText(textStatus, 32) / 2, 600, szTxtStat, colStatus);


		// draw our texture to the screen
		// DrawTexture(wabbit, 400, 200, WHITE);
		
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