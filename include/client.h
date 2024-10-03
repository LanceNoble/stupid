#pragma once
int initwsa();
void join(const char* ip, const char* port, int pipe[4], int data[5]);
void sync(int data[5]);
void leave();
void clean();