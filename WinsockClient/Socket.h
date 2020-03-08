#pragma once

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

using namespace std;

class cSocketManager
{
public:
    BOOL Init();
    BOOL Connect(string IpAddress, string Port);
    BOOL Send(string msg);
    BOOL Recv();
    BOOL GetRecv(string &msg);
    BOOL SendAndRecv(string msg);
    void Close();
    void Free();
private:
    static const int m_DEFAULT_BUFLEN = 512;
    BOOL m_bInit = FALSE;
    BOOL m_bConnected = FALSE;
    BOOL m_bReceived = FALSE;
    char m_aSendBuf[m_DEFAULT_BUFLEN];
    char m_aRecvBuf[m_DEFAULT_BUFLEN];

    SOCKET m_ConnectSocket = INVALID_SOCKET;
    struct addrinfo *m_pAddrInfo = NULL;

    BOOL ResolveAddress(string IpAddress, string Port);
};
