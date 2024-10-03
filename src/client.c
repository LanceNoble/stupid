#include "client.h"
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#define MAX_PLAYERS 3
#define E12B 000000000000
#define F12B 111111111111
#define E8B  00000000
#define F8B  11111111

#define STRIP_CONNECTION_STATUS(cs, type, id1, id2, id3) \
	type = (cs & 0b11000000) >> 6;						 \
	id1  = (cs & 0b00110000) >> 4;						 \
	id2  = (cs & 0b00001100) >> 2;						 \
	id3  =  cs & 0b00000011;

// Construct player position
// b[0]	    - sign
//	      0 - negative
//		  1 - positive
// b[1-11]  - x coord of player 1
// b[12]    - sign
//        0 - negative
//        1 - positive
// b[13-23] - y coord of player 1
#define BUILD_PLAYER_POSITION(spx, px, spy, py) (spx << 23) | (px << 12) | (spy << 11) | py

// Split a 24-bit player position into 3 bytes
#define CHUNK_PLAYER_POSITION(pp, buf) \
	buf[0] == pp & 0b## F8B##E8B##E8B  \
	buf[1] == pp & 0b##      F8B##E8B  \
    buf[2] == pp & 0b##           F8B

// Reattach a 24-bit player position that's been split into 3 bytes
#define GLUE_PLAYER_POSITION(int24, buf) int24 = (buf[0] << 16) | (buf[1] << 8) | buf[2]

#define STRIP_PLAYER_POSITION(pp, x, y) \
	x = pp & (0b##F12B << 12);          \
	y = pp &  0b##F12B;                 

#define STRIP_COORDINATE(coord, sign, num) \
	sign = coord & 0b100000000000;		   \
	num  = coord & 0b011111111111;		   

SOCKET guest;
struct addrinfo* addr;

int initwsa() {
	WSADATA wsaData;
	return WSAStartup(MAKEWORD(2, 2), &wsaData);
}

void join(char* ip, char* port, int pipe[4], int data[5]) {
	u_long mode = 1;
	struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	pipe[0] = getaddrinfo(ip, port, &hints, &addr);
	guest = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
	pipe[1] = WSAGetLastError();
	connect(guest, addr->ai_addr, addr->ai_addrlen);
	pipe[2] = WSAGetLastError();
	sync(data);
	ioctlsocket(guest, FIONBIO, &mode);
	pipe[3] = WSAGetLastError();
}

void sync(int data[5]) {
	int buf[1] = { 0 };
	data[0] = recv(guest, buf, 1, 0);
	data[1] = STRIP_CONNECTION_STATUS(buf[0], 00000011, 0);
	data[2] = STRIP_CONNECTION_STATUS(buf[0], 00001100, 2);
	data[3] = STRIP_CONNECTION_STATUS(buf[0], 00110000, 4);
	data[4] = STRIP_CONNECTION_STATUS(buf[0], 11000000, 6);
}

void leave() {
	closesocket(guest);
}

void clean() {
	freeaddrinfo(addr);
	WSACleanup();
}
