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

using namespace std;


//When we create the client, we do not want the thread to run & try to receive data from the server until
TCPClient::TCPClient()
{
    if (initWinsock()) {
        createSocket();
        connectSock();

        recvThread = std::thread();//TODO FIX
        recvThread.detach();
    }
}

TCPClient::~TCPClient()
{
    closesocket(serverSocket);
    WSACleanup();
    if (recvThreadRunning) {
        recvThreadRunning = false;
        recvThread.join();	//Destroy safely to thread.
    }
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

    recvThreadRunning = true;
    while (recvThreadRunning) {

        ZeroMemory(buf, sizeof(buf));

        int bytesReceived = recv(serverSocket, buf, sizeof(buf), 0);
        if (bytesReceived > 0) { //If client disconnects, bytesReceived = 0; if error, bytesReceived = -1;

            if(buf[0] == 1u){
                //At this step we should netgui interface
                std::cout << string(buf+1, 33) <<": " << string(buf+68, 100) << std::endl;
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
    return 1;
}

bool TCPClient::auth(bool loginflag, std::string& username, std::string& password){

    this->username = username;
    this->password = password;
    memcpy(regpkcg.username, this->username.c_str(), sizeof(username));
    memcpy(regpkcg.password, this->password.c_str(), sizeof(password));
    send(serverSocket, (const char*)(&regpkcg), sizeof(authpckg), 0);
    recv(serverSocket, buf, sizeof(buf), 0);
    if(buf[0] != 2u) return 0;
    return buf[2];
}

void TCPClient::sendMsg(string txt) {

    if (!txt.empty() && serverSocket != INVALID_SOCKET) {

        send(serverSocket, txt.c_str(), txt.size() + 1, 0);

        //It wouldn't work with the previous version bc while we were constantly listening for received msgs, we would keep caling this fct.
        //This fct would send the message & try to handle the receiving too. It would get stuck while waiting for a received msg.
    }

}
