/*
    Steps to set up a Winsock client (from https://docs.microsoft.com/en-us/windows/desktop/winsock/winsock-client-application):
        - Creating a socket.
        - Connect that socket to the IP address & port of the server.
            -> IP address & port of the server are stored in the sockaddr_in variable, hint.
        - Sending & receiving the data on the client.
            -> For the receiving part, we use an external thread to continuously receive data.
            -> For the sending part, we continuously check for input & send the message when the user presses enter.
                -> this part is done in the main.cpp file.
        - Disconnect client.


*/
#include "../include/tcpclient.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

//When we create the client, we do not want the thread to run & try to receive data from the server until
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
        cout << "Error: can't start Winsock." << endl;
        return false;
    }
    return true;
}

void TCPClient::createSocket() {

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cout << "Error: can't create socket." << endl;
        WSACleanup();
    }

    //Specify data for hint structure.
    hint.sin_family = AF_INET;
    hint.sin_port = htons(serverPort);
    inet_pton(AF_INET, serverIP.c_str(), &hint.sin_addr);
}

void TCPClient::threadRecv(){

    while (inetgui->isNetRunning()) {
        
        //inetgui all 4 funcs in this loop
        ZeroMemory(buf, sizeof(buf));

        if(inetgui->getAuth(regpckg)){
            auth();
        }

        if(inetgui->getOutputMsg(msg)){
            send(serverSocket, (const char*)(&msg), sizeof(msgpckg), 0);
        }
        int bytesReceived = recv(serverSocket, buf, sizeof(buf), 0);
        std::cout<<"bytesReceived: " << bytesReceived <<std::endl;
        if (bytesReceived > 0) { //If client disconnects, bytesReceived = 0; if error, bytesReceived = -1;
            if(buf[0] == 1u){
                memcpy(&msg, buf, sizeof(msgpckg));
                while(inetgui->setInputMsg(msg)) std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        }

    }
}

bool TCPClient::connectSock() {

    //If !initWinsock -> return false.
    int connResult = connect(serverSocket, (sockaddr*)&hint, sizeof(hint));
    if (connResult == SOCKET_ERROR) {
        cout << "Error: can't connect to server." << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    }
    DWORD timeout = 5000;
    setsockopt(serverSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
    return 1;
}

void TCPClient::auth(){
    send(serverSocket, (const char*)(&regpckg), sizeof(authpckg), 0);
    recv(serverSocket, buf, sizeof(buf), 0);
    memcpy(&acc, buf, sizeof(acc));
    while(!inetgui->setAccept(acc)) std::this_thread::sleep_for(std::chrono::milliseconds(50));
}
