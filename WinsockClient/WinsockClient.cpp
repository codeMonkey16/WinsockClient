#include <stdio.h>
#include <string>
#include <list>
#include <assert.h>
#include <iostream>

#include "Socket.h"

// A dotted-decimal IPv4 address or an IPv6 hex address
#define DEAFULT_IP_ADDR "localhost"
#define DEFAULT_PORT "27016"

int __cdecl main(int argc, char **argv)
{
    cSocketManager socketManager;
    if (!socketManager.Connect(DEAFULT_IP_ADDR, DEFAULT_PORT)) return 1;
#if 1
	while (true)
	{
		string send;
		cout << "Wait for input..." << endl;
		cin >> send;
		if (!((string)"exit").compare(send))
		{
			cout << "Exit." << endl;
			break;
		}
		string recv;
		if (!socketManager.SendAndRecv(send, recv))
			return 1;
		assert(!send.compare(recv));
	}
#else
	list<string> inputs { "test 1", "test 2", "test 3",
		"this is a long test string to test if the received string is truncated..."};

	for (auto expected : inputs) {
		string recv;
		if (!socketManager.SendAndRecv(expected, recv))
			return 1;
		assert(expected.compare(recv) == 0);
	}
#endif

    socketManager.Close();
    socketManager.Free();
    return 0;
}
