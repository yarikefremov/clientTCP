#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <string>
#include <WS2tcpip.h>
#include "pckg.h"
#include "inetgui.h"
#include <thread>
#include <chrono>

class TCPClient
{
public:

    TCPClient(NetworkGUIInterface* inetgui);
    ~TCPClient();
    bool initWinsock();
    bool connectSock();
    void auth();
    void sendMsg(std::string txt);
    void threadRecv();
    std::string username;
    bool joinChat = true;
    std::string password;

private:
    NetworkGUIInterface* inetgui;
    void createSocket();
    int serverPort = 54010;
    sockaddr_in hint;
    SOCKET serverSocket;		//This is the socket we will connect to.
    char buf[sizeof(msgpckg)];
    std::string serverIP = "127.0.0.1";
    acceptpckg acc;
    authpckg regpckg;
    msgpckg msg;

};

#endif // TCPCLIENT_H
