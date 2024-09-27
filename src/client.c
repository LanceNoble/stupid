#include "client.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

#define JOIN "0"
#define LEAVE "1"

SOCKET sock;
struct addrinfo* serverAddr;
struct addrinfo hintsTCP = { .ai_family = AF_INET,.ai_socktype = SOCK_STREAM,.ai_protocol = IPPROTO_TCP };
struct addrinfo hintsUDP = { .ai_family = AF_INET,.ai_socktype = SOCK_DGRAM,.ai_protocol = IPPROTO_UDP };
char recvBuf[8];

static int con() {
	int res;
	sock = socket(serverAddr->ai_family, serverAddr->ai_socktype, serverAddr->ai_protocol);
	if (sock == INVALID_SOCKET) {
		printf("socket fail\n");
		return WSAGetLastError();
	}
	if (connect(sock, serverAddr->ai_addr, serverAddr->ai_addrlen) == SOCKET_ERROR) {
		printf("connect fail\n");
		res = WSAGetLastError();
		closesocket(sock);
		return res;
	}
	return 0;
}

static int dis() {
	shutdown(sock, SD_SEND);
	closesocket(sock);
	return 0;
}

int init() {
	int res;
	WSADATA wsaData;
	if (res = WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return res;
	return 0;
}

int join(const char* ip, const char* port) {
	char recvBuf[8];
	int res = 0;
	if (res = getaddrinfo(ip, port, &hintsTCP, &serverAddr) != 0)
		return 1;
	if (res = con() != 0)
		return 2;
	if (res = send(sock, JOIN, 1, 0) == SOCKET_ERROR)
		return 3;
	if (res = recv(sock, recvBuf, 8, 0) == SOCKET_ERROR)
		return 4;
	if (res = recvBuf[0] != '0')
		return 5;
	dis();
	return 0;
}

int getclinum() {

}

int leave() {
	int res = 0;
	freeaddrinfo(serverAddr);
	if (res = con() != 0)
		return 1;
	if (res = send(sock, LEAVE, 1, 0) == SOCKET_ERROR)
		return 2;
	if (res = recv(sock, recvBuf, 8, 0) == SOCKET_ERROR)
		return 3;
	if (res = recvBuf[0] != '1')
		return 4;
	dis();
	return 0;
}

void clean() {
	closesocket(sock);
	freeaddrinfo(serverAddr);
	WSACleanup();
}
