// 10/08/2024
// This program is the client portion of my attempt at writing netcode
// 
// Successful Data Syncs:
// - List of connected players
// - Player positions
// - 
// 
// TODO:
// 1.

#include "client.h"

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

SOCKET guest;

union msg {
	struct {
		unsigned int type : 2;
		unsigned int id : 2;
		signed int xPos : 14;
		signed int yPos : 14;
	};
	int whole;
	char raw[4];
};

// Initialize Winsock
void initwsa() {
	int res;
	WSADATA wsaData;
	res = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (res != 0)
		exit(res);
}

// Join a server
void join(char* ip, char* port, struct player players[MAX_PLAYERS]) {
	u_long mode = 1; // On switch for activating non-block mode for sockets
	int status; // Status of every socket operation

	struct addrinfo* addr;
	struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;

	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	status = getaddrinfo(ip, port, &hints, &addr);
	if (status != 0) {
		WSACleanup();
		exit(status);
	}

	guest = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
	if (guest == INVALID_SOCKET) {
		status = WSAGetLastError();
		freeaddrinfo(addr);
		WSACleanup();
		exit(status);
	}

	status = connect(guest, addr->ai_addr, addr->ai_addrlen);
	if (status == SOCKET_ERROR) {
		status = WSAGetLastError();
		closesocket(guest);
		freeaddrinfo(addr);
		WSACleanup();
		exit(status);
	}

	// 
		
	freeaddrinfo(addr);

	ZeroMemory(players, sizeof(*players) * MAX_PLAYERS);
	status = ioctlsocket(guest, FIONBIO, &mode);
	if (status == SOCKET_ERROR) {
		status = WSAGetLastError();
		closesocket(guest);
		WSACleanup();
		exit(status);
	}
}

void sync(struct player players[MAX_PLAYERS], struct player** you) {
	union msg msg;
	msg.whole = 0;
	int numBytes = recv(guest, msg.raw, sizeof(msg), 0);
	if (numBytes == SOCKET_ERROR)
		return;

	if (numBytes == 0) {
		closesocket(guest);
		WSACleanup();
		exit(-1);
	}

	msg.whole = ntohl(msg.whole);
	if (msg.type == 1) {
		players[msg.id - 1].id = msg.id;
		players[msg.id - 1].x = msg.xPos;
		players[msg.id - 1].y = msg.yPos;
		if (*you == NULL)
			*you = &(players[msg.id - 1]);
	}
	else if (msg.type == 2) 
		ZeroMemory(&(players[msg.id - 1]), sizeof(players[msg.id - 1]));
	else if (msg.type == 3) {
		players[msg.id - 1].x = msg.xPos;
		players[msg.id - 1].y = msg.yPos;
	}
}

void notify(struct player* you, signed int x, signed int y) {
	if (you == NULL)
		return;
	union msg msg;
	msg.type = 3;
	msg.id = you->id;
	msg.xPos = you->x + x;
	msg.yPos = you->y + y;

	msg.whole = htonl(msg.whole);
	send(guest, msg.raw, sizeof(msg), 0);
}

void leave() {
	closesocket(guest);
}

void clean() {
	WSACleanup();
}
