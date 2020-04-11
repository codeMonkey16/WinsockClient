#include <stdio.h>
#include <string>
#include <list>
#include <assert.h>

#include "Socket.h"

// A dotted-decimal IPv4 address or an IPv6 hex address
#define DEAFULT_IP_ADDR "localhost"
#define DEFAULT_PORT "27016"

int __cdecl main(int argc, char **argv)
{
    cSocketManager socketManager;
    if (!socketManager.Connect(DEAFULT_IP_ADDR, DEFAULT_PORT)) return 1;

	list<string> inputs { "test 1", "test 2", "test 3",
		"this is a long test string to test if the received string is truncated..."};

	for (auto expected : inputs) {
		string recv;
		if (!socketManager.SendAndRecv(expected, recv))
			return 1;
		assert(expected.compare(recv) == 0);
	}

    socketManager.Close();
    socketManager.Free();
    return 0;
}
