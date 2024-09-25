#include "client.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#pragma comment(lib, "Ws2_32.lib")

SOCKET sock;
struct addrinfo* serverAddr;
struct addrinfo hintsTCP = { .ai_family = AF_INET,.ai_socktype = SOCK_STREAM,.ai_protocol = IPPROTO_TCP };
struct addrinfo hintsUDP = { .ai_family = AF_INET,.ai_socktype = SOCK_DGRAM,.ai_protocol = IPPROTO_UDP };

static int con() {
	sock = socket(serverAddr->ai_family, serverAddr->ai_socktype, serverAddr->ai_protocol);
	if (sock == INVALID_SOCKET) {
		printf("socket fail: %d\n", WSAGetLastError());
		return 1;
	}
	int res;
	/*
	u_long mode = 1;
	if (res = ioctlsocket(sock, FIONBIO, &mode) == SOCKET_ERROR) {
		printf("non-block fail: %d\n", WSAGetLastError());
		closesocket(sock);
	}
	select(0, NULL, NULL, NULL, NULL);
	*/
	if (res = connect(sock, serverAddr->ai_addr, serverAddr->ai_addrlen) == SOCKET_ERROR) {
		printf("connect fail: %d\n", WSAGetLastError());
		closesocket(sock);
	}
	return res;
}

static int dis() {
	// Graceful disconnect: https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-shutdown
	// Don't care about the contents of remaining data, just want to clean it all up
	shutdown(sock, SD_SEND);
	int bytes;
	do {
		char can[1];
		if (bytes = recv(sock, can, 1, 0) == SOCKET_ERROR) {
			printf("failed to gracefully disconnect: %ld\n", WSAGetLastError());
			break;
		}
	} while (bytes != 0);
	closesocket(sock);
	return 0;
}

int init() {
	WSADATA wsaData;
	int res;
	if (res = WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		printf("WSAStartup fail: %d\n", res);
	else if (res = getaddrinfo("localhost", "30000", &hintsTCP, &serverAddr) != 0) {
		printf("getaddrinfo fail: %d\n", res);
		WSACleanup();
	}
	return res;
}

int join() {
	con();
	int res = 0;
	if (res = send(sock, "j\0", 2, 0 == SOCKET_ERROR))
		printf("send fail: %d\n", WSAGetLastError());
	dis();
	return 0;
}

int move() {

}

int leave() {
	con();
	int res = 0;
	if (res = send(sock, "l\0", 2, 0 == SOCKET_ERROR))
		printf("send fail: %d\n", WSAGetLastError());
	dis();
	return 0;
}

void clean() {
	closesocket(sock);
	freeaddrinfo(serverAddr);
	WSACleanup();
}
