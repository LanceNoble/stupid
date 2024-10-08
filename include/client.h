#pragma once
#define MAX_PLAYERS 3
#define VACANT_PLAYER 0

union tcp_msg {
	struct {
		unsigned char type : 2;
		unsigned char target : 2;
		unsigned char ids : 4;
	};
	char raw[1];
};
union udp_msg {
	struct {
		unsigned int id : 2;
		signed   int x : 15;
		signed   int y : 15;
	};
	int whole;
	char raw[4];
};
struct player {
	unsigned int id;
	signed int x;
	signed int y;
};
void initwsa();
void join(char* ip, char* port, struct player players[MAX_PLAYERS]);
void recv_tcp(struct player players[MAX_PLAYERS], struct player** you);
void send_udp(struct player* you);
void recv_udp(struct player players[MAX_PLAYERS], struct player* you);
void leave();
void clean();