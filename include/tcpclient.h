#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <string>
#include <thread>
#include <WS2tcpip.h>
#include "pckg.h"

class TCPClient;

typedef void(*MessageReceivedHandler)(std::string msg);

class TCPClient
{
public:

    TCPClient();
    ~TCPClient();
    bool initWinsock();
    bool connectSock();
    bool auth(bool loginflag, std::string& username, std::string& password);
    void sendMsg(std::string txt);
    void threadRecv();
    std::thread recvThread;
    std::string username;
    bool joinChat = true;
    std::string password;

private:
    void createSocket();
    std::string serverIP = "127.0.0.1";
    int serverPort = 54010;
    sockaddr_in hint;
    SOCKET serverSocket;		//This is the socket we will connect to.
    bool recvThreadRunning = 0;
    char buf[sizeof(msgpckg)];
    authpckg regpkcg;

};

#endif // TCPCLIENT_H
