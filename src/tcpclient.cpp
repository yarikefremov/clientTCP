#include "../include/tcpclient.h"
#include <iostream>
TCPClient::TCPClient(NetworkGUIInterface* inetgui)
{
    this->inetgui = inetgui;
    if (initWinsock()) {
        createSocket();
        connectSock();
    }
}

TCPClient::~TCPClient()
{
    closesocket(serverSocket);
    WSACleanup();
}


bool TCPClient::initWinsock() {

    WSADATA data;
    WORD ver = MAKEWORD(2, 2);
    int wsResult = WSAStartup(ver, &data);
    if (wsResult != 0) {
        return false;
    }
    return true;
}

void TCPClient::createSocket() {

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        WSACleanup();
    }
    std::cout<<"+1";
    //Specify data for hint structure.
    hint.sin_family = AF_INET;
    hint.sin_port = htons(serverPort);
    inet_pton(AF_INET, serverIP.c_str(), &hint.sin_addr);
}

void TCPClient::threadRecv(){

    while (inetgui->isNetRunning()) {
        
        //inetgui all 4 funcs in this loop
        ZeroMemory(buf, sizeof(buf));
        if(inetgui->flags[2])
        if(inetgui->getAuth(regpckg)){
            auth();
        }

        if(inetgui->flags[1])
        if(inetgui->getOutputMsg(msg)){
            send(serverSocket, (const char*)(&msg), sizeof(msgpckg), 0);
        }
        int bytesReceived = recv(serverSocket, buf, sizeof(buf), 0);
        if (bytesReceived > 0) { //If client disconnects, bytesReceived = 0; if error, bytesReceived = -1;
            if(buf[0] == 1u){
                memcpy(&msg, buf, sizeof(msgpckg));
                while(!inetgui->setInputMsg(msg)) std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        }

    }
}

bool TCPClient::connectSock() {

    //If !initWinsock -> return false.
    int connResult = connect(serverSocket, (sockaddr*)&hint, sizeof(hint));
    if (connResult == SOCKET_ERROR) {
        closesocket(serverSocket);
        WSACleanup();
        std::cout<<"-2";
        return 0;
    }
    DWORD timeout = 1000;
    setsockopt(serverSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
    std::cout<<"+2";
    return 1;
}

void TCPClient::auth(){
    send(serverSocket, (const char*)(&regpckg), sizeof(authpckg), 0);
    recv(serverSocket, buf, sizeof(buf), 0);
    memcpy(&acc, buf, sizeof(acc));
    while(!inetgui->setAccept(acc)) std::this_thread::sleep_for(std::chrono::milliseconds(50));
}
