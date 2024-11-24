#include "../include/gui.h"
#include "../include/tcpclient.h"
#include "../include/inetgui.h"

void netThreadFunc(TCPClient* client){
    client->threadRecv();
}

int main() {
    NetworkGUIInterface inetgui;
    TCPClient tcp(&inetgui);
    GUI gui(&inetgui);
    tcp.threadRecv();
    std::thread netThread(netThreadFunc, &tcp);
    gui.netThread = &netThread;
    gui.init();
    return 0;
}
