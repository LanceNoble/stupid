#include "client.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <math.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

SOCKET sguestTCP;
SOCKET sguestUDP;
struct addrinfo* addrTCP;
struct addrinfo* addrUDP;

void initwsa() {
	int res;
	WSADATA wsaData;
	res = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (res != 0)
		exit(res);
}

void join(char* ip, char* port, struct player players[MAX_PLAYERS]) {
	u_long mode = 1;
	int res;
	struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;

	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	res = getaddrinfo(ip, port, &hints, &addrTCP);
	if (res != 0)
		exit(res);
	sguestTCP = socket(addrTCP->ai_family, addrTCP->ai_socktype, addrTCP->ai_protocol);
	if (sguestTCP == INVALID_SOCKET)
		exit(WSAGetLastError());

	res = connect(sguestTCP, addrTCP->ai_addr, addrTCP->ai_addrlen);
	if (res == SOCKET_ERROR)
		exit(WSAGetLastError());
	res = ioctlsocket(sguestTCP, FIONBIO, &mode);
	if (res == SOCKET_ERROR)
		exit(WSAGetLastError());

	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	res = getaddrinfo(ip, port, &hints, &addrUDP);
	if (res != 0)
		exit(res);
	sguestUDP = socket(addrUDP->ai_family, addrUDP->ai_socktype, addrUDP->ai_protocol);
	if (sguestUDP == INVALID_SOCKET)
		exit(WSAGetLastError());

	res = ioctlsocket(sguestUDP, FIONBIO, &mode);
	if (res == SOCKET_ERROR)
		exit(WSAGetLastError());

	ZeroMemory(players, sizeof(*players) * MAX_PLAYERS);

}

void recv_tcp(struct player players[MAX_PLAYERS], struct player** you) {
	union tcp_msg msg;
	int numBytes = recv(sguestTCP, msg.raw, sizeof(msg), 0);
	if (numBytes == SOCKET_ERROR)
		return;
	if (msg.type == 0) {
		*you = &(players[msg.target - 1]);
		(*you)->id = msg.target;
		(*you)->x = 0;
		(*you)->y = 0;

		for (int i = 0; i < MAX_PLAYERS - 1; i++) {
			int mask = pow(2, 2) - 1; 
			int start = i * 2; 
			int id = (msg.ids & (mask << start)) >> start;
			if (id == 0)
				continue;
			players[id - 1].id = id;
			players[id - 1].x = 0;
			players[id - 1].y = 0;
		}
	}
	if (msg.type == 1) {
		players[msg.target - 1].id = msg.target;
		players[msg.target - 1].x = 0;
		players[msg.target - 1].y = 0;
	}
	else if (msg.type == 2)
		ZeroMemory(&(players[msg.target - 1]), sizeof(players[msg.target - 1]));
}

void send_udp(struct player* you) {
	if (you == NULL)
		return;
	union udp_msg msg;
	msg.id = you->id;
	msg.x = you->x;
	msg.y = you->y;
	msg.whole = htonl(msg.whole);
	int numBytes = sendto(sguestUDP, msg.raw, sizeof(msg), 0, addrUDP->ai_addr, addrUDP->ai_addrlen);
}

void recv_udp(struct player players[MAX_PLAYERS], struct player* you) {
	union udp_msg msg;
	int numBytes = recvfrom(sguestUDP, msg.raw, sizeof(msg), 0, NULL, NULL);
	if (numBytes == SOCKET_ERROR)
		return;
	msg.whole = ntohl(msg.whole);
	if (msg.id == you->id)
		return;
	players[msg.id - 1].x = msg.x;
	players[msg.id - 1].y = msg.y;
}

void leave() {
	closesocket(sguestTCP);
	closesocket(sguestUDP);
}

void clean() {
	freeaddrinfo(addrTCP);
	freeaddrinfo(addrUDP);
	WSACleanup();
}
