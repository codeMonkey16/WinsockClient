// Must include winsock2.h before windows.h because windows.h includes winsock.h
#include <winsock2.h>
#include <ws2tcpip.h>

#include "Socket.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

BOOL cSocketManager::Init()
{
    if (m_bInit) return TRUE;

    ZeroMemory(m_aSendBuf, m_DEFAULT_BUFLEN);
    ZeroMemory(m_aRecvBuf, m_DEFAULT_BUFLEN);

    m_bInit = TRUE;
    return TRUE;
}

BOOL cSocketManager::ResolveAddress(string IpAddress, string Port)
{
    WSADATA wsaData;
	struct addrinfo hints;
    int ret;

    // Initialize Winsock
	ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (ret != 0)
	{
		printf("WSAStartup failed with error: %d\n", ret);
		return FALSE;
	}

    ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	ret = getaddrinfo(IpAddress.c_str(), Port.c_str(), &hints, &m_pAddrInfo);
	if (ret != 0)
	{
		printf("getaddrinfo failed with error: %d\n", ret);
		WSACleanup();
		return FALSE;
	}
    return TRUE;
}

BOOL cSocketManager::Connect(string IpAddress, string Port)
{
    if (!m_bInit)
    if (!Init())
    {
        return FALSE;
    }

    if (!ResolveAddress(IpAddress, Port))
    {
        return FALSE;
    }

    m_bConnected = FALSE;
    // Attempt to connect to an address until one succeeds
	for (struct addrinfo *pAddrInfo = m_pAddrInfo; pAddrInfo != NULL; pAddrInfo = pAddrInfo->ai_next)
	{
		// Create a SOCKET for connecting to server
		m_ConnectSocket = socket(pAddrInfo->ai_family, pAddrInfo->ai_socktype, pAddrInfo->ai_protocol);
		if (m_ConnectSocket == INVALID_SOCKET)
		{
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return FALSE;
		}

		// Connect to server.
		int ret = connect(m_ConnectSocket, pAddrInfo->ai_addr, (int)pAddrInfo->ai_addrlen);
		if (ret == SOCKET_ERROR)
		{
			closesocket(m_ConnectSocket);
			m_ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(m_pAddrInfo);

    if (m_ConnectSocket == INVALID_SOCKET)
	{
		printf("Unable to connect to server!\n");
		WSACleanup();
        return FALSE;
	}

    m_bConnected = TRUE;
    return m_bConnected;
}

BOOL cSocketManager::Send(string msg)
{
    if (!m_bConnected) return FALSE;

    const int length = sizeof(msg);
    memcpy(m_aSendBuf, msg.c_str(), length);

    // Send an buffer
	int ret = send(m_ConnectSocket, m_aSendBuf, length, 0);
	if (ret == SOCKET_ERROR)
	{
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(m_ConnectSocket);
		WSACleanup();
		return FALSE;
	}

	printf("Bytes Sent: %ld\n", ret);

    return TRUE;
}

BOOL cSocketManager::Recv()
{
    if (!m_bConnected) return FALSE;

    int ret = recv(m_ConnectSocket, m_aRecvBuf, m_DEFAULT_BUFLEN, 0);

    if (ret > 0)
    {
        printf("Bytes received: %d\n", ret);
        m_bReceived = TRUE;
        return TRUE;
    }

    if (0 == ret)
        printf("Connection closed\n");
    else
    {
        printf("recv failed with error: %d\n", WSAGetLastError());
        closesocket(m_ConnectSocket);
        WSACleanup();
    }
    return FALSE;
}

BOOL cSocketManager::GetRecv(string &msg)
{
	if (!m_bReceived) return FALSE;

    msg = string(m_aRecvBuf);
    ZeroMemory(m_aRecvBuf, m_DEFAULT_BUFLEN);
    m_bReceived = FALSE;
	return TRUE;
}

BOOL cSocketManager::SendAndRecv(string msg)
{
    if (!Send(msg)) return FALSE;

	if (!Recv()) return FALSE;

	string msgRecv;
	if (!GetRecv(msgRecv)) return FALSE;
	printf("SendAndRecv: %s\n", msgRecv.c_str());

    return TRUE;
}

void  cSocketManager::Close()
{
    int ret = shutdown(m_ConnectSocket, SD_SEND);
	if (ret == SOCKET_ERROR)
    {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(m_ConnectSocket);
		WSACleanup();
	}
}

void cSocketManager::Free()
{
	closesocket(m_ConnectSocket);
	WSACleanup();
}
