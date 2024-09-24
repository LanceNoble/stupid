#include "client.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#pragma comment(lib, "Ws2_32.lib")

SOCKET sock;
struct addrinfo* serverAddr;
struct addrinfo hints = { .ai_family = AF_INET,.ai_socktype = SOCK_STREAM,.ai_protocol = IPPROTO_TCP };

void initWSA() {
	WSADATA wsaData;
	int res = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (res != 0) {
		printf("WSAStartup fail: %d\n", res);
		exit(1);
	}
}

void search() {
	int res = getaddrinfo("localhost", "30000", &hints, &serverAddr);
	if (res != 0) {
		printf("getaddrinfo fail: %d\n", res);
		WSACleanup();
		exit(1);
	}
}

void clean() {
	WSACleanup();
}
