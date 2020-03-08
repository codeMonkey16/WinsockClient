#include <stdio.h>

#include "Socket.h"

// A dotted-decimal IPv4 address or an IPv6 hex address
#define DEAFULT_IP_ADDR "localhost"
#define DEFAULT_PORT "27016"

int __cdecl main(int argc, char **argv)
{
	cSocketManager socketManager;
	if (!socketManager.Connect(DEAFULT_IP_ADDR, DEFAULT_PORT)) return 1;

	if (!socketManager.SendAndRecv("test 1")) return 1;
	if (!socketManager.SendAndRecv("test 2")) return 1;
	if (!socketManager.SendAndRecv("test 3")) return 1;

	socketManager.Close();
	socketManager.Free();
	return 0;
}
