// Some RayLib functions have the same name as Windows functions, 
// So we can't include RayLib and Windows in the same source file
// So we simply write our client in a separate source file
// And link its functions to our main source file via a header file
#pragma once
#define MAX_PLAYERS 3
#define VACANT_PLAYER 0
struct player {
	unsigned int id;
	signed int x;
	signed int y;
};
void initwsa();
void join(char* ip, char* port, struct player players[MAX_PLAYERS]);
void sync(struct player players[MAX_PLAYERS], struct player** you);
void notify(struct player* you, signed int x, signed int y);
void leave();
void clean();