#include "client.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

void initWSA() {
	WSADATA wsaData;
	int res = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (res != 0) {
		printf("WSAStartup fail: %d\n", res);
		return 1;
	}
}

void clean() {
	WSACleanup();
}
